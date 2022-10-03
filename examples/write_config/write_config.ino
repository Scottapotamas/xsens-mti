// Configure the library to support writing to the IMU, 
// used for configuration of baud-rate, settings etc without MTManager

#include "xsens_mti.h"      // main library
#include "xsens_utility.h"

// Cache a copy of IMU data
float acceleration[3] = { 0 };   // in m/s^2
uint32_t last_measurement_ms = 0;
uint32_t measurement_timer = 0;

// Callback functions we'll use to catch IMU ack packets
void handle_ack_gotoconfig( xsens_packet_buffer_t *packet );
void handle_ack_gotomeasurement( xsens_packet_buffer_t *packet );
void handle_ack_outputconfiguration( xsens_packet_buffer_t *packet );

typedef enum {
    ACK_NONE = 0,
    ACK_CONFIG,
    ACK_MEASUREMENT,
    ACK_CONFIGURED
} ACKFlags_t;

ACKFlags_t ack_flag = ACK_NONE;


void imu_callback( XsensEventFlag_t event, XsensEventData_t *mtdata );
void imu_send_data( uint8_t *data, uint16_t length );

// The library holds state and pointers to callbacks in this structure
//   - adding the imu_send_data ptr lets the library talk to the IMU
xsens_interface_t imu_interface = XSENS_INTERFACE_RX_TX( &imu_callback, &imu_send_data );


// Simple state handling for configuration flow state-machine
typedef enum {
    STATE_STARTUP = 0,
    STATE_REQUEST_CONFIG_MODE,
    STATE_ACK_CONFIG_MODE,
    STATE_SET_OUTPUT_CONFIG,
    STATE_ACK_OUTPUT_CONFIG,
    STATE_REQUEST_MEASUREMENT_MODE,
    STATE_ACK_MEASUREMENT_MODE,
} DemoStates_t;

DemoStates_t demo_state     = STATE_STARTUP;
uint32_t     demo_timer     = 0;
bool         configure_fast = false;


void setup( void )
{
    Serial.begin( 115200 );     // serial debug text
    Serial1.begin( 115200 );    // connected to IMU

    // Setup custom handler callbacks to catch acknowledgements from IMU
    xsens_mti_override_id_handler( MT_ACK_GOTOCONFIG, &handle_ack_gotoconfig );
    xsens_mti_override_id_handler( MT_ACK_GOTOMEASUREMENT, &handle_ack_gotomeasurement );
    xsens_mti_override_id_handler( MT_ACK_OUTPUTCONFIGURATION, &handle_ack_outputconfiguration );

    Serial.println("Configuration demo starting...");

    demo_timer = millis();
    measurement_timer = millis();
}

void loop( void )
{
    while( Serial1.available() > 0 )  
    {  
        xsens_mti_parse( &imu_interface, Serial1.read() );
    }

    switch( demo_state )
    {
        case STATE_STARTUP:
            // Wait 10 seconds before starting the configuration process
            if( millis() - demo_timer >= 10000 )
            {
                demo_timer = millis();
                demo_state = STATE_REQUEST_CONFIG_MODE;
            } 
            else
            {
                if( millis() - measurement_timer >= 1000)
                {
                    // Been too long since the last inbound packet?
                    Serial.println("\nError: accel overdue?\n");
                }
                else
                {
                    // Acceleration packets have arrived recently, print the rate
                    Serial.print("Accel rate: ");
                    Serial.print( 1000 / last_measurement_ms );
                    Serial.println("Hz");

                    // TODO: non-blocking ratelimit of accel rate printout
                    delay(5);
                }
            }
        break;

        case STATE_REQUEST_CONFIG_MODE:
            Serial.println("Requesting config mode...");
            xsens_mti_request( &imu_interface, MT_GOTOCONFIG );
            demo_state = STATE_ACK_CONFIG_MODE;
        break;

        case STATE_ACK_CONFIG_MODE:
            // Wait for GoToConfigAck to return
            // handle_gotoconfigack will fire when it does, and set our flag
            if( ack_flag == ACK_CONFIG)
            {
                Serial.println("IMU in config mode");
                demo_state = STATE_SET_OUTPUT_CONFIG;
                ack_flag   = ACK_NONE;
            }
        break;

        case STATE_SET_OUTPUT_CONFIG:
        {
            uint8_t output_rate = 10;   // Hz acceleration message rate

            if( configure_fast )
            {
                output_rate = 100;
            }

            configure_fast != configure_fast;


            Serial.println("Setting acceleration output to ");
            Serial.print(output_rate);
            Serial.println("Hz");

            XsensFrequencyConfig_t settings[] = {
                { .id = XDI_PACKET_COUNTER, .frequency = 0xFFFF },
                { .id = XDI_SAMPLE_TIME_FINE, .frequency = 0xFFFF },
                { .id = XDI_ACCELERATION, .frequency = output_rate },
            //  { .id = XSENS_IDENTIFIER_FORMAT(XDI_QUATERNION, XSENS_FLOAT_FIXED1220, XSENS_COORD_ENU), .frequency = 100 },
            };

            xsens_mti_set_configuration( &imu_interface, settings, XSENS_ARR_ELEM(settings) );
            demo_state = STATE_ACK_OUTPUT_CONFIG;
        }
        break;

        case STATE_ACK_OUTPUT_CONFIG:
            // Read configuration packet response
            // TODO: MDATA2 odd input handling?

            if( ack_flag == ACK_CONFIGURED)
            {
                Serial.println("IMU confirmed config...");
                demo_state = STATE_REQUEST_MEASUREMENT_MODE;
                ack_flag   = ACK_NONE;
            }
        break;

        case STATE_REQUEST_MEASUREMENT_MODE:
            Serial.println("Requesting measurement mode...");
            xsens_mti_request( &imu_interface, MT_GOTOMEASUREMENT );
            demo_state = STATE_ACK_MEASUREMENT_MODE;
        break;

        case STATE_ACK_MEASUREMENT_MODE:
            // Wait x in this mode before attempting to configure different settings

            if( ack_flag == ACK_MEASUREMENT)
            {
                uint32_t duration_ms = millis() - demo_timer;

                Serial.println("IMU back in measurement mode");

                Serial.print(" - took ");
                Serial.print(duration_ms);
                Serial.println("ms\n");

                // Go back to the start of the settings flow
                demo_state = STATE_STARTUP;
                ack_flag   = ACK_NONE;
                demo_timer = millis();
            }
        break;

        default:
            // Oops!
            demo_state = STATE_STARTUP;
        break;
    }

}

// Command ACK callback functions
void handle_ack_gotoconfig( xsens_packet_buffer_t *packet )
{
    ack_flag = ACK_CONFIG;
}

void handle_ack_gotomeasurement( xsens_packet_buffer_t *packet )
{
    ack_flag = ACK_MEASUREMENT;
}

void handle_ack_outputconfiguration( xsens_packet_buffer_t *packet )
{
    ack_flag = ACK_CONFIGURED;
}

// MData2 callback for IMU events
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

            last_measurement_ms = millis() - measurement_timer;
            measurement_timer = millis();
        break;
    }
}

// The library calls this function to send packets to the IMU
void imu_send_data( uint8_t *data, uint16_t len )
{
    Serial1.write( data, len );
}