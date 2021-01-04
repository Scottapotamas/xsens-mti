#include "string.h"

#include "xsens_mdata2.h"
#include "xsens_mti.h"
#include "xsens_mti_private.h"
#include "xsens_utility.h"

interface_t *most_recent_interface;

message_handler_ref_t inbound_handler_table[] = {
    { .id = MT_WAKEUP, .handler_fn = NULL },
    { .id = MT_GOTOCONFIGACK, .handler_fn = NULL },
    { .id = MT_GOTOMEASUREMENTACK, .handler_fn = NULL },
    { .id = MT_RESETACK, .handler_fn = NULL },
    { .id = MT_DEVICEID, .handler_fn = &xsens_internal_handle_device_id },
    { .id = MT_PRODUCTCODE, .handler_fn = &xsens_internal_handle_product_code },
    { .id = MT_HARDWAREVERSION, .handler_fn = &xsens_internal_handle_hardware_version },
    { .id = MT_FIRMWAREREV, .handler_fn = &xsens_internal_handle_firmware_version },
    { .id = MT_SELFTESTRESULTS, .handler_fn = &xsens_internal_handle_selftest_results },
    { .id = MT_MT_ERROR, .handler_fn = &xsens_internal_handle_error },
    { .id = MT_WARNING, .handler_fn = NULL },
    { .id = MT_CONFIGURATION, .handler_fn = NULL },
    { .id = MT_MTDATA2, .handler_fn = &xsens_internal_handle_mdata2 },
    { .id = MT_REQLATLONALTACK, .handler_fn = NULL },
    { .id = MT_SETLATLONALTACK, .handler_fn = NULL },
    { .id = MT_REQFILTERPROFILEACK, .handler_fn = NULL },
    { .id = MT_REQGNSSPLATFORMACK, .handler_fn = NULL },
    { .id = MT_REQGNSSLEVERARMACK, .handler_fn = NULL },
    { .id = MT_SETGNSSLEVERARMACK, .handler_fn = NULL },
    { .id = MT_UTCTIME, .handler_fn = NULL },
    { .id = MT_ICCCOMMANDACK, .handler_fn = NULL },
    { .id = MT_SETINITIALHEADINGACK, .handler_fn = NULL },
    { .id = MT_FORWARDGNSSDATAACK, .handler_fn = NULL },
};

// Helper to parse inbound data with one call
void xsens_mti_parse_buffer( interface_t *interface, uint8_t *buffer, uint16_t size )
{
    for( uint16_t i = 0; i < size; i++ )
    {
        xsens_mti_parse( interface, buffer[i] );
    }
}

// Run each byte through the packet-level statemachine
void xsens_mti_parse( interface_t *interface, uint8_t byte )
{
    // CRC is the sum of bytes including the CRC byte (ex PREAMBLE)
    if( interface->state != PARSER_PREAMBLE )
    {
        interface->crc += ( byte & 0xFF );
    }

    switch( interface->state )
    {
        case PARSER_PREAMBLE:
            if( byte == PREAMBLE_BYTE )
            {
                xsens_mti_reset_parser( interface );
                interface->state = PARSER_ADDRESS;
            }
            break;

        case PARSER_ADDRESS:
            if( byte == ADDRESS_BYTE )
            {
                interface->state = PARSER_MESSAGE_ID;
            }
            break;

        case PARSER_MESSAGE_ID:
            interface->packet.message_id = byte;
            interface->state             = PARSER_LENGTH;
            break;

        case PARSER_LENGTH:
            if( byte == LENGTH_EXTENDED_MODE )
            {
                //                printf("Extended packet?\n");
                interface->state = PARSER_LENGTH_EXTENDED_B1;
            }
            if( byte == LENGTH_NONE )
            {
                interface->packet.length = 0;
                interface->state         = PARSER_CRC;
            }
            else
            {
                interface->packet.length = byte;
                interface->state         = PARSER_PAYLOAD;
            }
            break;

        case PARSER_LENGTH_EXTENDED_B1:
            interface->packet.length = byte;
            // TODO decode long length
            interface->state = PARSER_LENGTH_EXTENDED_B2;
            break;

        case PARSER_LENGTH_EXTENDED_B2:
            // TODO decode long length
            interface->packet.length &= byte << 8;
            interface->state = PARSER_PAYLOAD;
            break;

        case PARSER_PAYLOAD:
            interface->packet.payload[interface->payload_pos] = byte;
            interface->payload_pos++;

            // Once we've buffered the whole payload,
            // prepare to read the CRC
            if( interface->payload_pos >= interface->packet.length )
            {

                interface->state = PARSER_CRC;
            }
            break;

        case PARSER_CRC:
            // Check if CRC is valid
            if( interface->crc == 0x00 )
            {
                // Packet was successfully recieved
                // Run the payload handling function
                xsens_mti_handle_payload( interface );
            }
            else
            {
                //                printf("CRC Fail: 0x%x ID: 0x%X?\n", interface->crc, interface->packet.message_id);
            }

            interface->state = PARSER_PREAMBLE;
            break;
    }
}

void xsens_mti_reset_parser( interface_t *interface )
{
    // Clear the parser state and buffers
    memset( &( interface->packet ), 0, sizeof( packet_buffer_t ) );
    interface->payload_pos = 0;
    interface->crc         = 0;
}

// With a valid packet, process the payload
void xsens_mti_handle_payload( interface_t *interface )
{
    packet_buffer_t *packet = &( interface->packet );

    // Search the inbound handler table for a match
    message_handler_ref_t *handler = xsens_mti_find_inbound_handler_entry( packet->message_id );

    // If the ID is recognised, call the handler function (if it exists)
    if( handler )
    {
        callback_payload_t payload_handler_fn = (callback_payload_t)handler->handler_fn;
        if( payload_handler_fn )
        {
            most_recent_interface = interface;    // internally cache the interface for cb access
            payload_handler_fn( packet );
        }
    }
}

bool xsens_mti_override_id_handler( uint8_t id, callback_payload_t *user_fn )
{
    if( user_fn )
    {
        // Find the ID in the inbound handler 'jump table'
        message_handler_ref_t *handler = xsens_mti_find_inbound_handler_entry( id );

        if( handler )
        {
            handler->handler_fn = user_fn;
            return true;
        }
    }

    return false;
}

message_handler_ref_t *xsens_mti_find_inbound_handler_entry( uint8_t find_id )
{

    uint8_t table_length = sizeof( inbound_handler_table ) / sizeof( message_handler_ref_t );

    for( uint8_t i = 0; i < table_length; i++ )
    {
        if( inbound_handler_table[i].id == find_id )
        {
            return &inbound_handler_table[i];
        }
    }

    return (message_handler_ref_t *)NULL;
}

void xsens_mti_send( void )
{
}

void xsens_internal_handle_device_id( packet_buffer_t *packet )
{
    //    printf("DeviceID Handler\n");
    if( packet->length == 4 )
    {
        // MTi 1, 10, 100
        uint32_t version = coalesce_32BE_32LE( &packet->payload[0] );
        //        printf("  DeviceID: %d\n", version);
    }
    else if( packet->length == 8 )
    {
        // MTi600
        // TODO: untested
        uint32_t version = coalesce_32BE_32LE( &packet->payload[4] );
    }
}

void xsens_internal_handle_product_code( packet_buffer_t *packet )
{
    //    printf("ProductCode Handler\n");
    // ASCII formatted code max 20 bytes
}

void xsens_internal_handle_hardware_version( packet_buffer_t *packet )
{
    //    printf("HardwareVersion Handler\n");
    uint8_t hw_version[2];

    uint16_t *hw_ptr = (uint16_t *)&hw_version;
    hw_ptr           = coalesce_16BE_16LE( &packet->payload[0] );

    //    printf("  %d, %d\n", hw_version[0], hw_version[1]);
}

void xsens_internal_handle_firmware_version( packet_buffer_t *packet )
{
    //    printf("FirmwareVersion Handler\n");
    uint8_t  major    = packet->payload[0];
    uint8_t  minor    = packet->payload[1];
    uint8_t  revision = packet->payload[2];
    uint32_t build    = coalesce_32BE_32LE( &packet->payload[3] );
    uint32_t scm      = coalesce_32BE_32LE( &packet->payload[7] );

    //    printf("    Major: %d\n", major);
    //    printf("    Minor: %d\n", minor);
    //    printf("    Rev:   %d\n", revision);
    //    printf("    Build: %d\n", build);
    //    printf("    SCM:   %d\n", scm);
}

void xsens_internal_handle_selftest_results( packet_buffer_t *packet )
{
    //    printf("SelfTest Handler\n");
}

void xsens_internal_handle_error( packet_buffer_t *packet )
{
    uint8_t error_code = packet->payload[0];

    switch( error_code )
    {
        case ERROR_PERIOD_INVALID:
            //            printf("ERROR_PERIOD_INVALID\n");
            break;

        case ERROR_MESSAGE_INVALID:
            //            printf("ERROR_MESSAGE_INVALID\n");
            break;

        case ERROR_TIMER_OVERFLOW:
            //            printf("ERROR_TIMER_OVERFLOW\n");
            break;

        case ERROR_BAUDRATE:
            //            printf("ERROR_BAUDRATE\n");
            break;

        case ERROR_PARAMETER_INVALID:
            //            printf("ERROR_PARAMETER_INVALID\n");
            break;

        case ERROR_DEVICE:
            //            printf("ERROR_DEVICE\n");
            break;

        default:
            //            printf("ERROR: UNKNOWN ERROR CODE\n");
            break;
    }
}

void xsens_internal_handle_mdata2( packet_buffer_t *packet )
{
    // MData2 packets contain 1 to n smaller packets
    // with variable length fields, see xsens_mdata2.c/.h
    xsens_mdata2_process( packet, most_recent_interface->event_cb );
}
