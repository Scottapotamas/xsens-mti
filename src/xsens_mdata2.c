#include "xsens_mdata2.h"
#include "xsens_utility.h"

// MData2 packets have a series of smaller structures of data
// This handler walks through the buffer, identifies the XDA type from two bytes
// Then applies relevant conversions back into native types/structures as necessary
// Packets don't have a fixed number of child elements
void xsens_mdata2_process( packet_buffer_t *packet, callback_event_t evt_cb )
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
                // handle that
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
        }
    }

    // Finished MData2 parsing in payload
}

void xsens_mdata2_decode_field( mdata2_packet_t *output, callback_event_t evt_cb )
{
    EventData_t value = { 0 };

    // With the 'isolated' field from the rest of the payload,
    // find the matching XID and apply post-processing strategies
    switch( output->id )
    {
        case XDI_UTC_TIME:
            printf( "UTC Time with %d bytes\n", output->length );
            break;

        case XDI_PACKET_COUNTER:
            value.type    = XSENS_EVT_TYPE_U16;
            value.data.u2 = coalesce_16BE_16LE( &output->payload[0] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_PACKET_COUNT, &value );
            }
            break;

        case XDI_SAMPLE_TIME_FINE:
            value.type    = XSENS_EVT_TYPE_U32;
            value.data.u4 = coalesce_32BE_32LE( &output->payload[0] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_TIME_FINE, &value );
            }
            break;

        case XDI_SAMPLE_TIME_COARSE:
            value.type    = XSENS_EVT_TYPE_U32;
            value.data.u4 = coalesce_32BE_32LE( &output->payload[0] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_TIME_COARSE, &value );
            }
            break;

        case XDI_TEMPERATURE:
            value.type    = XSENS_EVT_TYPE_FLOAT;
            value.data.f4 = coalesce_32BE_F32LE( &output->payload[0] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_TEMPERATURE, &value );
            }
            break;

        case XDI_QUATERNION:
            value.type         = XSENS_EVT_TYPE_FLOAT4;
            value.data.f4x4[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x4[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x4[2] = coalesce_32BE_F32LE( &output->payload[8] );
            value.data.f4x4[3] = coalesce_32BE_F32LE( &output->payload[12] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_QUATERNION, &value );
            }
            break;

        case XDI_ROTATION_MATRIX:
            printf( "RotationMatrix with %d bytes\n", output->length );
            break;

        case XDI_EULER_ANGLES:
            printf( "EulerAngles with %d bytes\n", output->length );
            break;

        case XDI_BARO_PRESSURE:
            value.type    = XSENS_EVT_TYPE_U32;
            value.data.u4 = coalesce_32BE_32LE( &output->payload[0] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_PRESSURE, &value );
            }
            break;

        case XDI_DELTA_V:
            value.type         = XSENS_EVT_TYPE_FLOAT3;
            value.data.f4x3[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x3[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x3[2] = coalesce_32BE_F32LE( &output->payload[8] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_DELTA_V, &value );
            }
            break;

        case XDI_ACCELERATION:
            value.type         = XSENS_EVT_TYPE_FLOAT3;
            value.data.f4x3[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x3[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x3[2] = coalesce_32BE_F32LE( &output->payload[8] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_ACCELERATION, &value );
            }
            break;

        case XDI_FREE_ACCELERATION:
            value.type         = XSENS_EVT_TYPE_FLOAT3;
            value.data.f4x3[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x3[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x3[2] = coalesce_32BE_F32LE( &output->payload[8] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_FREE_ACCELERATION, &value );
            }
            break;

        case XDI_ACCELERATION_HIGH_RATE:
            printf( "AccelerationHR with %d bytes\n", output->length );
            break;

        case XDI_ALTITUDE_ELLIPSOID:
            printf( "AltitudeEllipsoid with %d bytes\n", output->length );
            break;

        case XDI_POSITION_ECEF:
            printf( "PositionECEF with %d bytes\n", output->length );
            break;

        case XDI_LAT_LON:
            printf( "LAT_LON with %d bytes\n", output->length );
            break;

        case XDI_GNSS_PVT_DATA:
            printf( "PVT_DATA with %d bytes\n", output->length );
            break;

        case XDI_GNSS_SAT_INFO:
            printf( "SAT_INFO with %d bytes\n", output->length );
            break;

        case XDI_GNSS_PVT_PULSE:
            printf( "PVT_PULSE with %d bytes\n", output->length );
            break;

        case XDI_RATE_OF_TURN:
            value.type         = XSENS_EVT_TYPE_FLOAT3;
            value.data.f4x3[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x3[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x3[2] = coalesce_32BE_F32LE( &output->payload[8] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_RATE_OF_TURN, &value );
            }
            break;

        case XDI_DELTA_Q:
            value.type         = XSENS_EVT_TYPE_FLOAT4;
            value.data.f4x4[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x4[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x4[2] = coalesce_32BE_F32LE( &output->payload[8] );
            value.data.f4x4[3] = coalesce_32BE_F32LE( &output->payload[12] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_DELTA_Q, &value );
            }
            break;

        case XDI_RATE_OF_TURN_HIGH_RATE:
            printf( "RateOfTurnHR with %d bytes\n", output->length );
            break;

        case XDI_RAW_ACC_GYRO_MAG_TEMP:
            printf( "RawSensors with %d bytes\n", output->length );
            break;

        case XDI_RAW_GYRO_TEMP:
            printf( "RawGyroTemp with %d bytes\n", output->length );
            break;

        case XDI_MAGNETIC_FIELD:
            value.type         = XSENS_EVT_TYPE_FLOAT3;
            value.data.f4x3[0] = coalesce_32BE_F32LE( &output->payload[0] );
            value.data.f4x3[1] = coalesce_32BE_F32LE( &output->payload[4] );
            value.data.f4x3[2] = coalesce_32BE_F32LE( &output->payload[8] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_MAGNETIC, &value );
            }
            break;

        case XDI_VELOCITY_XYZ:
            printf( "VelocityXYZ with %d bytes\n", output->length );
            break;

        case XDI_STATUS_BYTE:
            printf( "StatusByte with %d bytes\n", output->length );
            break;

        case XDI_STATUS_WORD:
            // Casting to a bitfield is end-users responsibility.

            // union XDI_STATUS32_UNION status;
            // status.word = coalesce_32BE_32LE(&output->payload[0]);
            // printf("filterOK: %d\n", status.bitfield.filter_valid);

            value.type    = XSENS_EVT_TYPE_U32;
            value.data.u4 = coalesce_32BE_32LE( &output->payload[0] );
            if( evt_cb )
            {
                evt_cb( XSENS_EVT_STATUS_WORD, &value );
            }
            break;

        case XDI_DEVICE_ID:
            printf( "DeviceID with %d bytes\n", output->length );
            break;

        case XDI_LOCATION_ID:
            printf( "LocationID with %d bytes\n", output->length );
            break;

        default:
            printf( "MData2 Unknown ID 0x%X with %d bytes\n", output->id, output->length );
            break;
    }
}