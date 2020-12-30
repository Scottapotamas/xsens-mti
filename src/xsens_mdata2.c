#include "xsens_mdata2.h"
#include "xsens_utility.h"

// MData2 packets have a series of smaller structures of data
// This handler walks through the buffer, identifies the XDA type from two bytes
// Then applies relevant conversions back into native types/structures as necessary
// Packets don't have a fixed number of child elements
void xsens_mdata2_process( packet_buffer_t *packet )
{
    mdata2_parser_state_t md2_state = XDI_PARSE_ID_B1;
    mdata2_packet_t output = { 0 };
    uint8_t bytes_consumed = 0;

    // Walk through the packet and run a tiny statemachine
    // to parse the sub-fields
    for( uint16_t i = 0; i < packet->length; i++ )
    {
        switch( md2_state )
        {
            case XDI_PARSE_ID_B1:
                // High byte
                output.id = (uint16_t)((uint16_t)packet->payload[i] << 8u);
                md2_state = XDI_PARSE_ID_B2;
            break;

            case XDI_PARSE_ID_B2:
                // Low byte
                output.id |= (uint16_t)((uint16_t)packet->payload[i] );
                md2_state = XDI_PARSE_LENGTH;
            break;

            case XDI_PARSE_LENGTH:
                // Length is one byte
                output.length = packet->payload[i];
                md2_state = XDI_PARSE_DATA;
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
                    xsens_mdata2_decode_field( &output );

                    // Cleanup our state before parsing remaining fields
                    md2_state = XDI_PARSE_ID_B1;
                    bytes_consumed = 0;
                    memset( &output, 0, sizeof(mdata2_packet_t) );
                }
            break;
        }

    }
    
    // Finished MData2 parsing in payload
}

void xsens_mdata2_decode_field( mdata2_packet_t *output )
{
    // With the 'isolated' field from the rest of the payload,
    // find the matching XID and apply post-processing strategies
    switch( output->id )
    {
        case XDI_UTC_TIME:
            printf("UTC Time with %d bytes\n", output->length );
        break;

        case XDI_PACKET_COUNTER:
            printf("PacketCounter with %d bytes\n", output->length );
            uint16_t count = coalesce_16BE_16LE(&output->payload[0]);
            printf("    Value: %d\n", count);
        break;

        case XDI_SAMPLE_TIME_FINE:
            printf("SampleTimeFine with %d bytes\n", output->length );
            uint32_t fine = coalesce_32BE_32LE(&output->payload[0]);
            printf("    Value: %d\n", fine);
        break;

        case XDI_SAMPLE_TIME_COARSE:
            printf("SampleTimeCoarse with %d bytes\n", output->length );
            uint32_t coarse = coalesce_32BE_32LE(&output->payload[0]);
            printf("    Value: %d\n", coarse);
        break;

        case XDI_TEMPERATURE:
            printf("Temperature with %d bytes\n", output->length );
            float temp = coalesce_32BE_F32LE(&output->payload[0]);
            printf("    Temp: %f\n", temp);
        break;

        case XDI_QUATERNION:
            printf("Quaternion with %d bytes\n", output->length );
            
            float quat[4] = { 0.0f };

            quat[0] = coalesce_32BE_F32LE(&output->payload[0]);
            quat[1] = coalesce_32BE_F32LE(&output->payload[4]);
            quat[2] = coalesce_32BE_F32LE(&output->payload[8]);
            quat[3] = coalesce_32BE_F32LE(&output->payload[12]);

            printf("    q0: %f\n", quat[0]);
            printf("    q1: %f\n", quat[1]);
            printf("    q2: %f\n", quat[2]);
            printf("    q3: %f\n", quat[3]);

        break;

        case XDI_ROTATION_MATRIX:
            printf("RotationMatrix with %d bytes\n", output->length );
        break;

        case XDI_EULER_ANGLES:
            printf("EulerAngles with %d bytes\n", output->length );
        break;

        case XDI_BARO_PRESSURE:
            printf("Pressure with %d bytes\n", output->length );
            uint32_t pressure = coalesce_32BE_32LE(&output->payload[0]);
            printf("    pressure: %d\n", pressure);
        break;

        case XDI_DELTA_V:
            printf("DeltaV with %d bytes\n", output->length );
            
            float dv[3] = { 0.0f };

            dv[0] = coalesce_32BE_F32LE(&output->payload[0]);
            dv[1] = coalesce_32BE_F32LE(&output->payload[4]);
            dv[2] = coalesce_32BE_F32LE(&output->payload[8]);

            printf("    dv.x: %f\n", dv[0]);
            printf("    dv.y: %f\n", dv[1]);
            printf("    dv.z: %f\n", dv[2]);
        break;

        case XDI_ACCELERATION:
            printf("Acceleration with %d bytes\n", output->length );
            
            float acc[3] = { 0.0f };

            acc[0] = coalesce_32BE_F32LE(&output->payload[0]);
            acc[1] = coalesce_32BE_F32LE(&output->payload[4]);
            acc[2] = coalesce_32BE_F32LE(&output->payload[8]);

            printf("    acc.x: %f\n", acc[0]);
            printf("    acc.y: %f\n", acc[1]);
            printf("    acc.z: %f\n", acc[2]);
        break;

        case XDI_FREE_ACCELERATION:
            printf("FreeAcceleration with %d bytes\n", output->length );
            float fracc[3] = { 0.0f };

            fracc[0] = coalesce_32BE_F32LE(&output->payload[0]);
            fracc[1] = coalesce_32BE_F32LE(&output->payload[4]);
            fracc[2] = coalesce_32BE_F32LE(&output->payload[8]);

            printf("    frAcc.x: %f\n", fracc[0]);
            printf("    frAcc.y: %f\n", fracc[1]);
            printf("    frAcc.z: %f\n", fracc[2]);
        break;

        case XDI_ACCELERATION_HIGH_RATE:
            printf("AccelerationHR with %d bytes\n", output->length );
        break;

        case XDI_ALTITUDE_ELLIPSOID:
            printf("AltitudeEllipsoid with %d bytes\n", output->length );
        break;

        case XDI_POSITION_ECEF:
            printf("PositionECEF with %d bytes\n", output->length );
        break;

        case XDI_LAT_LON:
            printf("LAT_LON with %d bytes\n", output->length );
        break;

        case XDI_GNSS_PVT_DATA:
            printf("PVT_DATA with %d bytes\n", output->length );
        break;

        case XDI_GNSS_SAT_INFO:
            printf("SAT_INFO with %d bytes\n", output->length );
        break;

        case XDI_GNSS_PVT_PULSE:
            printf("PVT_PULSE with %d bytes\n", output->length );
        break;

        case XDI_RATE_OF_TURN:
            printf("RateOfTurn with %d bytes\n", output->length );
            float rot[3] = { 0.0f };

            rot[0] = coalesce_32BE_F32LE(&output->payload[0]);
            rot[1] = coalesce_32BE_F32LE(&output->payload[4]);
            rot[2] = coalesce_32BE_F32LE(&output->payload[8]);

            printf("    rot.x: %f\n", rot[0]);
            printf("    rot.y: %f\n", rot[1]);
            printf("    rot.z: %f\n", rot[2]);
        break;

        case XDI_DELTA_Q:
            printf("DeltaQ with %d bytes\n", output->length );
            float dq[4] = { 0.0f };

            dq[0] = coalesce_32BE_F32LE(&output->payload[0]);
            dq[1] = coalesce_32BE_F32LE(&output->payload[4]);
            dq[2] = coalesce_32BE_F32LE(&output->payload[8]);
            dq[3] = coalesce_32BE_F32LE(&output->payload[12]);

            printf("    dq0: %f\n", dq[0]);
            printf("    dq1: %f\n", dq[1]);
            printf("    dq2: %f\n", dq[2]);
            printf("    dq3: %f\n", dq[3]);
        break;

        case XDI_RATE_OF_TURN_HIGH_RATE:
            printf("RateOfTurnHR with %d bytes\n", output->length );
        break;

        case XDI_RAW_ACC_GYRO_MAG_TEMP:
            printf("RawSensors with %d bytes\n", output->length );
        break;

        case XDI_RAW_GYRO_TEMP:
            printf("RawGyroTemp with %d bytes\n", output->length );
        break;

        case XDI_MAGNETIC_FIELD:
            printf("MagneticField with %d bytes\n", output->length );
            float mag[3] = { 0.0f };

            mag[0] = coalesce_32BE_F32LE(&output->payload[0]);
            mag[1] = coalesce_32BE_F32LE(&output->payload[4]);
            mag[2] = coalesce_32BE_F32LE(&output->payload[8]);

            printf("    mag.x: %f\n", mag[0]);
            printf("    mag.y: %f\n", mag[1]);
            printf("    mag.z: %f\n", mag[2]);
        break;

        case XDI_VELOCITY_XYZ:
            printf("VelocityXYZ with %d bytes\n", output->length );
        break;

        case XDI_STATUS_BYTE:
            printf("StatusByte with %d bytes\n", output->length );
        break;

        case XDI_STATUS_WORD:
            printf("StatusWord with %d bytes\n", output->length );
            union XDI_STATUS32_UNION status;
            status.word = coalesce_32BE_32LE(&output->payload[0]);

            printf("    filterOK: %d\n", status.bitfield.filter_valid);
            printf("    clipping: %d\n", status.bitfield.clipping);
            printf("    selftest: %d\n", status.bitfield.self_test);

        break;

        case XDI_DEVICE_ID:
            printf("DeviceID with %d bytes\n", output->length );
        break;

        case XDI_LOCATION_ID:
            printf("LocationID with %d bytes\n", output->length );
        break;

        default:
            printf("MData2 Unknown ID 0x%X with %d bytes\n", output->id, output->length );
        break;
    }

}