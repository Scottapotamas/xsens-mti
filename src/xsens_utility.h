#ifndef XSENS_UTILITY_H
#define XSENS_UTILITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

// Helper for number of elements in array
#define XSENS_ARR_ELEM(arr) (sizeof(arr) / sizeof(*arr)) // Failed to compile? Ensure the type is an array!   

// Endianness conversion functions 
void xsens_swap_endian_u16( uint8_t *dest, uint8_t *source );

void xsens_swap_endian_u32( uint8_t *dest, uint8_t *source );

void xsens_swap_endian_u64( uint8_t *dest, uint8_t *source );

// Read 2 big-endian bytes and return a little-endian uint16
uint16_t xsens_coalesce_16BE_16LE( uint8_t *source );

// Read 4 big-endian bytes and return a little-endian uint32
uint32_t xsens_coalesce_32BE_32LE( uint8_t *source );

// Read 4 big-endian bytes and return a little-endian float
float xsens_coalesce_32BE_F32LE( uint8_t *source );

// Converts 6-byte big-endian 'fixed-point' Q16.32 packet format to little-endian
void xsens_coalesce_48BE_48LE( uint8_t *dest, uint8_t *source );

// Convert quaternion to roll/pitch/yaw angles (in radians)
void xsens_quaternion_to_euler( float *quaternion, float *euler );

// Convert roll/pitch/yaw euler angles (in radians) to quaternion
void xsens_euler_to_quaternion( float *euler, float *quaternion );

// Convert float32 to fixed-point 12.20
// 	12-bit integer with 20-bit fractional part
// 	Expresses values across range [-2048.0 to 2047.9999990]
//  Refer https://base.xsens.com/s/article/An-explanation-of-the-MTi-fixed-point-output-formats-1605869706093
int32_t xsens_f32_to_fp1220( float value );

// Convert fixed-point 12.20 to float32
float xsens_fp1220_to_f32( int32_t value );

// Convert float32 to fixed-point 16.32
// 	16-bit integer with 32-bit fractional part
// 	Expresses values across range [-32768.0 to 32767.9999999998]
int64_t xsens_f64_to_fp1632( double value );

// Convert fixed-point 16.32 to float64
double xsens_fp1632_to_f64( int64_t value );

#ifdef __cplusplus
}
#endif

#endif    //end XSENS_UTILITY_H