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
#include "math.h"
#include "rtwtypes.h"
#include "Subsystem.h"  

               /* Model's header file */
                  /* MathWorks types */
#include "Subsystem_private.h"

#define KP_NUM (5)
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

/*typedef struct _Allparams{
        CarParams carParams;
        BrakeMsg Brakeparams;
        CarDistance carDistance;
}Allparams;
        
 volatile Allparams params; 
 */

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
volatile INT8 frontCarSpeed = 17;
volatile INT8 front_car_count = 0;
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
    //INT8 setSpeed = -1;
    //INT8 setSpeed1 = -1;
    INT8 carSpeed = 0;

    //INT16 error;
    //INT16 errInteg;
    INT16 error_dist;
    INT16 errInteg_dist;

    INT8 brake;
    INT8 gear = 0;
    
    //INT8 des_distance = 30;

     INT8 nicola_speed;

    INT32 count=0;

    UINT8 cruiseOn = 0;

    AccelMsg accelMsg;
    BrakeMsg brake_msg;
    
    init();
    TERMIO_Init();

    DDRB = 0xF0;
    PORTB = 0xF0;
    
    Subsystem_initialize();
	
    EnableInterrupts;

    for(;;)
    {
        //printf("Distance = %d",carDistance.distance);
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
                if((flag==1)&&(carDistance.distance > (des_distance + 5))) 
                {
                   //setSpeed += 0.1;
                  // errInteg=0;
                }
                if(1) 
                {
                  
                error = ((INT8)setSpeed - carParams.speed);
                errInteg += error;
                errInteg = limit(errInteg, -100*KI_DEN/KI_NUM, 100*KI_DEN/KI_NUM);

                controlOutput = KP_NUM*error/KP_DEN + KI_NUM*errInteg/KI_DEN;
                controlOutput1 = KP_NUM*error/KP_DEN + KI_NUM*errInteg/KI_DEN;
                controlOutput = limit(controlOutput, 0, 100);
                controlOutput1 = limit(abs(controlOutput1), 0, 100);
                } 
                //if((carDistance.distance > (des_distance))&&(carDistance.distance < (des_distance+20)))
                /*if(0)
                {
                  
                error_dist = (carDistance.distance-des_distance);
                errInteg_dist += error_dist;
                errInteg_dist = limit(errInteg_dist, -100*KI_BRAKE_DEN/KI_BRAKE_NUM, 100*KI_BRAKE_DEN/KI_BRAKE_NUM);
                
                controlOutput1 = KP_BRAKE_NUM*error_dist/KP_BRAKE_DEN + KI_BRAKE_NUM*errInteg_dist/KI_BRAKE_DEN;
                controlOutput1 = limit(controlOutput1, 0, 100);
                } */
                
                /*if(carDistance.distance==2500)
                controlOutput1=0;
                */
                
                //if(carDistance.distance > des_distance )
                  // accel = (UINT8) (controlOutput + (carDistance.distance-des_distance)/5);
                //else
                    accel = (UINT8) (controlOutput);

                gear = (UINT8) limit(getGear(gear), 0, 7);

                //accelMsg.accel = (UINT8)limit(accel - accelCorrection, 0, 50) ;
                accelMsg.accel = (UINT8)limit(accel - accelCorrection, 0, 50) ;
                accelMsg.gear = gear;
                accelMsg.clutch = 0;
                
                
               /* controlOutput1 = KP_BRAKE_NUM*error/KP_BRAKE_DEN + KI_BRAKE_NUM*errInteg/KI_BRAKE_DEN;
                controlOutput1 = limit(controlOutput1, 0, 100);

                /*brake = (UINT8) controlOutput1; 
                //For braking in ACC
                brake_msg.brakeFL = (UINT8)limit(brake - 30, 0, 100) ;
                brake_msg.brakeFR = (UINT8)limit(brake - 30, 0, 100) ;
                brake_msg.brakeRL = (UINT8)limit(brake - 30, 0, 100) ;
                brake_msg.brakeRR = (UINT8)limit(brake - 30, 0, 100) ;*/ 
                
                
                if(( carParams.speed > setSpeed)&&(carDistance.distance1<25))           
                 {
                    //CANTx(CAN_ACCEL_CORR_MSG_ID,&accel_corr,sizeof(AccelMsg));
                    CANTx(CAN_BRAKE_MSG_ID,&brake_msg,sizeof(BrakeMsg));
                 } 
                /*if((carParams.speed == (INT8)setSpeed)&&(carDistance.distance>des_distance))
                {
                     accelMsg.accel = 40 ;
                     accelMsg.gear = gear;
                     accelMsg.clutch = 0;
                } */
               
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
                    setSpeed1= carParams.speed;
                    cruiseOn = 1;
                    errInteg = 0;
                    errInteg_dist = 0;
                    PORTB = 0x70;
                    flag=0;
                    flagd2=1;
                    flagd1=0;
                    count=0;
                    temp=0;
                    des_speed=0;
                   // deltaX=((0)-(carParams.speed*carParams.speed))/(2*(-8.5));  
                    
                }
                
               
                /*if(carDistance.distance < (des_distance + 4))             //When distance between the two cars goes below the 
                {                                                   //desired limit, ACC kicks in by calculating the new speed
                    timegap=(des_distance*8)/carParams.speed;         //from the desired timegap inorder to maintain clearance
                    des_speed=((carDistance.distance-4)*8)/timegap;
                    //printf("des distance = %d",des_speed);
                    if(des_speed<carParams.speed)
                    setSpeed = des_speed;
                    
                    flag=1;
                }*/
                /*if(flagd2==0)
                    {
                        if(carDistance.distance1>=(d1+5))
                        {
                            count++;
                            d2=carDistance.distance1;
                            time=(d2-d1)/(float)carParams.speed;
                            range2=carDistance.distance;
                            des_speed= ((range2-range1)/time)+(carParams.speed);
                            temp=(temp+des_speed);
                            des_speed=temp/count;
                            deltaX=(des_speed-carParams.speed)/(2*acceleration);
                            
                            flagd2=1;
                            flagd1=0;
                        }
                    }
                if(flagd1==0)
                    {
                        d1=carDistance.distance1;
                        range1=carDistance.distance;
                        flagd1=1;
                        flagd2=0;
                    }
                 */
                //if(carDistance.distance<2500 && carDistance.distance > des_distance) 
                
                    
			setSpeed = Subsystem_step(carParams.speed,setSpeed1,carDistance.distance,des_distance , frontCarSpeed);
			//	setSpeed = Subsystem_step(10,setSpeed1,carDistance.distance);
              /* if(carDistance.distance<2500)
                {
                  
                //deltaX=((16*16)-(carParams.speed*carParams.speed))/(2*(-6.5));
                //if(carDistance.distance < (des_distance+20 ))
                //if((carDistance.distance - des_distance) < deltaX)            
                 //When distance between the two cars goes below the 
                //{ 
                    //setSpeed = des_speed;
                    if(flagd2==0)
                    {
                        if(carDistance.distance1>=(d1+2))
                        {
                            count++;
                            d2=(float)carDistance.distance1;
                            time=(d2-d1)/(float)carParams.speed;
                            range2=(float)carDistance.distance;
                            des_speed1= ((range2-range1)/time)+((float)carParams.speed);
                             temp=(temp+des_speed1);
                            des_speed=temp/count;
                            //if(count<2)
                             //   des_speed = des_speed1;
                            //else
                             //   des_speed = 0.999*temp + 0.001*des_speed1;  
                                
                            flagd2=1;
                            flagd1=0;
                            if((count%8)==0)
                            deltaX=((des_speed*des_speed)-(carParams.speed*carParams.speed))/(2*(-7));
                            //deltaX=((22.5*22.5)-(carParams.speed*carParams.speed))/(2*(-6.5));
                            //temp=des_speed;
                            
                        }
                        
                    }
                    
                    if(flagd1==0)
                    {
                        d1=(float)carDistance.distance1;
                        range1=(float)carDistance.distance;
                        flagd1=1;
                        flagd2=0;
                    }

                    
                    if((carDistance.distance - des_distance) < deltaX) 
                    {
                        setSpeed=des_speed;
                        //setSpeed=22.5;
                        flag=1;
                    } 
                    else if((flag)&&(carDistance.distance > (des_distance+5)))
                    {
                        setSpeed= des_speed + ((float)carDistance.distance-(float)des_distance)/10;
                        //if(setSpeed>setSpeed1)
                         //   setSpeed=setSpeed1;
                        //else
                         //   setSpeed= setSpeed; 
                        
                    } 
                    else if(flag)
                    {
                         setSpeed=des_speed;
                    } 
                    else if(carDistance.distance < (des_distance-2))
                    {
                         setSpeed=des_speed - ((float)carDistance.distance-(float)des_distance)/10;
                    }
                        
                    
                }
                else if(carDistance.distance == 2500)           //if car in front moves off go back to the speed when
                {   
                                                               //cruise control was pressed
                    //setSpeed = setSpeed1;
                    if(carParams.speed<setSpeed1)
                    {
                        if((count1%20)==0){
                            count1=0;
                            setSpeed = setSpeed + 0.15;
                        }
                        count1++;
                    } 
                    
                    else
                        setSpeed=setSpeed1;
                    
                    flag_acc = 0;
                    flag=0;
                    flagd2=1;
                    flagd1=0;
					//errInteg = 0;
                    //count=0;
                    //des_speed=0;
                    //temp=0;
                   
                } */
                
            }
            else
            {
                flag = 0;
                cruiseOn = 0;
                setSpeed = -1;
                PORTB = 0xF0;
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
        /*if(brakeParamsUpdated)
        {
            SCITxPkt(&params, sizeof(Allparams));
            carParamsUpdated = 0;
            brakeParamsUpdated = 0;
            serial=1;
            //for(i = 0; i < 1000; i++);
        } */

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
    int temp;

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
        des_distance = carSensor.safeDistance;
          
        if(carSensor.speed2 > 0)   
          frontCarSpeed = (carSensor.speed2 * 5)/18;

        
    }

    CANRFLG_RXF = 1; // Reset the flag
}

#pragma CODE_SEG DEFAULT


void init()
{
    setclk24();
    CANInit();

}
