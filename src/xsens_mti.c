#include "stdio.h"

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
    printf("Got a valid inbound. ID: 0x%X, %d bytes\n", packet->message_id, packet->length);

    switch( packet->message_id )
    {
        case 0:

        break;
    }
}