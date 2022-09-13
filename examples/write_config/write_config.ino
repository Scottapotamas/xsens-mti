// Configure the library to support writing to the IMU, 
// used for configuration of baud-rate, settings etc without MTManager

#include "xsens_mti.h"      // main library
#include "xsens_utility.h"  // needed for quaternion conversion function

// Cache a copy of IMU data
float acceleration[3] = { 0 };   // in m/s^2

// Callback functions used by the library
void imu_callback( XsensEventFlag_t event, XsensEventData_t *mtdata );
void imu_send_data( uint8_t *data, uint16_t length );

// The library holds state and pointers to callbacks in this structure
//   - adding the imu_send_data function ptr lets the library talk to the IMU
xsens_interface_t imu_interface = XSENS_INTERFACE_RX_TX( &imu_callback, &imu_send_data );


void setup( void )
{
    Serial.begin( 115200 );
}

void loop( void )
{
    while( Serial.available() > 0 )  
    {  
        xsens_mti_parse( &imu_interface, Serial.read() );
    }

    // TODO: simple demo for IMU reset and settings change

}


void imu_callback( XsensEventFlag_t event, XsensEventData_t *mtdata )
{
    switch( event )
    {
        case XSENS_EVT_ACCELERATION:
            if( mtdata->type == XSENS_EVT_TYPE_FLOAT3 )
            {
                acceleration[0] = mtdata->data.f4x3[0];
                acceleration[1] = mtdata->data.f4x3[1];
                acceleration[2] = mtdata->data.f4x3[2];
            }
            break;
    }
}

// The library calls this function to send packets to the IMU
void imu_send_data( uint8_t *data, uint16_t len )
{
    Serial.write( data, len );
}