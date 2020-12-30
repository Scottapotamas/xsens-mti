#include "xsens_mti.h"
#include "xsens_mti_private.h"

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
    
    if( interface->state.state != PARSER_PREAMBLE )
    {
        
        interface->state.crc += (byte & 0xFF);
    }

    switch( interface->state.state )
    {
        case PARSER_PREAMBLE:
            if( byte == PREAMBLE_BYTE )
            {
                reset_parser( interface );
                interface->state.state = PARSER_ADDRESS;
            }
        break;

        case PARSER_ADDRESS:
            if( byte == ADDRESS_BYTE )
            {
                interface->state.state = PARSER_MESSAGE_ID;
            }
        break;

        case PARSER_MESSAGE_ID:
            interface->state.message_id = byte;
            interface->state.state = PARSER_LENGTH;
        break;

        case PARSER_LENGTH:
            if( byte == LENGTH_EXTENDED_MODE )
            {
                printf("Extended packet?\n");
                interface->state.state = PARSER_LENGTH_EXTENDED_B1;
            }
            if( byte == LENGTH_NONE )
            {
                interface->state.state = PARSER_CRC;
            }
            else
            {
                interface->state.length = byte;
                interface->state.state = PARSER_PAYLOAD;
            }
        break;

        case PARSER_LENGTH_EXTENDED_B1:
            interface->state.length = byte;
            // TODO decode long length
            interface->state.state = PARSER_LENGTH_EXTENDED_B2;
        break;

        case PARSER_LENGTH_EXTENDED_B2:
            // TODO decode long length
            interface->state.length &= byte << 8;
            interface->state.state = PARSER_PAYLOAD;
        break;

        case PARSER_PAYLOAD:
            interface->state.payload[interface->state.payload_pos] = byte;
            interface->state.payload_pos++;
            
            // Once we've buffered the whole payload,
            // prepare to read the CRC
            if( interface->state.payload_pos < interface->state.length )
            {
                interface->state.state = PARSER_CRC;
            }
        break;

        case PARSER_CRC:
            // Check if CRC is valid
            if( interface->state.crc == 00 )
            {
                // Packet was successfully recieved
                // Run the payload handling function
                handle_payload( interface );
            }
            else
            {
                printf("CRC Fail\n");
            }

            interface->state.state = PARSER_PREAMBLE;
        break;
    }
}

void reset_parser( interface_t *interface )
{
    // Reset the parser state and buffers
}

// With a valid packet, process the payload
void handle_payload( interface_t *interface )
{
    printf("Got a valid inbound packet\n");

    switch( interface->state.message_id )
    {
        case 0:

        break;
    }
}