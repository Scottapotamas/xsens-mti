#ifndef XSENS_MTI_H
#define XSENS_MTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xsens_constants.h"

// Packet state machine values
typedef enum
{
    PARSER_PREAMBLE = 0,
    PARSER_ADDRESS,
    PARSER_MESSAGE_ID,
    PARSER_LENGTH,
    PARSER_LENGTH_EXTENDED_B1,
    PARSER_LENGTH_EXTENDED_B2,
    PARSER_PAYLOAD,
    PARSER_CRC,
} xsens_parser_state_t;

// Userspace storage for library state and buffers
typedef struct
{
    xsens_parser_state_t state;
    xsens_packet_buffer_t packet;
    uint16_t        payload_pos;
    uint8_t         crc;

    callback_event_t    event_cb;
    callback_data_out_t output_cb;
} xsens_interface_t;

// Macros to make interface object instantiation cleaner
//   Needed because Arduino/C++ doesn't support out-of-order/incomplete 
#define XSENS_INTERFACE_RX( CB_PTR ) { .state = PARSER_PREAMBLE, .packet = XSENS_PACKET_BUF_EMPTY, .payload_pos = 0, .crc = 0, .event_cb = CB_PTR, .output_cb = 0 }
#define XSENS_INTERFACE_RX_TX( CB_PTR, OUTPUT_PTR ) { .state = PARSER_PREAMBLE, .packet = XSENS_PACKET_BUF_EMPTY, .payload_pos = 0, .crc = 0, .event_cb = CB_PTR, .output_cb = OUTPUT_PTR }

// Callback to internal (or userspace) handler function
// Function is passed a successfully receieved packet for processing
typedef void ( *callback_payload_t )( xsens_packet_buffer_t * );

// Entry for a list of function pointers to handle each MID correctly
typedef struct
{
    uint8_t             id;
    callback_payload_t  handler_fn;
} message_handler_ref_t;

// Parse inbound bytes received from the MTi device
void xsens_mti_parse_buffer( xsens_interface_t *interface, uint8_t *buffer, uint16_t size );
void xsens_mti_parse( xsens_interface_t *interface, uint8_t byte );
void xsens_mti_reset_parser( xsens_interface_t *interface );

// Add or override the internal packet handler function with a userspace function
bool xsens_mti_override_id_handler( uint8_t id, callback_payload_t *user_fn );

// Send a ready-made packet to hardware
void xsens_mti_send( xsens_interface_t *interface, xsens_packet_buffer_t *packet );

// Send a request message (no payload) to hardware
void xsens_mti_request( xsens_interface_t *interface, uint8_t id );


// Helper functions for configuring IMU settings
void xsens_mti_set_baudrate( xsens_interface_t *interface, XsensBaudSetting_t baudrate );
void xsens_mti_reset_orientation( xsens_interface_t *interface, XsensOrientationSetting_t code );

#ifdef __cplusplus
}
#endif

#endif    //end XSENS_MTI_H