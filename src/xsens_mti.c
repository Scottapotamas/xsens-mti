#include "string.h"

#include "xsens_mti.h"
#include "xsens_mti_private.h"


message_handler_ref_t inbound_handler_table[] =
{
    { .id = WAKEUP, .handler_fn = NULL },
    { .id = GOTOCONFIGACK, .handler_fn = NULL },
    { .id = GOTOMEASUREMENTACK, .handler_fn = NULL },
    { .id = RESETACK, .handler_fn = NULL },
    { .id = DEVICEID, .handler_fn = &handle_device_id },
    { .id = PRODUCTCODE, .handler_fn = &handle_product_code },
    { .id = HARDWAREVERSION, .handler_fn = &handle_hardware_version }, 
    { .id = FIRMWAREREV, .handler_fn = &handle_firmware_version }, 
    { .id = SELFTESTRESULTS, .handler_fn = &handle_selftest_results }, 
    { .id = ERROR, .handler_fn = &handle_error }, 
    { .id = WARNING, .handler_fn = &handle_warning }, 
    { .id = CONFIGURATION, .handler_fn = NULL }, 
    { .id = MTDATA2, .handler_fn = &handle_mdata2 }, 
    { .id = REQLATLONALTACK, .handler_fn = NULL }, 
    { .id = SETLATLONALTACK, .handler_fn = NULL }, 
    { .id = REQFILTERPROFILEACK, .handler_fn = NULL }, 
    { .id = REQGNSSPLATFORMACK, .handler_fn = NULL }, 
    { .id = REQGNSSLEVERARMACK, .handler_fn = NULL }, 
    { .id = SETGNSSLEVERARMACK, .handler_fn = NULL }, 
    { .id = UTCTIME, .handler_fn = NULL }, 
    { .id = ICCCOMMANDACK, .handler_fn = NULL }, 
    { .id = SETINITIALHEADINGACK, .handler_fn = NULL }, 
    { .id = FORWARDGNSSDATAACK, .handler_fn = NULL }, 
};


// Setup callbacks
void init( interface_t *interface )
{

}


// Helper to parse inbound data with one call
void parse_buffer( interface_t *interface, uint8_t *buffer, uint16_t size )
{
    for( uint16_t i = 0; i < size; i++)
    {
        parse( interface, buffer[i] );
    }
}

// Run each byte through the packet-level statemachine
void parse( interface_t *interface, uint8_t byte )
{
    // CRC is the sum of bytes including the CRC byte (ex PREAMBLE)
    if( interface->state != PARSER_PREAMBLE )
    {
        interface->crc += (byte & 0xFF);
    }

    switch( interface->state )
    {
        case PARSER_PREAMBLE:
            if( byte == PREAMBLE_BYTE )
            {
                reset_parser( interface );
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
            interface->state = PARSER_LENGTH;
        break;

        case PARSER_LENGTH:
            if( byte == LENGTH_EXTENDED_MODE )
            {
                printf("Extended packet?\n");
                interface->state = PARSER_LENGTH_EXTENDED_B1;
            }
            if( byte == LENGTH_NONE )
            {
                interface->packet.length = 0;
                interface->state = PARSER_CRC;
            }
            else
            {
                interface->packet.length = byte;
                interface->state = PARSER_PAYLOAD;
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
                handle_payload( interface );
            }
            else
            {
                printf("CRC Fail: 0x%x ID: 0x%X?\n", interface->crc, interface->packet.message_id);
            }

            interface->state = PARSER_PREAMBLE;
        break;
    }
}

void reset_parser( interface_t *interface )
{
    // Clear the parser state and buffers
    memset( &(interface->packet), 0, sizeof(packet_buffer_t) );
    interface->payload_pos = 0;
    interface->crc = 0;
}




// With a valid packet, process the payload
void handle_payload( interface_t *interface )
{
    packet_buffer_t *packet = &(interface->packet);

    // Search the inbound handler table for a match
    message_handler_ref_t *handler = find_inbound_handler_table_entry( packet->message_id );

    // If the ID is recognised, call the handler function (if it exists)
    if( handler )
    {
        callback_payload_t payload_handler_fn = handler->handler_fn;
        if( payload_handler_fn )
        {
            payload_handler_fn( packet );
        }
    }

}

bool override_id_handler( uint8_t id, callback_payload_t *user_fn )
{
    if( user_fn )
    {
        // Find the ID in the inbound handler 'jump table'
        message_handler_ref_t *handler = find_inbound_handler_table_entry( id );
        
        if( handler )
        {
            handler->handler_fn = user_fn;
        }
    }
}

message_handler_ref_t * find_inbound_handler_table_entry( uint8_t find_id )
{
    
    uint8_t table_length = sizeof(inbound_handler_table) / sizeof(message_handler_ref_t);

    for( uint8_t i = 0; i < table_length; i++ )
    {
        if( inbound_handler_table[i].id == find_id )
        {
            return &inbound_handler_table[i];
        }
    }

    return (message_handler_ref_t*)NULL;
}













void handle_device_id( packet_buffer_t *packet )
{
    printf("DeviceID Handler\n");
}

void handle_product_code( packet_buffer_t *packet )
{
    printf("ProductCode Handler\n");

}

void handle_hardware_version( packet_buffer_t *packet )
{
    printf("HardwareVersion Handler\n");

}

void handle_firmware_version( packet_buffer_t *packet )
{
    printf("FirmwareVersion Handler\n");

}

void handle_selftest_results( packet_buffer_t *packet )
{
    printf("SelfTest Handler\n");
}

void handle_error( packet_buffer_t *packet )
{
    printf("Error Handler\n");

}

void handle_warning( packet_buffer_t *packet )
{
    printf("Warning Handler\n");

}

        break;
    }
}