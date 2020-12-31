#include "xsens_utility.h"

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