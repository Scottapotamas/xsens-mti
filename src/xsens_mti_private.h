#ifndef XSENS_MTI_PRIVATE_H
#define XSENS_MTI_PRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xsens_mti.h"

// Library provided payload handler functions
void xsens_internal_handle_device_id( xsens_packet_buffer_t *packet );
void xsens_internal_handle_product_code( xsens_packet_buffer_t *packet );
void xsens_internal_handle_hardware_version( xsens_packet_buffer_t *packet );
void xsens_internal_handle_firmware_version( xsens_packet_buffer_t *packet );
void xsens_internal_handle_selftest_results( xsens_packet_buffer_t *packet );
void xsens_internal_handle_error( xsens_packet_buffer_t *packet );

void xsens_internal_handle_mdata2( xsens_packet_buffer_t *packet );

// Responsible for calling downstream handler and post-processing functions
// after a valid packet is parsed
void xsens_mti_handle_payload( xsens_interface_t *interface );

// Search the hardcoded message handler table for a given ID
// Returns a pointer to the entry if it exists
// Returns 0 if no match is found
message_handler_ref_t *xsens_mti_find_inbound_handler_entry( uint8_t find_id );

// Testable find-in-table function used by xsens_mti_find_inbound_handler_entry
message_handler_ref_t *xsens_mti_find_handler_entry( uint8_t find_id, message_handler_ref_t *entry_table, uint8_t entry_num );

// Run through a buffer of bytes and return the CRC
// Used when sending messages
uint8_t xsens_mti_buffer_crc( uint8_t *buffer, uint16_t size );


#ifdef __cplusplus
}
#endif

#endif    //end XSENS_MTI_PRIVATE_H