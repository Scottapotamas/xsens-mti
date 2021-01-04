#include "xsens_utility.h"
#include "math.h"
#include "stdlib.h"

uint16_t coalesce_16BE_16LE( uint8_t *source )
{
    return ( source[1] << 0 | source[0] << 8 );
}

uint32_t coalesce_32BE_32LE( uint8_t *source )
{
    return ( ( source[3] << 0 )
             | ( source[2] << 8 )
             | ( source[1] << 16 )
             | ( source[0] << 24 ) );
}

float coalesce_32BE_F32LE( uint8_t *source )
{
    float    f;
    uint8_t *f_ptr = (uint8_t *)&f;

    f_ptr[3] = source[0];
    f_ptr[2] = source[1];
    f_ptr[1] = source[2];
    f_ptr[0] = source[3];

    return f;
}

void convert_quaternion_to_euler( float quaternion[4], float euler[3] )
{
    float w = quaternion[0];
    float x = quaternion[1];
    float y = quaternion[2];
    float z = quaternion[3];

    // Roll: x-axis
    double sinr_cosp = 2 * ( w * x + y * z );
    double cosr_cosp = 1 - 2 * ( x * x + y * y );
    euler[0]        = atan2( sinr_cosp, cosr_cosp );

    // Pitch: y-axis
    double sinp = 2 * ( w * y - z * x );
    if( abs( sinp ) >= 1 )
    {
        euler[1] = copysign( M_PI / 2, sinp );    // use 90 degrees if out of range
    }
    else
    {
        euler[1] = asin( sinp );
    }

    // Yaw: z-axis
    double siny_cosp = 2 * ( w * z + x * y );
    double cosy_cosp = 1 - 2 * ( y * y + z * z );
    euler[2]        = atan2( siny_cosp, cosy_cosp );
}