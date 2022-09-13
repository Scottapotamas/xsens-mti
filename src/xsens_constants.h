#ifndef XSENS_CONSTANTS_H
#define XSENS_CONSTANTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"

// Library Related Abstractions

// The basic concept of a valid decoded packet
typedef struct
{
    uint8_t  message_id;
    uint16_t length;
    uint8_t  payload[2048]; // xsens 'extended packets' can be up to 2kB
} xsens_packet_buffer_t;

#define XSENS_PACKET_BUF_EMPTY { .message_id = 0, .length = 0, .payload = {0} }

// Event flags sent to application-level code
typedef enum
{
    XSENS_EVT_TEMPERATURE = 0,
    XSENS_EVT_UTC_TIME,
    XSENS_EVT_PACKET_COUNT,
    XSENS_EVT_TIME_FINE,
    XSENS_EVT_TIME_COARSE,
    XSENS_EVT_QUATERNION,
    XSENS_EVT_EULER,
    XSENS_EVT_ROT_MATRIX,
    XSENS_EVT_PRESSURE,
    XSENS_EVT_DELTA_V,
    XSENS_EVT_DELTA_Q,
    XSENS_EVT_ACCELERATION,
    XSENS_EVT_FREE_ACCELERATION,
    XSENS_EVT_ACCELERATION_HR,
    XSENS_EVT_RATE_OF_TURN,
    XSENS_EVT_RATE_OF_TURN_HR,
    XSENS_EVT_GNSS_PVT_PULSE,
    XSENS_EVT_RAW_ACC_GYRO_MAG_TEMP,
    XSENS_EVT_RAW_GYRO_TEMP,
    XSENS_EVT_MAGNETIC,
    XSENS_EVT_STATUS_BYTE,
    XSENS_EVT_STATUS_WORD,
    XSENS_EVT_DEVICE_ID,
    XSENS_EVT_LOCATION_ID,
    XSENS_EVT_POSITION_ECEF,
    XSENS_EVT_LAT_LON,
    XSENS_EVT_ALTITUDE_ELLIPSOID,
    XSENS_EVT_VELOCITY_XYZ,
} XsensEventFlag_t;

// Enum describes possible types sent to user in the event data union
typedef enum
{
    XSENS_EVT_TYPE_NONE = 0,
    XSENS_EVT_TYPE_U8,
    XSENS_EVT_TYPE_U16,
    XSENS_EVT_TYPE_U32,
    XSENS_EVT_TYPE_FLOAT,
    XSENS_EVT_TYPE_FLOAT2,
    XSENS_EVT_TYPE_FLOAT3,
    XSENS_EVT_TYPE_FLOAT4,
    XSENS_EVT_TYPE_FLOAT9,
} XsensEventDataType_t;

// Unionised data sent to user callback
typedef struct
{
    XsensEventDataType_t type;
    union
    {
        uint8_t  u1;
        uint16_t u2;
        uint32_t u4;
        float    f4;
        float    f4x2[2];
        float    f4x3[3];
        float    f4x4[4];
        float    f4x9[9];
    } data;
} XsensEventData_t;

// Userspace callback to notify application layer code of a relevant event
// These events are accompanied by output-ready decoded data
typedef void ( *callback_event_t )( XsensEventFlag_t, XsensEventData_t * );

// Callback to userspace serial write function
// Uses args for a uint8_t buffer of bytes, with a uint16_t size value
typedef void ( *callback_data_out_t )( uint8_t *, uint16_t );

// Specific Values and Structures for xsens handling

#define PREAMBLE_BYTE 0xFA
#define ADDRESS_BYTE  0xFF

#define LENGTH_EXTENDED_MODE 0xFF
#define LENGTH_NONE          0x00

enum XSENS_ERRORCODE
{
    ERROR_PERIOD_INVALID    = 0x03,
    ERROR_MESSAGE_INVALID   = 0x04,
    ERROR_TIMER_OVERFLOW    = 0x30,
    ERROR_BAUDRATE          = 0x20,
    ERROR_PARAMETER_INVALID = 0x21,
    ERROR_DEVICE            = 0x28,
};

// Message Identifiers for MTi->HOST
enum XSENS_MESSAGE_ID_INBOUND
{
    MT_WAKEUP               = 0x3E,
    MT_GOTOCONFIGACK        = 0x31,
    MT_GOTOMEASUREMENTACK   = 0x11,
    MT_RESETACK             = 0x41,
    MT_DEVICEID             = 0x01,
    MT_PRODUCTCODE          = 0x1D,
    MT_HARDWAREVERSION      = 0x1F,
    MT_FIRMWAREREV          = 0x13,
    MT_SELFTESTRESULTS      = 0x25,
    MT_ERROR                = 0x42,
    MT_WARNING              = 0x43,
    MT_CONFIGURATION        = 0x0D,
    MT_MTDATA2              = 0x36,
    MT_REQLATLONALTACK      = 0x6F,
    MT_SETLATLONALTACK      = 0x6F,
    MT_REQFILTERPROFILEACK  = 0x65,
    MT_REQGNSSPLATFORMACK   = 0x77,
    MT_REQGNSSLEVERARMACK   = 0x69,
    MT_SETGNSSLEVERARMACK   = 0x69,
    MT_UTCTIME              = 0x61,
    MT_ICCCOMMANDACK        = 0x75,
    MT_SETINITIALHEADINGACK = 0xD7,
    MT_FORWARDGNSSDATAACK   = 0xE3,
};

// Message Identifiers for Host->MTi
enum XSENS_MESSAGE_ID_OUTBOUND
{
    MT_WAKEUPACK                  = 0x3F,
    MT_GOTOCONFIG                 = 0x30,
    MT_GOTOMEASUREMENT            = 0x10,
    MT_RESET                      = 0x40,
    MT_REQDID                     = 0x00,
    MT_REQPRODUCTCODE             = 0x1C,
    MT_REQHARDWAREVERSION         = 0x1E,
    MT_REQFWREV                   = 0x12,
    MT_RUNSELFTEST                = 0x24,
    MT_REQBAUDRATE                = 0x18,
    MT_SETBAUDRATE                = 0x18,
    MT_REQOPTIONFLAGS             = 0x48,
    MT_SETOPTIONFLAGS             = 0x48,
    MT_REQLOCATIONID              = 0x84,
    MT_SETLOCATIONID              = 0x84,
    MT_RESTOREFACTORYDEF          = 0x0E,
    MT_REQTRANSMITDELAY           = 0xDC,
    MT_SETTRANSMITDELAY           = 0xDC,
    MT_REQCONFIGURATION           = 0x0C,
    MT_REQSTRINGOUTPUTTYPE        = 0x8E,
    MT_SETSTRINGOUTPUTTYPE        = 0x8E,
    MT_REQPERIOD                  = 0x04,
    MT_SETPERIOD                  = 0x04,
    MT_REQOUTPUTSKIPFACTOR        = 0xD4,
    MT_SETOUTPUTSKIPFACTOR        = 0xD4,
    MT_REQALIGNMENTROTATION       = 0xEC,
    MT_SETALIGNMENTROTATION       = 0xEC,
    MT_REQEXTOUTPUTMODE           = 0x86,
    MT_SETEXTOUTPUTMODE           = 0x86,
    MT_REQSETPORTCONFIG           = 0x8C,
    MT_REQSETCANOUTPUTCONFIG      = 0xE8,
    MT_REQSETCANCONFIG            = 0xE6,
    MT_REQDATA                    = 0x34,
    MT_REQLATLONALT               = 0x6E,
    MT_SETLATLONALT               = 0x6E,
    MT_REQAVAILABLEFILTERPROFILES = 0x62,
    MT_REQFILTERPROFILE           = 0x64,
    MT_SETFILTERPROFILE           = 0x64,
    MT_REQGNSSPLATFORM            = 0x76,
    MT_SETGNSSPLATFORM            = 0x76,
    MT_SETGNSSLEVERARM            = 0x68,
    MT_RESETORIENTATION           = 0xA4,
    MT_SETNOROTATION              = 0x22,
    MT_REQUTCTIME                 = 0x60,
    MT_SETUTCTIME                 = 0x60,
    MT_ADJUSTUTCTIME              = 0xA8,
    MT_ICCCOMMAND                 = 0x74,
    MT_SETINITIALHEADING          = 0xD6,
    MT_FORWARDGNSSDATA            = 0xE2,
};

enum XSENS_DEVICE_MODE
{
    MODE_WAKEUP = 0,
    MODE_CONFIG,
    MODE_MEASUREMENT,
};

typedef enum
{
    XSENS_BAUD_4800 = 0x0B,
    XSENS_BAUD_9600 = 0x09,
    XSENS_BAUD_11400 = 0x08,
    XSENS_BAUD_19200 = 0x07,
    XSENS_BAUD_28800 = 0x06,
    XSENS_BAUD_38400 = 0x05,
    XSENS_BAUD_57600 = 0x04,
    XSENS_BAUD_76600 = 0x03,
    XSENS_BAUD_115200 = 0x02,
    XSENS_BAUD_230400 = 0x01,
    XSENS_BAUD_460800 = 0x00,
    XSENS_BAUD_921600 = 0x80,    // two valid bytes exist for this setting
    // XSENS_BAUD_921600 = 0xA0,
    XSENS_BAUD_2000000 = 0x0C,
    XSENS_BAUD_3686400 = 0x0E,
    XSENS_BAUD_4000000 = 0x0D,
} XsensBaudSetting_t;

typedef enum
{
    XSENS_RESET_ORIENTATION_RESERVED = 0x02,

    XSENS_ORIENTATION_STORE = 0x00,
    XSENS_ORIENTATION_HEADING_RESET = 0x01,
    XSENS_ORIENTATION_INCLINATION_RESET = 0x03,
    XSENS_ORIENTATION_ALIGNMENT_RESET = 0x04,
    XSENS_ORIENTATION_HEADING_DEFAULT = 0x05,
    XSENS_ORIENTATION_INCLINATION_DEFAULT = 0x06,
    XSENS_ORIENTATION_ALIGNMENT_DEFAULT = 0x07,
} XsensOrientationSetting_t;

// MData2 Field Identifiers
enum XDA_TYPE_IDENTIFIER
{
    XDI_UTC_TIME           = 0x1010,
    XDI_PACKET_COUNTER     = 0x1020,
    XDI_SAMPLE_TIME_FINE   = 0x1060,
    XDI_SAMPLE_TIME_COARSE = 0x1070,

    XDI_TEMPERATURE = 0x0810,

    XDI_QUATERNION      = 0x2010,
    XDI_ROTATION_MATRIX = 0x2020,
    XDI_EULER_ANGLES    = 0x2030,

    XDI_BARO_PRESSURE = 0x3010,

    XDI_DELTA_V                = 0x4010,
    XDI_ACCELERATION           = 0x4020,
    XDI_FREE_ACCELERATION      = 0x4030,
    XDI_ACCELERATION_HIGH_RATE = 0x4040,

    XDI_ALTITUDE_ELLIPSOID = 0x5020,
    XDI_POSITION_ECEF      = 0x5030,
    XDI_LAT_LON            = 0x5040,
    XDI_GNSS_PVT_DATA      = 0x7010,
    XDI_GNSS_SAT_INFO      = 0x7020,
    XDI_GNSS_PVT_PULSE     = 0x7030,

    XDI_RATE_OF_TURN           = 0x8020,
    XDI_DELTA_Q                = 0x8030,
    XDI_RATE_OF_TURN_HIGH_RATE = 0x8040,

    XDI_RAW_ACC_GYRO_MAG_TEMP = 0xA010,
    XDI_RAW_GYRO_TEMP         = 0xA020,

    XDI_MAGNETIC_FIELD = 0xC020,

    XDI_VELOCITY_XYZ = 0xD010,

    XDI_STATUS_BYTE = 0xE010,
    XDI_STATUS_WORD = 0xE020,
    XDI_DEVICE_ID   = 0xE080,
    XDI_LOCATION_ID = 0xE090,

};

// 32-bit status structure
//   Casting to bitfield is end-users responsibility.
//      union XDI_STATUS32_UNION status;
//      status.word = xsens_coalesce_32BE_32LE(&output->payload[0]);
//      printf("filterOK: %d\n", status.bitfield.filter_valid);
typedef struct
{
    unsigned int self_test : 1;
    unsigned int filter_valid : 1;
    unsigned int gnss_fix : 1;
    unsigned int no_rotation_status : 2;
    unsigned int representative_motion : 1;
    unsigned int clock_bias_estimation : 1;
    unsigned int reserved_0 : 1;
    unsigned int clip_acc_x : 1;
    unsigned int clip_acc_y : 1;
    unsigned int clip_acc_z : 1;
    unsigned int clip_gyro_x : 1;
    unsigned int clip_gyro_y : 1;
    unsigned int clip_gyro_z : 1;
    unsigned int clip_mag_x : 1;
    unsigned int clip_mag_y : 1;
    unsigned int clip_mag_z : 1;
    unsigned int reserved_1 : 2;
    unsigned int clipping : 1;
    unsigned int reserved_2 : 1;
    unsigned int sync_in_mark : 1;
    unsigned int sync_out_mark : 1;
    unsigned int filter_mode : 3;
    unsigned int has_gnss_pulse : 1;
    unsigned int rtk_status : 2;
    unsigned int reserved_3 : 3;
} xdi_status32_t;

union XDI_STATUS32_UNION
{
    uint32_t       word;
    xdi_status32_t bitfield;
};

#ifdef __cplusplus
}
#endif

#endif
// end XSENS_CONSTANTS_H