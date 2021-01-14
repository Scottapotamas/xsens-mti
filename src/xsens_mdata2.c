#include <string.h>

#include "xsens_mdata2.h"
#include "xsens_utility.h"

// MData2 packets have a series of smaller structures of data
// This handler walks through the buffer, identifies the XDA type from two bytes
// Then applies relevant conversions back into native types/structures as necessary
// Packets don't have a fixed number of child elements
void xsens_mdata2_process( xsens_packet_buffer_t *packet, callback_event_t evt_cb )
{
    mdata2_parser_state_t md2_state      = XDI_PARSE_ID_B1;
    mdata2_packet_t       output         = { 0 };
    uint8_t               bytes_consumed = 0;

    // Walk through the packet and run a tiny statemachine
    // to parse the sub-fields
    for( uint16_t i = 0; i < packet->length; i++ )
    {
        switch( md2_state )
        {
            case XDI_PARSE_ID_B1:
                // High byte
                output.id = ( uint16_t )( (uint16_t)packet->payload[i] << 8u );
                md2_state = XDI_PARSE_ID_B2;
                break;

            case XDI_PARSE_ID_B2:
                // Low byte
                output.id |= ( uint16_t )( (uint16_t)packet->payload[i] );
                md2_state = XDI_PARSE_LENGTH;
                break;

            case XDI_PARSE_LENGTH:
                // Length is one byte
                output.length = packet->payload[i];
                md2_state     = XDI_PARSE_DATA;
                break;

            case XDI_PARSE_DATA:
                // Copy data across
                output.payload[bytes_consumed] = packet->payload[i];
                bytes_consumed++;

                // Once the field's data been copied to our sub-buffer,
                // handle it
                if( bytes_consumed >= output.length )
                {
                    // Using the isolated field, search for matching XID
                    // and then convert payloads to LE & structured data
                    xsens_mdata2_decode_field( &output, evt_cb );

                    // Cleanup our state before parsing remaining fields
                    md2_state      = XDI_PARSE_ID_B1;
                    bytes_consumed = 0;
                    memset( &output, 0, sizeof( mdata2_packet_t ) );
                }
                break;

            default:
                // Case switch should be an exhaustive match?
                // Reset state?
                md2_state      = XDI_PARSE_ID_B1;
                bytes_consumed = 0;
                memset( &output, 0, sizeof( mdata2_packet_t ) );
                break;
        }
    }

    // Finished MData2 parsing in payload
}

typedef struct
{
    uint16_t xid;
    uint8_t  event;
    uint8_t  type;
} mdata2_decode_rules_t;

// Entries with XSENS_EVT_TYPE_NONE aren't supported directly by the library
// User is just given the payload and they can opt to handle it themselves...
//
//    XDI_GNSS_PVT_DATA - not supported, even passing directly to dev
//    XDI_GNSS_SAT_INFO - ^
static const mdata2_decode_rules_t xid_decode_table[] = {
    { XDI_TEMPERATURE, XSENS_EVT_TEMPERATURE, XSENS_EVT_TYPE_FLOAT },
    { XDI_UTC_TIME, XSENS_EVT_UTC_TIME, XSENS_EVT_TYPE_NONE },
    { XDI_PACKET_COUNTER, XSENS_EVT_PACKET_COUNT, XSENS_EVT_TYPE_U16 },
    { XDI_SAMPLE_TIME_FINE, XSENS_EVT_TIME_FINE, XSENS_EVT_TYPE_U32 },
    { XDI_SAMPLE_TIME_COARSE, XSENS_EVT_TIME_COARSE, XSENS_EVT_TYPE_U32 },
    { XDI_QUATERNION, XSENS_EVT_QUATERNION, XSENS_EVT_TYPE_FLOAT4 },
    { XDI_EULER_ANGLES, XSENS_EVT_EULER, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_ROTATION_MATRIX, XSENS_EVT_ROT_MATRIX, XSENS_EVT_TYPE_FLOAT9 },
    { XDI_BARO_PRESSURE, XSENS_EVT_PRESSURE, XSENS_EVT_TYPE_U32 },
    { XDI_DELTA_V, XSENS_EVT_DELTA_V, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_DELTA_Q, XSENS_EVT_DELTA_Q, XSENS_EVT_TYPE_FLOAT4 },
    { XDI_ACCELERATION, XSENS_EVT_ACCELERATION, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_FREE_ACCELERATION, XSENS_EVT_FREE_ACCELERATION, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_ACCELERATION_HIGH_RATE, XSENS_EVT_ACCELERATION_HR, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_RATE_OF_TURN, XSENS_EVT_RATE_OF_TURN, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_RATE_OF_TURN_HIGH_RATE, XSENS_EVT_RATE_OF_TURN_HR, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_GNSS_PVT_PULSE, XSENS_EVT_GNSS_PVT_PULSE, XSENS_EVT_TYPE_U32 },
    { XDI_RAW_ACC_GYRO_MAG_TEMP, XSENS_EVT_RAW_ACC_GYRO_MAG_TEMP, XSENS_EVT_TYPE_NONE },
    { XDI_RAW_GYRO_TEMP, XSENS_EVT_RAW_GYRO_TEMP, XSENS_EVT_TYPE_NONE },
    { XDI_MAGNETIC_FIELD, XSENS_EVT_MAGNETIC, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_STATUS_BYTE, XSENS_EVT_STATUS_BYTE, XSENS_EVT_TYPE_U8 },
    { XDI_STATUS_WORD, XSENS_EVT_STATUS_WORD, XSENS_EVT_TYPE_U32 },
    { XDI_DEVICE_ID, XSENS_EVT_DEVICE_ID, XSENS_EVT_TYPE_U32 },
    { XDI_LOCATION_ID, XSENS_EVT_LOCATION_ID, XSENS_EVT_TYPE_U16 },
    { XDI_POSITION_ECEF, XSENS_EVT_POSITION_ECEF, XSENS_EVT_TYPE_FLOAT3 },
    { XDI_LAT_LON, XSENS_EVT_LAT_LON, XSENS_EVT_TYPE_FLOAT2 },
    { XDI_ALTITUDE_ELLIPSOID, XSENS_EVT_ALTITUDE_ELLIPSOID, XSENS_EVT_TYPE_FLOAT },
    { XDI_VELOCITY_XYZ, XSENS_EVT_VELOCITY_XYZ, XSENS_EVT_TYPE_FLOAT3 },
};

// With the 'isolated' field from the rest of the payload,
// convert to LE and pass to user cb in a union
void xsens_mdata2_decode_field( mdata2_packet_t *output, callback_event_t evt_cb )
{
    XsensEventData_t             value       = { 0 };
    const mdata2_decode_rules_t *decode_rule = 0;

    // Find the matching XID in the table
    uint8_t table_length = sizeof( xid_decode_table ) / sizeof( mdata2_decode_rules_t );
    for( uint8_t i = 0; i < table_length; i++ )
    {
        if( xid_decode_table[i].xid == output->id )
        {
            decode_rule = &xid_decode_table[i];
            break;
        }
    }

    // Apply post-processing (BE->LE) strategy specific to the packet type
    if( decode_rule )
    {
        // The structure describes the union type
        value.type = decode_rule->type;

        // Convert BE data to LE, put it in the right union field
        switch( decode_rule->type )
        {
            case XSENS_EVT_TYPE_U8:
                value.data.u1 = output->payload[0];
                break;

            case XSENS_EVT_TYPE_U16:
                value.data.u2 = xsens_coalesce_16BE_16LE( &output->payload[0] );
                break;

            case XSENS_EVT_TYPE_U32:
                value.data.u4 = xsens_coalesce_32BE_32LE( &output->payload[0] );
                break;

            case XSENS_EVT_TYPE_FLOAT:
                value.data.f4 = xsens_coalesce_32BE_F32LE( &output->payload[0] );
                break;

            case XSENS_EVT_TYPE_FLOAT2:
                value.data.f4x2[0] = xsens_coalesce_32BE_F32LE( &output->payload[0] );
                value.data.f4x2[1] = xsens_coalesce_32BE_F32LE( &output->payload[4] );
                break;

            case XSENS_EVT_TYPE_FLOAT3:
                value.data.f4x3[0] = xsens_coalesce_32BE_F32LE( &output->payload[0] );
                value.data.f4x3[1] = xsens_coalesce_32BE_F32LE( &output->payload[4] );
                value.data.f4x3[2] = xsens_coalesce_32BE_F32LE( &output->payload[8] );
                break;

            case XSENS_EVT_TYPE_FLOAT4:
                value.data.f4x4[0] = xsens_coalesce_32BE_F32LE( &output->payload[0] );
                value.data.f4x4[1] = xsens_coalesce_32BE_F32LE( &output->payload[4] );
                value.data.f4x4[2] = xsens_coalesce_32BE_F32LE( &output->payload[8] );
                value.data.f4x4[3] = xsens_coalesce_32BE_F32LE( &output->payload[12] );
                break;

            case XSENS_EVT_TYPE_FLOAT9:
                value.data.f4x9[0] = xsens_coalesce_32BE_F32LE( &output->payload[0] );
                value.data.f4x9[1] = xsens_coalesce_32BE_F32LE( &output->payload[4] );
                value.data.f4x9[2] = xsens_coalesce_32BE_F32LE( &output->payload[8] );
                value.data.f4x9[3] = xsens_coalesce_32BE_F32LE( &output->payload[12] );
                value.data.f4x9[4] = xsens_coalesce_32BE_F32LE( &output->payload[16] );
                value.data.f4x9[5] = xsens_coalesce_32BE_F32LE( &output->payload[20] );
                value.data.f4x9[6] = xsens_coalesce_32BE_F32LE( &output->payload[24] );
                value.data.f4x9[7] = xsens_coalesce_32BE_F32LE( &output->payload[28] );
                value.data.f4x9[8] = xsens_coalesce_32BE_F32LE( &output->payload[32] );
                break;

            default:
                // There's an error or not supported, return a 'null' type?
                value.type = XSENS_EVT_TYPE_NONE;
                break;
        }

        // Call the user-callback with the transformed data
        if( evt_cb )
        {
            evt_cb( decode_rule->event, &value );
        }
    }
}