#include "string.h"

#include "xsens_mdata2.h"
#include "xsens_mti.h"
#include "xsens_mti_private.h"
#include "xsens_utility.h"

xsens_interface_t *most_recent_interface;

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
    { .id = MT_ERROR, .handler_fn = &xsens_internal_handle_error },
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
void xsens_mti_parse_buffer( xsens_interface_t *interface, uint8_t *buffer, uint16_t size )
{
    for( uint16_t i = 0; i < size; i++ )
    {
        xsens_mti_parse( interface, buffer[i] );
    }
}

// Run each byte through the packet-level statemachine
void xsens_mti_parse( xsens_interface_t *interface, uint8_t byte )
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
            // TODO decode long length packets
            interface->state = PARSER_LENGTH_EXTENDED_B2;
            break;

        case PARSER_LENGTH_EXTENDED_B2:
            // TODO decode long length packets
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
                // TODO send CRC failed event to user?
            }

            interface->state = PARSER_PREAMBLE;
            break;
    }
}

void xsens_mti_reset_parser( xsens_interface_t *interface )
{
    // Clear the parser state and buffers
    memset( &( interface->packet ), 0, sizeof( xsens_packet_buffer_t ) );
    interface->payload_pos = 0;
    interface->crc         = 0;
}

// With a valid packet, process the payload
void xsens_mti_handle_payload( xsens_interface_t *interface )
{
    xsens_packet_buffer_t *packet = &( interface->packet );

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

    return xsens_mti_find_handler_entry(find_id, inbound_handler_table, table_length );
}

message_handler_ref_t *xsens_mti_find_handler_entry( uint8_t find_id, message_handler_ref_t *entry_table, uint8_t entry_count )
{
    for( uint8_t i = 0; i < entry_count; i++ )
    {
        if( entry_table[i].id == find_id )
        {
            return &entry_table[i];
        }
    }

    return (message_handler_ref_t *)NULL;
}

uint8_t xsens_mti_buffer_crc( uint8_t *buffer, uint16_t size )
{
    uint8_t crc = 0;

    for( uint16_t i = 0; i < size; i++ )
    {
        crc -= buffer[i];
    }

    return crc;
}

void xsens_mti_send( xsens_interface_t *interface, xsens_packet_buffer_t *packet )
{
    if( interface && packet )
    {
        uint8_t buffer[2048] = { 0 };
        uint16_t buffer_pos = 0;
        uint8_t crc = 0;

        // Preamble
        buffer[buffer_pos++] = PREAMBLE_BYTE;

        // Device Address
        buffer[buffer_pos++] = ADDRESS_BYTE;

        // Message ID
        buffer[buffer_pos++] = packet->message_id;

        // Payload Length
        if( packet->length < 0xFF )
        {
            buffer[buffer_pos++] = packet->length;
        }
        else
        {
            // Extended packet handling sets the normal length byte to 255,
            // followed by two bytes of payload data
            buffer[buffer_pos++] = 0xFF;

            memcpy( &buffer[buffer_pos], &packet->length, 2 );
            buffer_pos += 2;
        }

        // Payload Data
        if( packet->length != 0 )
        {
            memcpy( &buffer[buffer_pos], (uint8_t *)packet->payload, packet->length );
            buffer_pos += packet->length;
        }

        // Calculate the CRC of the packet, exluding the preamble
        buffer[buffer_pos] = xsens_mti_buffer_crc( &buffer[1], buffer_pos-1 );
        buffer_pos += 1;

        // Pass the buffer to the user so they can send to hardware
        if( interface->output_cb )
        {
            interface->output_cb( buffer, buffer_pos );
        }
    }
}

void xsens_mti_request( xsens_interface_t *interface, uint8_t id )
{
    xsens_packet_buffer_t packet = { 0 };

    packet.message_id = id;
    packet.length = 0;
    packet.payload[0] = 0;

    xsens_mti_send( interface, &packet );
}



void xsens_mti_set_baudrate( xsens_interface_t *interface, XsensBaudSetting_t baudrate )
{
    xsens_packet_buffer_t packet = { 0 };
    
    packet.message_id = MT_SETBAUDRATE;
    packet.length = 1;
    packet.payload[0] = baudrate;

    xsens_mti_send( interface, &packet );
}


void xsens_mti_reset_orientation( xsens_interface_t *interface, XsensOrientationSetting_t code )
{
    xsens_packet_buffer_t packet = { 0 };
    
    packet.message_id = MT_RESETORIENTATION;
    packet.length = 2;
    packet.payload[0] = 0x00;
    packet.payload[1] = code;

    xsens_mti_send( interface, &packet );
}






void xsens_internal_handle_device_id( xsens_packet_buffer_t *packet )
{
    XsensEventData_t value = { 0 };

    if( packet->length == 4 )    // MTi 1, 10, 100
    {
        value.type    = XSENS_EVT_TYPE_U32;
        value.data.u4 = xsens_coalesce_32BE_32LE( &packet->payload[0] );
    }
    else if( packet->length == 8 )    // MTi-600
    {
        // TODO: untested 8-byte device ID
        value.type    = XSENS_EVT_TYPE_U32;
        value.data.u4 = xsens_coalesce_32BE_32LE( &packet->payload[4] );
    }

    if( most_recent_interface->event_cb )
    {
        most_recent_interface->event_cb( XSENS_EVT_DEVICE_ID, &value );
    }
}

void xsens_internal_handle_product_code( xsens_packet_buffer_t *packet )
{
    // ASCII formatted code max 20 bytes
    // TODO: handle product code
}

void xsens_internal_handle_hardware_version( xsens_packet_buffer_t *packet )
{
    // TODO: handle product code

    //    uint8_t hw_version[2];
    //    uint16_t *hw_ptr = (uint16_t *)&hw_version;
    //    hw_ptr           = xsens_coalesce_16BE_16LE( &packet->payload[0] );
}

void xsens_internal_handle_firmware_version( xsens_packet_buffer_t *packet )
{
    // TODO: handle firmware version

    uint8_t  major    = packet->payload[0];
    uint8_t  minor    = packet->payload[1];
    uint8_t  revision = packet->payload[2];
    uint32_t build    = xsens_coalesce_32BE_32LE( &packet->payload[3] );
    uint32_t scm      = xsens_coalesce_32BE_32LE( &packet->payload[7] );
}

void xsens_internal_handle_selftest_results( xsens_packet_buffer_t *packet )
{
    // TODO: handle selftest results
}

void xsens_internal_handle_error( xsens_packet_buffer_t *packet )
{
    uint8_t error_code = packet->payload[0];

    /*
    switch( error_code )
    {
        case ERROR_PERIOD_INVALID:
            break;

        case ERROR_MESSAGE_INVALID:
            break;

        case ERROR_TIMER_OVERFLOW:
            break;

        case ERROR_BAUDRATE:
            break;

        case ERROR_PARAMETER_INVALID:
            break;

        case ERROR_DEVICE:
            break;

        default:
            break;
    }
     */
}

void xsens_internal_handle_mdata2( xsens_packet_buffer_t *packet )
{
    // MData2 packets contain 1 to n smaller packets
    // with variable length fields, see xsens_mdata2.c/.h
    xsens_mdata2_process( packet, most_recent_interface->event_cb );
}
