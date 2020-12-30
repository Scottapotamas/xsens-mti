#ifndef XENS_MTI_H
#define XENS_MTI_H

#ifdef __cplusplus
extern "C" {
#endif


#include "xsens_constants.h"




typedef enum  {
    EVT_BLAH = 0,
} EventFlags_t;




// Packet state machine values
typedef enum {
    PARSER_PREAMBLE = 0,
    PARSER_ADDRESS,
    PARSER_MESSAGE_ID,
    PARSER_LENGTH,
    PARSER_LENGTH_EXTENDED_B1,
    PARSER_LENGTH_EXTENDED_B2,
    PARSER_PAYLOAD,
    PARSER_CRC,
} parser_state_t;

// Userspace callback to notify application layer code of a relevant event
typedef void (*callback_event_t)(EventFlags_t);

// Callback to userspace serial write function
// Uses args for a uint8_t buffer of bytes, with a uint16_t size value
typedef void (*callback_data_out_t)(uint8_t*, uint16_t);

// Userspace storage for library state and buffers
typedef struct
{
    parser_state_t state;
    packet_buffer_t packet;
    uint16_t payload_pos;
    uint8_t crc;

    callback_event_t event_cb;
    callback_data_out_t output_cb;
} interface_t;


// Callback to internal (or userspace) handler function
// Function is passed a successfully receieved packet for processing
typedef void (*callback_payload_t)(packet_buffer_t*);

// Entry for a list of function pointers to handle each MID correctly
typedef struct
{
    uint8_t id;
    callback_payload_t *handler_fn;
} message_handler_ref_t;



// Parse inbound bytes received from the MTi device
void xsens_mti_parse_buffer( interface_t *interface, uint8_t *buffer, uint16_t size );
void xsens_mti_parse( interface_t *interface, uint8_t byte );
void xsens_mti_reset_parser( interface_t *interface );

// Add or override the internal packet handler function with a userspace function
bool xsens_mti_override_id_handler( uint8_t id, callback_payload_t *user_fn );




#ifdef __cplusplus
}
#endif

#endif //end XENS_MTI_H