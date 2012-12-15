#ifndef _TYPES_H
#define _TYPES_H

#include "common.h" 

#define CAN_INPUT_MSG_ID (0x60) /* Raw driver input */
#define CAN_BRAKE_MSG_ID (0x62) /* Computed brake output */
#define CAN_ACCEL_MSG_ID (0x64) /* Computed accel, gear and clutch values */
#define CAN_PARAM_MSG_ID (0x66) /* Car params from simulator */
#define CAN_ACCEL_CORR_MSG_ID (0x80) /* Accelerator correction from EBCM to ECM for TC */
#define CAN_STAB_MSG_ID (0x84) /* Used for stability control */
#define CAN_DIST_MSG_ID (0x88) /* Car Distance from params */
#define CAN_SENSOR_MSG_ID (0x90) /* Radar range of view */
#define CAN_POSITION_MSG_ID (0x92) /* Position of cars on the track */


#define STABILITY (0x08)
#define ABS       (0x04)
#define TRACTION  (0x02)
#define CRUISE    (0x01)

typedef struct _CarParams
{
    INT8 speed;
    UINT16 engineRPM;
    INT8 wheelSpeedFL;
    INT8 wheelSpeedFR;
    INT8 wheelSpeedRL;
    INT8 wheelSpeedRR;
    INT8 yawRate;
    //UINT16 dist;
    //UINT16 dist1;
    //INT8 lateralspeed;
    //INT8 lateralacc;
} CarParams;  

typedef struct _StabMsg 
{
    INT8 lateralspeed;
    INT8 lateralacc;
} StabMsg;

#define SERIAL_CAR_PARAMS_PKT_LENGTH (2 + sizeof(CarParams) + sizeof(StabMsg) + 1)

typedef struct _CarInputs {
    UINT8 accel;
    UINT8 brake;
    INT8 steer;
    INT8 gear;
    UINT8 clutch;
    UINT8 controls; /* use it for ABS/TRAC/CRUISE  */
} CarInputs;
#define SERIAL_CAR_INPUTS_PKT_LENGTH (2 + sizeof(CarInputs) + 1)

typedef struct _AccelMsg
{
    UINT8 accel;
    INT8 gear;
    UINT8 clutch;
} AccelMsg;

typedef struct _BrakeMsg
{
    UINT8 brakeFL;
    UINT8 brakeFR;
    UINT8 brakeRL;
    UINT8 brakeRR;
} BrakeMsg;

typedef struct _TorcsInput
{
    UINT8 accel;
    UINT8 brakeFL;
    UINT8 brakeFR;
    UINT8 brakeRL;
    UINT8 brakeRR;
    INT8 steer;
    INT8 gear;
    UINT8 clutch;
} TorcsInput;
typedef struct _CarDistance 
{
    UINT16 distance;
    UINT16 distance1;
}CarDistance;

typedef struct _CarInputs1 {
    UINT8 accel;
    UINT8 brake;
    INT8  steer;
    INT8  gear;
    INT8  safeDistance;
    INT8  radarAngle;
    UINT8 controls; /* use it for ABS/TRAC/CRUISE  */
 
}CarInputs1;

typedef struct _CarSensor 
{
    UINT8 safeDistance;
    UINT8 radarAngle;
    
}CarSensor;

typedef struct _CarPosition 
{
    UINT8 X1pos;
    UINT8 Y1pos;
    UINT8 X2pos;
    UINT8 Y2pos;
}CarPosition;
  

#endif
