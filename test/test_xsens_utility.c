#include "unity.h"
#include <string.h>

// MODULE UNDER TEST
#include "xsens_utility.h"

// DEFINITIONS 
 
// PRIVATE TYPES
 
// PRIVATE DATA

// PRIVATE FUNCTIONS


// SETUP, TEARDOWN
void setUp(void)
{
}
 
void tearDown(void)
{

}

// TESTS
void test_endian_swap_u16( void )
{   
    uint16_t be     = 0xAAFF;
    uint16_t le     = 0xFFAA;
    uint16_t result = 0x0000;
    xsens_swap_endian_u16( &result, &le );
    TEST_ASSERT_EQUAL_HEX16( be, result );

    be     = 0x4321;
    le     = 0x2143;
    result = 0x0000;
    xsens_swap_endian_u16( &result, &le );
    TEST_ASSERT_EQUAL_HEX16( be, result );

    be     = 0x0000;
    le     = 0x0000;
    result = 0x0000;
    xsens_swap_endian_u16( &result, &le );
    TEST_ASSERT_EQUAL_HEX16( be, result );
}

void test_endian_swap_u32( void )
{   
    uint32_t be     = 0xAABBCCDD;
    uint32_t le     = 0xDDCCBBAA;
    uint32_t result = 0x00000000;
    xsens_swap_endian_u32( &result, &le );
    TEST_ASSERT_EQUAL_HEX32( be, result );

    be     = 0x00110000;
    le     = 0x00001100;
    result = 0x00000000;
    xsens_swap_endian_u32( &result, &le );
    TEST_ASSERT_EQUAL_HEX32( be, result );
}

void test_quaternion_euler_basic( void )
{   
    // Euler angles are in radians
    float expected_euler[3] = { 1.0f, 0.0f, 0.0f };

    float quaternion[4] = { 0.8775826f, 0.4794255f, 0.0f, 0.0f};
    float result_euler[3] = { 0.0f };

    xsens_quaternion_to_euler( &quaternion, &result_euler );
    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_euler, result_euler, 3);
}

void test_quaternion_euler_zero( void )
{   
    float expected_euler[3] = { 0.0f, 0.0f, 0.0f };

    float quaternion[4] = { 1.0f, 0.0, 0.0f, 0.0f};
    float result_euler[3] = { 1.0f };

    xsens_quaternion_to_euler( &quaternion, &result_euler );
    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_euler, result_euler, 3);
}

// Roll, Pitch and Yaw all have positive values
void test_quaternion_euler_1( void )
{   
    float expected_euler[3] = { 0.5f, 1.0f, -0.25f };

    float quaternion[4] = { 0.8584542f, 0.1575093f, 0.4879661f, 0.0116753f };
    float result_euler[3] = { 0.0f };

    xsens_quaternion_to_euler( &quaternion, &result_euler );

    //TODO: Investigate quaternion conversion accuracy issues
    TEST_IGNORE();

    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_euler, result_euler, 3);
}

// This one has negative roll angle
void test_quaternion_euler_2( void )
{   
    float expected_euler[3] = { -1.5f, 0.0f, 0.0f };

    float quaternion[4] = { 0.7316889f, -0.6816388f, 0.0f, 0.0f };
    float result_euler[3] = { 0.0f };

    xsens_quaternion_to_euler( &quaternion, &result_euler );
    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_euler, result_euler, 3);
}

void test_euler_quaternion_basic( void )
{   
    float expected_quat[4] = { 0.8775826f, 0.4794255f, 0.0f, 0.0f };

    float euler[3] = { 1.0f, 0.0f, 0.0f };
    float result_quat[4] = { 1.0f };

    xsens_euler_to_quaternion( &euler, &result_quat );
    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_quat, result_quat, 4);
}

void test_euler_quaternion_zero( void )
{   
    float expected_quat[4] = { 1.0f, 0.0f, 0.0f, 0.0f };

    float euler[3] = { 0.0f, 0.0, 0.0f};
    float result_quat[4] = { 1.0f };

    xsens_euler_to_quaternion( &euler, &result_quat );
    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_quat, result_quat, 4);
}

void test_euler_quaternion_1( void )
{   
    float expected_quat[4] = { 0.8584542f, 0.1575093f, 0.4879661f, 0.0116753f };

    float euler[3] = { 0.5f, 1.0f, -0.25f };
    float result_quat[4] = { 1.0f };

    xsens_euler_to_quaternion( &euler, &result_quat );
    
    //TODO: Investigate quaternion conversion accuracy issues
    TEST_IGNORE();

    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_quat, result_quat, 4);
}

void test_euler_quaternion_2( void )
{   
    float expected_quat[4] = { 0.7316889f, -0.6816388f, 0.0f, 0.0f };

    float euler[3] = { -1.5f, 0.0f, 0.0f };
    float result_quat[4] = { 1.0f };

    xsens_euler_to_quaternion( &euler, &result_quat );
    TEST_ASSERT_EQUAL_FLOAT_ARRAY( expected_quat, result_quat, 4);
}