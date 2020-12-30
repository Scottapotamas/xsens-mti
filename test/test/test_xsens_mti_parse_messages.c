// These tests cover inbound message IDs
// Tests written from real captured packets from MTi-300

#include "unity.h"
#include <string.h>

// MODULE UNDER TEST
#include "xsens_mti.h"
#include "xsens_mti_private.h"
#include "xsens_constants.h"
#include "xsens_mdata2.h"
#include "xsens_utility.h"

// DEFINITIONS 
 
// PRIVATE TYPES
 
// PRIVATE DATA
interface_t test_imu = { 0 };

// PRIVATE FUNCTIONS

void mock_output_function( uint8_t *buffer, uint16_t size );
void mock_event_function( EventFlags_t event );

void mock_output_function( uint8_t *buffer, uint16_t size )
{
    printf("Output of %d bytes\n", size);
}

void mock_event_function( EventFlags_t event )
{
    printf("Notified of evt: %d\n", event);
}

// SETUP, TEARDOWN
void setUp(void)
{
    memset( &test_imu, 0, sizeof(test_imu) );
    test_imu.output_cb = &mock_output_function;
    test_imu.event_cb = &mock_event_function;
    init( &test_imu );
}
 
void tearDown(void)
{

}

// TESTS
void test_parse_GoToConfigAck( void )
{   
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x31, 
                                0x00,
                                0xD0 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();
}

void test_parse_StringOutputType( void )
{   
    // <no data (possible: StringOutputType, 2 bytes)>
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x8F, 
                                0x00,
                                0x72 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();
}

void test_parse_OutputConfiguration( void )
{   
    //{(PacketCounter, Freq: 65535), (SampleTimeFine, Freq: 65535)}
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0xC1, 
                                0x08,
                                0x10, 0x20, 0xFF, 0xFF, 0x10, 0x60, 0xFF, 0xFF, 
                                0x9C };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();

}

void test_parse_InitMTResults( void )
{   
    //DeviceID: 037003F8
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x03, 
                                0x04,
                                0x03, 0x70, 0x03, 0xF8, 
                                0x8C };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();
}

void test_parse_Configuration( void )
{
    // DeviceID: 037003F8, 
    // Sample Period: 1152, 
    // OutputSkipFactor: 0, 
    // SyncInMode: 0, 
    // SyncInSkipFactor: 0, 
    // SyncInOffset: 0, (Year: 0, Month: 0, Day: 0), (Hour: 0, Min: 0, Sec: 0, Ms: 0), 
    // reserved: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00, 
    // reserved: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00, 
    // numDevices: 1, 
    // DeviceID: 037003F8, 
    // MTDataLength: 0, 
    // OutputMode: 0, 
    // OutputSettings: 1, 
    // reserved: 00 27 01 08 02 49 05 01
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x0D, 
                                0x76,
                                0x03, 0x70, 0x03, 0xF8, 0x04, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x70, 0x03, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x27, 0x01, 0x08, 0x02, 0x49, 0x05, 0x01,
                                0x9B };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();

}

void test_parse_FirmwareRev( void )
{   
    //(major: 1, minor: 8, revision: 2, buildnr: 37, svnrev: 70964)
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x13, 
                                0x0B,
                                0x01, 0x08, 0x02, 0x00, 0x00, 0x00, 0x25, 0x00, 0x01, 0x15, 0x34, 
                                0x69 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();
}

void test_parse_GotoMeasurementAck( void )
{   
    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x11, 
                                0x00,
                                0xF0 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();
}




void test_parse_mdata2_quat_minimal( void )
{   
    // Basic quaternion 'only' packet

    // (PacketCounter, 2 bytes, 18050),
    // (SampleTimeFine, 4 bytes, 29686846),
    // (Quaternion|Float|ENU, 16 bytes, (q0:   0.94455600, q1:  -0.32308814, q2:   0.01374718, q3:  -0.05691256)),
    // (StatusWord, 4 bytes, 00000000010000000000000000000011)

    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x36, 
                                0x26,
                                0x10, 0x20, 0x02, 0x46, 0x82, 0x10, 0x60, 0x04, 0x01, 0xC4, 0xFC, 0x3E, 0x20, 0x10, 0x10, 0x3F, 0x71, 0xCE, 0x6C, 0xBE, 0xA5, 0x6B, 0xCF, 0x3C, 0x61, 0x3B, 0xD8, 0xBD, 0x69, 0x1D, 0x25, 0xE0, 0x20, 0x04, 0x00, 0x40, 0x00, 0x03,
                                0x12 };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));

    TEST_IGNORE();
}

void test_parse_mdata2_shake_clipping( void )
{   
    // Packet when the IMU was being shaked intensely
    // Gyro, Acc and Mag should be clipping
    // FilterValid flag should be false

    // (PacketCounter, 2 bytes, 64389), 
    // (SampleTimeFine, 4 bytes, 27564254),
    // (Quaternion|Float|ENU, 16 bytes,
    // (q0: 0.66437358, q1: -0.42175028, q2: 0.02720882, q3: 0.61643654)),
    // (Acceleration|Float, 12 bytes,
    // (accX: -30.28455162, accY: -29.60960007, accZ: -71.76024628)),
    // (DeltaV|Float, 12 bytes,
    // (x: -0.07186279, y: -0.07130830, z: -0.18206376)),
    // (FreeAcceleration|Float, 12 bytes, (freeAccX: 52.39491272, freeAccY: -62.83823395, freeAccZ: -25.59408188)),
    // (RateOfTurn|Float, 12 bytes, (gyrX: 4.16570139, gyrY: -10.33340263, gyrZ: -4.51734877)),
    // (DeltaQ|Float, 16 bytes, (q0: 0.99988699, q1: 0.00520693, q2: -0.01291627, q3: -0.00564647)),
    // (MagneticField|Float, 12 bytes, (magX: 0.43057421, magY: -0.23942292, magZ: 1.37189472)),
    // (BaroPressure, 4 bytes, Pressure: 100062), 
    // (StatusWord, 4 bytes, 00000000010010000001010000000001)

    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x36, 
                                0x8B,
                                0x10, 0x20, 0x02, 0xFB, 0x85, 0x10, 0x60, 0x04, 0x01, 0xA4, 0x98, 0xDE, 0x20, 0x10, 0x10, 0x3F, 0x2A, 0x14, 0x63, 0xBE, 0xD7, 0xEF, 0xA7, 0x3C, 0xDE, 0xE5, 0x08, 0x3F, 0x1D, 0xCE, 0xC9, 0x40, 0x20, 0x0C, 0xC1, 0xF2, 0x46, 0xC3, 0xC1, 0xEC, 0xE0, 0x76, 0xC2, 0x8F, 0x85, 0x3F, 0x40, 0x10, 0x0C, 0xBD, 0x93, 0x2C, 0xCC, 0xBD, 0x92, 0x0A, 0x16, 0xBE, 0x3A, 0x6E, 0xEC, 0x40, 0x30, 0x0C, 0x42, 0x51, 0x94, 0x64, 0xC2, 0x7B, 0x5A, 0x5A, 0xC1, 0xCC, 0xC0, 0xAE, 0x80, 0x20, 0x0C, 0x40, 0x85, 0x4D, 0x6D, 0xC1, 0x25, 0x55, 0x9E, 0xC0, 0x90, 0x8E, 0x1F, 0x80, 0x30, 0x10, 0x3F, 0x7F, 0xF8, 0x98, 0x3B, 0xAA, 0x9E, 0xE5, 0xBC, 0x53, 0x9E, 0xC0, 0xBB, 0xB9, 0x06, 0x0C, 0xC0, 0x20, 0x0C, 0x3E, 0xDC, 0x74, 0x39, 0xBE, 0x75, 0x2B, 0x48, 0x3F, 0xAF, 0x9A, 0x3F, 0x30, 0x10, 0x04, 0x00, 0x01, 0x86, 0xDE, 0xE0, 0x20, 0x04, 0x00, 0x48, 0x14, 0x01, 
                                0x8E };

    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));
    
    TEST_IGNORE();

}

void test_parse_mdata2_full( void )
{
    // Tests a mdata2 packet where most fields are in one packet

    // (PacketCounter, 2 bytes, 37261),
    // (SampleTimeFine, 4 bytes, 20332454),
    // (Quaternion|Float|ENU, 16 bytes,
    // (q0:   0.71045315, q1:   0.69453555, q2:  -0.07777759, q3:  -0.08262789)),
    // (Acceleration|Float, 12 bytes,
    // (accX:  -0.05550629, accY:   9.81465530, accZ:   0.21842313)),
    // (DeltaV|Float, 12 bytes,
    // (x:  -0.00013867, y:   0.02453661, z:   0.00054736)),
    // (FreeAcceleration|Float, 12 bytes,
    // (freeAccX:  -0.01142347, freeAccY:   0.01110744, freeAccZ:   0.02007198)),
    // (RateOfTurn|Float, 12 bytes,
    // (gyrX:   0.02131760, gyrY:  -0.00327826, gyrZ:  -0.00163019)),
    // (DeltaQ|Float, 16 bytes,
    // (q0:   1.00000000, q1:   0.00002665, q2:  -0.00000410, q3:  -0.00000204)),
    // (MagneticField|Float, 12 bytes,
    // (magX:  -0.49215657, magY:   0.70221740, magZ:  -1.25496686)),
    // (Temperature|Float, 4 bytes, Temp:  37.62500000),
    // (BaroPressure, 4 bytes, Pressure: 100065),
    // (StatusWord, 4 bytes, 00000000010000000000000000000011)

    uint8_t test_packet[] = {   0xFA, 
                                0xFF, 
                                0x36, 
                                0x92,
                                0x10, 0x20, 0x02, 0x91, 0x8D, 0x10, 0x60, 0x04, 0x01, 0x36, 0x3F, 0xA6, 0x20, 0x10, 0x10, 0x3F, 0x35, 0xE0, 0x42, 0x3F, 0x31, 0xCD, 0x15, 0xBD, 0x9F, 0x49, 0xDB, 0xBD, 0xA9, 0x38, 0xCF, 0x40, 0x20, 0x0C, 0xBD, 0x63, 0x5A, 0x90, 0x41, 0x1D, 0x08, 0xD4, 0x3E, 0x5F, 0xAA, 0x50, 0x40, 0x10, 0x0C, 0xB9, 0x11, 0x68, 0x00, 0x3C, 0xC9, 0x01, 0x00, 0x3A, 0x0F, 0x7D, 0x00, 0x40, 0x30, 0x0C, 0xBC, 0x3B, 0x29, 0x80, 0x3C, 0x35, 0xFC, 0x00, 0x3C, 0xA4, 0x6E, 0x00, 0x80, 0x20, 0x0C, 0x3C, 0xAE, 0xA2, 0x41, 0xBB, 0x56, 0xD8, 0x00, 0xBA, 0xD5, 0xAC, 0x01, 0x80, 0x30, 0x10, 0x3F, 0x80, 0x00, 0x00, 0x37, 0xDF, 0x88, 0x01, 0xB6, 0x89, 0x80, 0x00, 0xB6, 0x08, 0xC0, 0x00, 0xC0, 0x20, 0x0C, 0xBE, 0xFB, 0xFB, 0xF2, 0x3F, 0x33, 0xC4, 0x85, 0xBF, 0xA0, 0xA2, 0xC1, 0x08, 0x10, 0x04, 0x42, 0x16, 0x80, 0x00, 0x30, 0x10, 0x04, 0x00, 0x01, 0x86, 0xE1, 0xE0, 0x20, 0x04, 0x00, 0x40, 0x00, 0x03, 
                                0xDD };
    
    parse_buffer( &test_imu, &test_packet, sizeof(test_packet));

    TEST_IGNORE();
}
