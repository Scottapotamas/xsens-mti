#ifndef XENS_UTILITY_H
#define XENS_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

// Read 2 big-endian bytes and return a little-endian uint16
uint16_t coalesce_16BE_16LE( uint8_t *source );

// Read 4 big-endian bytes and return a little-endian uint32
uint32_t coalesce_32BE_32LE( uint8_t *source );

// Read 4 big-endian bytes and return a little-endian float
float coalesce_32BE_F32LE( uint8_t *source );

#ifdef __cplusplus
}
#endif

#endif //end XENS_UTILITY_H