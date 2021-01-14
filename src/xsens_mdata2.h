#ifndef XSENS_MDATA2_H
#define XSENS_MDATA2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "xsens_constants.h"

typedef enum
{
    XDI_PARSE_ID_B1,
    XDI_PARSE_ID_B2,
    XDI_PARSE_LENGTH,
    XDI_PARSE_DATA
} mdata2_parser_state_t;

typedef struct
{
    uint16_t id;
    uint8_t  length;
    uint8_t  payload[255];    // TODO: work out what size is actually needed
} mdata2_packet_t;

void xsens_mdata2_process( xsens_packet_buffer_t *packet, callback_event_t evt_cb );

void xsens_mdata2_decode_field( mdata2_packet_t *output, callback_event_t evt_cb );

#ifdef __cplusplus
}
#endif

#endif    //end XSENS_MDATA2_H