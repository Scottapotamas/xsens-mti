#include "xsens_mti.h"
#include "xsens_mti_private.h"

// Setup callbacks
void init()
{

}

parser_state_t parser_state = PARSER_PREAMBLE;
uint8_t message_id = 0x00;
uint16_t packet_length = 0;
uint8_t payload_buffer[2048] = { 0 };
uint16_t payload_buffer_pos = 0;
uint8_t running_crc = 0x00;

// Helper to parse inbound data with one call
void parse_buffer( uint8_t *buffer, uint16_t size )
{
    for( uint16_t i = 0; i < size; i++)
    {
        parse( buffer[i] );
    }
}

// Run each byte through the packet-level statemachine
void parse( uint8_t byte )
{
    if( parser_state != PARSER_PREAMBLE )
    {
        running_crc += (byte & 0xFF);
    }

    switch( parser_state )
    {
        case PARSER_PREAMBLE:
            if( byte == PREAMBLE_BYTE )
            {
                // TODO: Reset the parser states and buffers to 
                // TODO: prepare for new packet
                parser_state = PARSER_ADDRESS;
            }
        break;

        case PARSER_ADDRESS:
            if( byte == ADDRESS_BYTE )
            {
                parser_state = PARSER_MESSAGE_ID;
            }
        break;

        case PARSER_MESSAGE_ID:
            message_id = byte;
            parser_state = PARSER_LENGTH;
        break;

        case PARSER_LENGTH:
            if( byte == LENGTH_EXTENDED_MODE )
            {
                printf("Extended packet?\n");
                parser_state = PARSER_LENGTH_EXTENDED_B1;
            }
            if( byte == LENGTH_NONE )
            {
                parser_state = PARSER_CRC;
            }
            else
            {
                packet_length = byte;
                parser_state = PARSER_PAYLOAD;
            }
        break;

        case PARSER_LENGTH_EXTENDED_B1:
            packet_length = byte;
            // TODO decode long length
            parser_state = PARSER_LENGTH_EXTENDED_B2;
        break;

        case PARSER_LENGTH_EXTENDED_B2:
            // TODO decode long length
            packet_length &= byte << 8;
            parser_state = PARSER_PAYLOAD;
        break;

        case PARSER_PAYLOAD:
            payload_buffer[payload_buffer_pos] = byte;
            payload_buffer_pos++;
            
            // Once we've buffered the whole payload,
            // prepare to read the CRC
            if( payload_buffer_pos < packet_length )
            {
                parser_state = PARSER_CRC;
            }
        break;

        case PARSER_CRC:
            // Check if CRC is valid
            if( running_crc == 00 )
            {
                // Packet was successfully recieved
                // Run the payload handling function
                handle_payload();
            }
            else
            {
                printf("CRC Fail\n");
            }

            parser_state = PARSER_PREAMBLE;
        break;
    }
}

void reset_parser( void )
{

}

// With a valid packet, process the payload
void handle_payload()
{
    printf("Got a valid inbound packet\n");

    switch( message_id )
    {
        case 0:

        break;
    }
}