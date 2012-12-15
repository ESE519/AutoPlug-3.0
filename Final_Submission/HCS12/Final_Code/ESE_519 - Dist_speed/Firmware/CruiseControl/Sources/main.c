#include <hidef.h>      /* common defines and macros */
#include <MC9S12C128.h> /* derivative information */
#pragma LINK_INFO DERIVATIVE "mc9s12c128"
#include <string.h>
#include <math.h>

#include "common.h"
#include "CAN.h"
#include "SCI.h"
#include "types.h"
#include "PLL.h"
#include "stdio.h"
#include "termio.h"


#define KP_NUM (20)
#define KP_DEN (1)
#define KI_NUM (1)
#define KI_DEN (20)

#define KP_BRAKE_NUM (5)
#define KP_BRAKE_DEN (50)
#define KI_BRAKE_NUM (1)
#define KI_BRAKE_DEN (500)

typedef struct _Car
{
    float gearRatio[8];
    float enginerpmRedLine; /* rad/s */
    float SHIFT; /* = 0.95 (% of rpmredline) */
    UINT16 SHIFT_MARGIN;
    float wheelRadius[4];
} Car;



CarInputs carInputs;
CarParams carParams;
CarDistance carDistance;
CarSensor carSensor;

volatile UINT8 carInputsUpdated = 0;
volatile UINT8 carParamsUpdated = 0;
volatile UINT8 brakeParamsUpdated = 0;
volatile UINT8 accelCorrection =  0;
volatile UINT8 accelCorrection1 = 50 ;
volatile float setSpeed = -1.0;
volatile float des_speed = 0.0,temp=0.0,des_speed1 = 0.0;
volatile float time=0.0;
volatile float d1=0,d2=0,range1=0,range2=0,serial=0,deltaX=0;
volatile INT8 flag=0,flag_acc = 0,flagd1=0,flagd2=1,acceleration=0,accel;
volatile INT16 controlOutput;
volatile INT16 controlOutput1;
volatile INT16 error;
volatile INT16 errInteg;
volatile INT8 des_distance = 30;
volatile  INT8 setSpeed1 = -1;
volatile INT32 count1=0;

const Car car = {
    {0, 3.9*4.5, 2.9*4.5, 2.3*4.5, 1.87*4.5, 1.68*4.5, 1.54*4.5, 1.46*4.5},
    1958.26,
    0.95,
    4,
    {0.3024, 0.3024, 0.3151, 0.3151}
};


void init(void);
INT8 getGear(INT8 currentGear);

void main(void)
{

    INT8 carSpeed = 0;


    INT16 error_dist;
    INT16 errInteg_dist;

    INT8 brake;
    INT8 gear = 0;
    
    INT32 count=0;

    UINT8 cruiseOn = 0;

    AccelMsg accelMsg;
    BrakeMsg brake_msg;
    
    init();
    TERMIO_Init();

    DDRB = 0xF0;
    PORTB = 0xF0;
    
    
    EnableInterrupts;

    for(;;)
    {
        
        if(carParamsUpdated)
        {
            if(!cruiseOn)
            {
                if(carInputs.gear != -1)
                    gear = (INT8) limit(getGear(gear), 0, 7);
                else
                    gear = -1;

                accelMsg.accel = (UINT8) limit(carInputs.accel - accelCorrection, 0, 100);
                accelMsg.clutch = carInputs.clutch;
                accelMsg.gear = gear;
            }
            else
            {
              
                if(1) 
                {
                  
                error = ((INT8)setSpeed - carParams.speed);
                errInteg += error;
                errInteg = limit(errInteg, -100*KI_DEN/KI_NUM, 100*KI_DEN/KI_NUM);

                controlOutput = KP_NUM*error/KP_DEN + KI_NUM*errInteg/KI_DEN;
                controlOutput = limit(controlOutput, 0, 100);
                } 
               
                    accel = (UINT8) (controlOutput);

                gear = (UINT8) limit(getGear(gear), 0, 7);

               
                accelMsg.accel = (UINT8)limit(accel - accelCorrection, 0, 50) ;
                accelMsg.gear = gear;
                accelMsg.clutch = 0;
                
               
            }
                    
            CANTx(CAN_ACCEL_MSG_ID, &accelMsg, sizeof(AccelMsg));
            carParamsUpdated = 0;
        }
        else if(carInputsUpdated)
        {
            
           if((carInputs.controls & CRUISE) && (carParams.speed > 0))
            {
                
                if(!cruiseOn)
                {
                   
                    setSpeed = carParams.speed;
                    setSpeed1= carParams.speed;       /*holds the spped at which cruise was pressed*/
                    cruiseOn = 1;
                    errInteg = 0;
                    PORTB = 0x70;
                    flag=0;
                    flagd2=1;
                    flagd1=0;
                    count=0;
                    temp=0;
                    des_speed=0;  
                    
                }
                
                /*if front car within radar range start calculation for ACC implementation */
                if(carDistance.distance<2500)
                {

                    if(flagd2==0)
                    {
                        if(carDistance.distance1>=(d1+2))
                        {
                            count++;
                            d2=(float)carDistance.distance1;
                            time=(d2-d1)/(float)carParams.speed;
                            range2=(float)carDistance.distance;
                            
                            /*Calculating the front car speed*/
                            des_speed1= ((range2-range1)/time)+((float)carParams.speed);
                             temp=(temp+des_speed1);
                            /* calculating the average of calculated front car speeds to minimise error*/ 
                            des_speed=temp/count;
   
                            flagd2=1;
                            flagd1=0;
                            /*Calculate the distance at which acc car should start braking for a fixed decceleration value*/
                            if((count%8)==0)
                            deltaX=((des_speed*des_speed)-(carParams.speed*carParams.speed))/(2*(-6.5));

                            
                        }
                        
                    }
                    
                    if(flagd1==0)
                    {
                        d1=(float)carDistance.distance1;
                        range1=(float)carDistance.distance;
                        flagd1=1;
                        flagd2=0;
                    }

                    /* when the following car is within braking range set its speed to front car speed*/
                    if((carDistance.distance - des_distance) < deltaX) 
                    {
                      
                        setSpeed=des_speed;
                        flag=1;
                    }
                    /* if distance between cars is greater than safe distance, increase following car speed*/ 
                     if((flag)&&(carDistance.distance > (des_distance+2)))
                    {
                        setSpeed= des_speed + ((float)carDistance.distance-(float)des_distance)/6;
                        if(setSpeed>setSpeed1)
                            setSpeed = setSpeed1;

                        
                    } 
                    /* if distance between cars is lesser than safe distance, decrease following car speed*/ 
                    else if(carDistance.distance < (des_distance-2))
                    {
                         setSpeed=des_speed + ((float)carDistance.distance-(float)des_distance)/10;
                          if(setSpeed>setSpeed1)
                            setSpeed = setSpeed1;
  
                    }
                    else if(flag)
                    {
                         setSpeed=des_speed;
                          if(setSpeed>setSpeed1)
                            setSpeed = setSpeed1;
                    }                    
                }
                /*if front car is out of radar range increase following car speed gradually to reach setSpeed1*/
                else if(carDistance.distance == 2500)           
                {                                                                  
                    if(carParams.speed<setSpeed1)
                    {
                        if((count1%2)==0){
                            count1=0;
                            setSpeed = setSpeed + 0.015;
                        }
                        count1++;
                    } 
                    
                    else
                        setSpeed=setSpeed1;
                    
                    flag_acc = 0;
                    flag=0;
                    flagd2=1;
                    flagd1=0;
                   
                } 
                
            }
            else
            {
                flag = 0;
                cruiseOn = 0;
                setSpeed = -1;
                PORTB = 0xF0;
                des_speed=0;
            }
            if(!cruiseOn)
            {
                if(carInputs.gear != -1)
                    gear = (UINT8)limit(getGear(gear), 0, 7);
                else
                    gear = -1;

                accelMsg.accel = (UINT8)limit(carInputs.accel - accelCorrection, 0, 100);
                accelMsg.clutch = carInputs.clutch;
                accelMsg.gear = gear;
                
            }
            carInputsUpdated = 0;
        }

    }
}

/* Compute gear */
INT8 getGear(INT8 currentGear)
{
    float gr_up, omega, wr;
    float rearWheelSpeed;

    if (currentGear <= 0)
        return 1;

    //rearWheelSpeed = (carParams.wheelSpeedRL + carParams.wheelSpeedRR)/2;
    rearWheelSpeed = carParams.speed;

    gr_up = car.gearRatio[currentGear];
    omega = car.enginerpmRedLine/gr_up;
    wr = car.wheelRadius[2];

    if (rearWheelSpeed > omega*wr*car.SHIFT)
    {
        return currentGear + 1;
    }
    else
    {
        float gr_down = car.gearRatio[currentGear - 1];
        omega = car.enginerpmRedLine/gr_down;
        if ( (currentGear > 1) && (rearWheelSpeed + car.SHIFT_MARGIN < omega*wr*car.SHIFT) )
            return currentGear - 1;
    }
    return currentGear;
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED

interrupt 38 void CANRx_vect(void)
{
    UINT16 identifier;
    UINT8 length;

    identifier = (CANRXIDR0 << 3) + (CANRXIDR1 >> 5);

    length = CANRXDLR & 0x0F;

    if(identifier == CAN_INPUT_MSG_ID)
    {
        if(!carInputsUpdated) // Only update when old value has been used
        {
            if(length > sizeof(CarInputs))
                length = sizeof(CarInputs);

            memcpy(&carInputs, &CANRXDSR0, length);
            carInputsUpdated = 1;
        }
    }
    else if (identifier == CAN_PARAM_MSG_ID)
    {
        if(!carParamsUpdated) // Only update when old value has been used
        {
            if(length > sizeof(CarParams))
                length = sizeof(CarParams);

            memcpy(&carParams, &CANRXDSR0, length);
            //params.carParams = carParams;
            carParamsUpdated = 1;
        }
    }
    else if (identifier == CAN_ACCEL_CORR_MSG_ID)
    {
        accelCorrection = CANRXDSR0;
    }
    else if (identifier == CAN_DIST_MSG_ID)
    {
        if(length > sizeof(CarDistance))
        length = sizeof(CarDistance);
        
        memcpy(&carDistance, &CANRXDSR0, length);
        //params.carDistance=carDistance;
        //printf("\n Distance is %d \r\n ", carDistance.distance * 8);
    }
    else if (identifier == CAN_SENSOR_MSG_ID)
    {
        if(length > sizeof(CarSensor))
        length = sizeof(CarSensor);
        
        memcpy(&carSensor, &CANRXDSR0, length);
        
        if(carSensor.safeDistance >0)        
            des_distance = carSensor.safeDistance;
        
        if(carSensor.setSpeed >0) 
        {
            setSpeed=(carSensor.setSpeed*5)/18;
            setSpeed1=(carSensor.setSpeed*5)/18;
        }
    }

    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT


void init()
{
    setclk24();
    CANInit();

}
