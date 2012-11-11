#ifndef _TYPES_H
#define _TYPES_H

#define CAN_INPUT_MSG_ID (0x60) /* Raw driver input */
#define CAN_BRAKE_MSG_ID (0x62) /* Computed brake output */
#define CAN_ACCEL_MSG_ID (0x64) /* Computed accel, gear and clutch values */
#define CAN_PARAM_MSG_ID (0x66) /* Car params from simulator */
#define CAN_ACCEL_CORR_MSG_ID (0x80) /* Accelerator correction from EBCM to ECM for TC */
#define CAN_STAB_MSG_ID (0x84) /* Used for stability control */
#define CAN_DISTANCE_MSG_ID (0x88) /* Car Distance from params */

#define CAN_INPUT_MSG_ID_2 (0x61) /* Raw driver input */
#define CAN_BRAKE_MSG_ID_2 (0x63) /* Computed brake output */
#define CAN_ACCEL_MSG_ID_2 (0x65) /* Computed accel, gear and clutch values */
#define CAN_PARAM_MSG_ID_2 (0x67) /* Car params from simulator */
#define CAN_ACCEL_CORR_MSG_ID_2 (0x81) /* Accelerator correction from EBCM to ECM for TC */
#define CAN_STAB_MSG_ID_2 (0x85) /* Used for stability control */
#define CAN_DISTANCE_MSG_ID_2 (0x89) /* Car Distance from params */

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
    INT8 distance;
    //INT8 lateralspeed;
    //INT8 lateralacc;
} CarParams;

typedef struct _Params
{
    INT8 speed;
    UINT16 engineRPM;
    INT8 wheelSpeedFL;
    INT8 wheelSpeedFR;
    INT8 wheelSpeedRL;
    INT8 wheelSpeedRR;
    INT8 yawRate;
    INT8 distance;
    //INT8 lateralspeed;
    //INT8 lateralacc;
    INT8 speed2;
    UINT16 engineRPM2;
    INT8 wheelSpeedFL2;
    INT8 wheelSpeedFR2;
    INT8 wheelSpeedRL2;
    INT8 wheelSpeedRR2;
    INT8 yawRate2;
    INT8 distance2;
} Params;

typedef struct _StabMsg 
{
    INT8 lateralspeed;
    INT8 lateralacc;
} StabMsg;

#define SERIAL_CAR_PARAMS_PKT_LENGTH (2 + sizeof(Params) + sizeof(StabMsg) + 1)

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
    UINT8 accel2 ;
    UINT8 brakeFL2;
    UINT8 brakeFR2;
    UINT8 brakeRL2;
    UINT8 brakeRR2;
    INT8 steer2;
    INT8 gear2;
    UINT8 clutch2;
} TorcsInput;

typedef struct _CarDistance 
{
   
    INT8 distance;
} CarDistance;
  

#endif
