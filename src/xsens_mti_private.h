#ifndef XSENS_MTI_PRIVATE_H
#define XSENS_MTI_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xsens_mti.h"

// Library provided payload handler functions
void xsens_internal_handle_device_id( packet_buffer_t *packet );
void xsens_internal_handle_product_code( packet_buffer_t *packet );
void xsens_internal_handle_hardware_version( packet_buffer_t *packet );
void xsens_internal_handle_firmware_version( packet_buffer_t *packet );
void xsens_internal_handle_selftest_results( packet_buffer_t *packet );
void xsens_internal_handle_error( packet_buffer_t *packet );

void xsens_internal_handle_mdata2( packet_buffer_t *packet );

// Responsible for calling downstream handler and post-processing functions
// after a valid packet is parsed
void xsens_mti_handle_payload( interface_t *interface );

// Search the message handler table for a given ID
// Returns a pointer to the entry if it exists
// Returns 0 if no match is found
message_handler_ref_t *xsens_mti_find_inbound_handler_entry( uint8_t find_id );

#ifdef __cplusplus
}
#endif

#endif    //end XSENS_MTI_PRIVATE_H