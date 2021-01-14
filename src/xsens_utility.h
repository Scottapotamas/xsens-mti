#ifndef XSENS_UTILITY_H
#define XSENS_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

// Read 2 big-endian bytes and return a little-endian uint16
uint16_t xsens_coalesce_16BE_16LE( uint8_t *source );

// Read 4 big-endian bytes and return a little-endian uint32
uint32_t xsens_coalesce_32BE_32LE( uint8_t *source );

// Read 4 big-endian bytes and return a little-endian float
float xsens_coalesce_32BE_F32LE( uint8_t *source );

// Convert quaternion to roll/pitch/yaw angles (in radians)
void xsens_quaternion_to_euler( float *quaternion, float *euler );

#ifdef __cplusplus
}
#endif

#endif    //end XSENS_UTILITY_H