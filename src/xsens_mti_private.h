#ifndef XENS_MTI_PRIVATE_H
#define XENS_MTI_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xsens_mti.h"

// Library provided payload handler functions
void handle_device_id( packet_buffer_t *packet );
void handle_product_code( packet_buffer_t *packet);
void handle_hardware_version( packet_buffer_t *packet );
void handle_firmware_version( packet_buffer_t *packet );
void handle_selftest_results( packet_buffer_t *packet );
void handle_error( packet_buffer_t *packet );
void handle_warning( packet_buffer_t *packet );
void handle_mdata2( packet_buffer_t *packet );




// Responsible for calling downstream handler and post-processing functions
// after a valid packet is parsed
void handle_payload( interface_t *interface );

// Search the message handler table for a given ID
// Returns a pointer to the entry if it exists
// Returns 0 if no match is found
message_handler_ref_t * find_inbound_handler_table_entry( uint8_t find_id );


#ifdef __cplusplus
}
#endif

#endif //end XENS_MTI_PRIVATE_H