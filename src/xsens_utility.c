#include "xsens_utility.h"
#include "math.h"
#include "stdlib.h"

void xsens_swap_endian_u16( uint8_t *dest, uint8_t *source )
{
    dest[1] = source[0];
    dest[0] = source[1];
}

void xsens_swap_endian_u32( uint8_t *dest, uint8_t *source )
{
    dest[3] = source[0];
    dest[2] = source[1];
    dest[1] = source[2];
    dest[0] = source[3];
}

void xsens_swap_endian_u64( uint8_t *dest, uint8_t *source )
{
    dest[7] = source[0];
    dest[6] = source[1];
    dest[5] = source[2];
    dest[4] = source[3];
    dest[3] = source[4];
    dest[2] = source[5];
    dest[1] = source[6];
    dest[0] = source[7];
}

uint16_t xsens_coalesce_16BE_16LE( uint8_t *source )
{
    return ( source[1] << 0 | source[0] << 8 );
}

uint32_t xsens_coalesce_32BE_32LE( uint8_t *source )
{
    return ( ( source[3] << 0 )
             | ( source[2] << 8 )
             | ( source[1] << 16 )
             | ( source[0] << 24 ) );
}

float xsens_coalesce_32BE_F32LE( uint8_t *source )
{
    float    f;
    uint8_t *f_ptr = (uint8_t *)&f;

    f_ptr[3] = source[0];
    f_ptr[2] = source[1];
    f_ptr[1] = source[2];
    f_ptr[0] = source[3];

    return f;
}

// As per manual, big-endian 32-bit first, then BE 16-bit part i.e [b3, b2, b1, b0, b5, b4]
void xsens_coalesce_48BE_48LE( uint8_t *dest, uint8_t *source )
{
    dest[0] = source[3];
    dest[1] = source[2];
    dest[2] = source[1];
    dest[3] = source[0];
    dest[4] = source[5];
    dest[5] = source[4];
}

void xsens_quaternion_to_euler( float *quaternion, float *euler )
{
    float w = quaternion[0];
    float x = quaternion[1];
    float y = quaternion[2];
    float z = quaternion[3];

    // Roll: x-axis
    float sinr_cosp = 2 * ( w * x + y * z );
    float cosr_cosp = 1 - 2 * ( x * x + y * y );
    euler[0]        = (float)atan2( sinr_cosp, cosr_cosp );

    // Pitch: y-axis
    float sinp = 2 * ( w * y - z * x );
    if( fabs( sinp ) >= 1 )
    {
        euler[1] = (float)copysign( M_PI / 2, sinp );    // use 90 degrees if out of range
    }
    else
    {
        euler[1] = (float)asin( sinp );
    }

    // Yaw: z-axis
    float siny_cosp = 2 * ( w * z + x * y );
    float cosy_cosp = 1 - 2 * ( y * y + z * z );
    euler[2]        = (float)atan2( siny_cosp, cosy_cosp );
}

void xsens_euler_to_quaternion( float *euler, float *quaternion )
{
    float roll  = euler[0];
    float pitch = euler[1];
    float yaw   = euler[2];

    float cy = cos( yaw   * 0.5f );
    float sy = sin( yaw   * 0.5f );
    float cp = cos( pitch * 0.5f );
    float sp = sin( pitch * 0.5f );
    float cr = cos( roll  * 0.5f );
    float sr = sin( roll  * 0.5f );

    quaternion[0] = cr * cp * cy + sr * sp * sy;
    quaternion[1] = sr * cp * cy - cr * sp * sy;
    quaternion[2] = cr * sp * cy + sr * cp * sy;
    quaternion[3] = cr * cp * sy - sr * sp * cy;
}

int32_t xsens_f32_to_fp1220( float value )
{
    return round(value * (1UL<<20));
}

float xsens_fp1220_to_f32( int32_t value )
{
    return (float)value / (1UL<<20);
}

int64_t xsens_f64_to_fp1632( double value )
{
    return (int64_t)(value * (1ULL<<32))&0x0000FFFFFFFFFFFF;
}

double xsens_fp1632_to_f64( int64_t value )
{   
    // Is the sign-bit set?
    if( value&0x0000800000000000 )
    {
        // Set the unused bytes, the division will handle -ve
        value |= 0xFFFF000000000000;
    }

    return (double)(value) / (1ULL<<32);
}
