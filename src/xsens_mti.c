#include "string.h"

#include "xsens_mti.h"
#include "xsens_mti_private.h"


message_handler_ref_t inbound_handler_table[] =
{
    { .id = WAKEUP, .handler_fn = NULL },
    { .id = GOTOCONFIGACK, .handler_fn = NULL },
    { .id = GOTOMEASUREMENTACK, .handler_fn = NULL },
    { .id = RESETACK, .handler_fn = NULL },
    { .id = DEVICEID, .handler_fn = &handle_device_id },
    { .id = PRODUCTCODE, .handler_fn = &handle_product_code },
    { .id = HARDWAREVERSION, .handler_fn = &handle_hardware_version }, 
    { .id = FIRMWAREREV, .handler_fn = &handle_firmware_version }, 
    { .id = SELFTESTRESULTS, .handler_fn = &handle_selftest_results }, 
    { .id = ERROR, .handler_fn = &handle_error }, 
    { .id = WARNING, .handler_fn = &handle_warning }, 
    { .id = CONFIGURATION, .handler_fn = NULL }, 
    { .id = MTDATA2, .handler_fn = &handle_mdata2 }, 
    { .id = REQLATLONALTACK, .handler_fn = NULL }, 
    { .id = SETLATLONALTACK, .handler_fn = NULL }, 
    { .id = REQFILTERPROFILEACK, .handler_fn = NULL }, 
    { .id = REQGNSSPLATFORMACK, .handler_fn = NULL }, 
    { .id = REQGNSSLEVERARMACK, .handler_fn = NULL }, 
    { .id = SETGNSSLEVERARMACK, .handler_fn = NULL }, 
    { .id = UTCTIME, .handler_fn = NULL }, 
    { .id = ICCCOMMANDACK, .handler_fn = NULL }, 
    { .id = SETINITIALHEADINGACK, .handler_fn = NULL }, 
    { .id = FORWARDGNSSDATAACK, .handler_fn = NULL }, 
};


// Setup callbacks
void init( interface_t *interface )
{

}


// Helper to parse inbound data with one call
void parse_buffer( interface_t *interface, uint8_t *buffer, uint16_t size )
{
    for( uint16_t i = 0; i < size; i++)
    {
        parse( interface, buffer[i] );
    }
}

// Run each byte through the packet-level statemachine
void parse( interface_t *interface, uint8_t byte )
{
    // CRC is the sum of bytes including the CRC byte (ex PREAMBLE)
    if( interface->state != PARSER_PREAMBLE )
    {
        interface->crc += (byte & 0xFF);
    }

    switch( interface->state )
    {
        case PARSER_PREAMBLE:
            if( byte == PREAMBLE_BYTE )
            {
                reset_parser( interface );
                interface->state = PARSER_ADDRESS;
            }
        break;

        case PARSER_ADDRESS:
            if( byte == ADDRESS_BYTE )
            {
                interface->state = PARSER_MESSAGE_ID;
            }
        break;

        case PARSER_MESSAGE_ID:
            interface->packet.message_id = byte;
            interface->state = PARSER_LENGTH;
        break;

        case PARSER_LENGTH:
            if( byte == LENGTH_EXTENDED_MODE )
            {
                printf("Extended packet?\n");
                interface->state = PARSER_LENGTH_EXTENDED_B1;
            }
            if( byte == LENGTH_NONE )
            {
                interface->packet.length = 0;
                interface->state = PARSER_CRC;
            }
            else
            {
                interface->packet.length = byte;
                interface->state = PARSER_PAYLOAD;
            }
        break;

        case PARSER_LENGTH_EXTENDED_B1:
            interface->packet.length = byte;
            // TODO decode long length
            interface->state = PARSER_LENGTH_EXTENDED_B2;
        break;

        case PARSER_LENGTH_EXTENDED_B2:
            // TODO decode long length
            interface->packet.length &= byte << 8;
            interface->state = PARSER_PAYLOAD;
        break;

        case PARSER_PAYLOAD:
            interface->packet.payload[interface->payload_pos] = byte;
            interface->payload_pos++;
            
            // Once we've buffered the whole payload,
            // prepare to read the CRC
            if( interface->payload_pos >= interface->packet.length )
            {

                interface->state = PARSER_CRC;
            }
        break;

        case PARSER_CRC:
            // Check if CRC is valid
            if( interface->crc == 0x00 )
            {
                // Packet was successfully recieved
                // Run the payload handling function
                handle_payload( interface );
            }
            else
            {
                printf("CRC Fail: 0x%x ID: 0x%X?\n", interface->crc, interface->packet.message_id);
            }

            interface->state = PARSER_PREAMBLE;
        break;
    }
}

void reset_parser( interface_t *interface )
{
    // Clear the parser state and buffers
    memset( &(interface->packet), 0, sizeof(packet_buffer_t) );
    interface->payload_pos = 0;
    interface->crc = 0;
}




// With a valid packet, process the payload
void handle_payload( interface_t *interface )
{
    packet_buffer_t *packet = &(interface->packet);

    // Search the inbound handler table for a match
    message_handler_ref_t *handler = find_inbound_handler_table_entry( packet->message_id );

    // If the ID is recognised, call the handler function (if it exists)
    if( handler )
    {
        callback_payload_t payload_handler_fn = handler->handler_fn;
        if( payload_handler_fn )
        {
            payload_handler_fn( packet );
        }
    }

}

bool override_id_handler( uint8_t id, callback_payload_t *user_fn )
{
    if( user_fn )
    {
        // Find the ID in the inbound handler 'jump table'
        message_handler_ref_t *handler = find_inbound_handler_table_entry( id );
        
        if( handler )
        {
            handler->handler_fn = user_fn;
        }
    }
}

message_handler_ref_t * find_inbound_handler_table_entry( uint8_t find_id )
{
    
    uint8_t table_length = sizeof(inbound_handler_table) / sizeof(message_handler_ref_t);

    for( uint8_t i = 0; i < table_length; i++ )
    {
        if( inbound_handler_table[i].id == find_id )
        {
            return &inbound_handler_table[i];
        }
    }

    return (message_handler_ref_t*)NULL;
}






void handle_device_id( packet_buffer_t *packet )
{
    printf("DeviceID Handler\n");
}

void handle_product_code( packet_buffer_t *packet )
{
    printf("ProductCode Handler\n");

}

void handle_hardware_version( packet_buffer_t *packet )
{
    printf("HardwareVersion Handler\n");

}

void handle_firmware_version( packet_buffer_t *packet )
{
    printf("FirmwareVersion Handler\n");

}

void handle_selftest_results( packet_buffer_t *packet )
{
    printf("SelfTest Handler\n");
}

void handle_error( packet_buffer_t *packet )
{
    printf("Error Handler\n");

}

void handle_warning( packet_buffer_t *packet )
{
    printf("Warning Handler\n");

}


typedef enum {
    XDI_PARSE_ID_B1,
    XDI_PARSE_ID_B2,
    XDI_PARSE_LENGTH,
    XDI_PARSE_DATA
} mdata2_parser_state_t;

typedef struct 
{
    uint16_t id;
    uint8_t length;
    uint8_t payload[255];   // TODO: work out what size is actually needed
} mdata2_packet_t;

enum  {
    XDI_UTC_TIME = 0x1010,
    XDI_PACKET_COUNTER = 0x1020,
    XDI_SAMPLE_TIME_FINE = 0x1060,
    XDI_SAMPLE_TIME_COARSE = 0x1070,

    XDI_TEMPERATURE = 0x0810,

    XDI_QUATERNION = 0x2010,
    XDI_ROTATION_MATRIX = 0x2020,
    XDI_EULER_ANGLES = 0x2030,

    XDI_BARO_PRESSURE = 0x3010,

    XDI_DELTA_V = 0x4010,
    XDI_ACCELERATION = 0x4020,
    XDI_FREE_ACCELERATION = 0x4030,
    XDI_ACCELERATION_HIGH_RATE = 0x4040,

    XDI_ALTITUDE_ELLIPSOID = 0x5020,
    XDI_POSITION_ECEF = 0x5030,
    XDI_LAT_LON = 0x5040,
    XDI_GNSS_PVT_DATA = 0x7010,
    XDI_GNSS_SAT_INFO = 0x7020,
    XDI_GNSS_PVT_PULSE = 0x7030,

    XDI_RATE_OF_TURN = 0x8020,
    XDI_DELTA_Q = 0x8030,
    XDI_RATE_OF_TURN_HIGH_RATE = 0x8040,

    XDI_RAW_ACC_GYRO_MAG_TEMP = 0xA010,
    XDI_RAW_GYRO_TEMP = 0xA020,

    XDI_MAGNETIC_FIELD = 0xC020,
    XDI_VELOCITY_XYZ = 0xD010,

    XDI_STATUS_BYTE = 0xE010,
    XDI_STATUS_WORD = 0xE020,
    XDI_DEVICE_ID = 0xE080,
    XDI_LOCATION_ID = 0xE090,

} XDA_TYPE_IDENTIFIER;

void handle_mdata2_output( mdata2_packet_t *output );

// MData2 packets have a series of smaller structures of data
// This handler walks through the buffer, identifies the XDA type from two bytes
// Then applies relevant conversions back into native types/structures as necessary
// Packets don't have a fixed number of child elements
void handle_mdata2( packet_buffer_t *packet )
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
                    handle_mdata2_output( &output );

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


inline uint16_t coalesce_16BE_16LE( uint8_t *source );
inline uint32_t coalesce_32BE_32LE( uint8_t *source );
inline float coalesce_32BE_F32LE( uint8_t *source );

inline uint16_t coalesce_16BE_16LE( uint8_t *source )
{
    return ( source[1] << 0 | source[0] << 8 );
}

inline uint32_t coalesce_32BE_32LE( uint8_t *source )
{
    return ( (source[3] << 0) | (source[2] << 8) | (source[1] << 16) | source[0] << 24);
}

inline float coalesce_32BE_F32LE( uint8_t *source )
{
    float f;
    uint8_t *f_ptr = (uint8_t *) &f;
    f_ptr[3] = source[0];
    f_ptr[2] = source[1];
    f_ptr[1] = source[2];
    f_ptr[0] = source[3];

    return f;
}

void handle_mdata2_output( mdata2_packet_t *output )
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
        break;

        case XDI_DEVICE_ID:
            printf("DeviceID with %d bytes\n", output->length );
        break;

        case XDI_LOCATION_ID:
            printf("LoccationID with %d bytes\n", output->length );
        break;

        default:
            printf("MData2 Unknown ID 0x%X with %d bytes\n", output->id, output->length );
        break;
    }

}