/* PCAN - driver input
 *
 * Transmits driver inputs from the wiimote and sends them on the CAN bus via PCAN
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <wiiuse.h>
#include <libpcan.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>

#define PCAN_DEVICE "/dev/pcanusb0"
#define CAN_BAUD_RATE_REG_VAL 0x001C  // 500kbps

#define MAX_WIIMOTES    1            // Changed 
#define CAN_INPUT_MSG_ID  (0x60)  /* Raw driver input for first car */
#define CAN_INPUT_MSG_ID_2  (0x61)  /* Raw driver input for second car */

#define STABILITY   0x08
#define ABS         0x04
#define TC          0x02
#define CRUISE      0x01

typedef struct {
    uint8_t accel;
    uint8_t brake;
    int8_t steer;
    int8_t gear;
    uint8_t clutch;
    uint8_t controls; /* used for ABS/TRAC/CRUISE  */
} CarInputs;

CarInputs carInputs1  = {0, 0, 0, 0, 0, 0};
CarInputs carInputs2  = {0, 0, 0, 0, 0, 0};

HANDLE initCAN(void);

void handle_event(struct wiimote_t* wm)
{
    static uint8_t controls1   = ABS | TC | STABILITY;
    static uint8_t controls2  = ABS | TC | STABILITY;
    
#if 0
    printf("\n\n--- EVENT [id %i] ---\n", wm->unid);

    /* if a button is pressed, report it */
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_A))		printf("A pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_B))		printf("B pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_UP))		printf("UP pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_DOWN))	printf("DOWN pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT))	printf("LEFT pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_RIGHT))	printf("RIGHT pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_MINUS))	printf("MINUS pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_PLUS))	printf("PLUS pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE))		printf("ONE pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO))		printf("TWO pressed\n");
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_HOME))	printf("HOME pressed\n");
#endif
    /*
     *	Pressing home will tell the wiimote we are interested in movement.
     */
    if (IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_HOME))
    {
        wiiuse_set_orient_threshold(wm, 0.5); // Changed was 0.5 earlier
        wiiuse_motion_sensing(wm, 1);
    }

    if(IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_PLUS)) {
      if(wm->unid == 1)
        controls1 ^= ABS;
      else if(wm->unid == 2)
        controls2 ^= ABS;
       //controls ^= ABS;
    }

    if(IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_MINUS)) {
      if(wm->unid == 1)
        controls1 ^= TC;
      else if(wm->unid == 2)
        controls2 ^= TC;
        //controls ^= TC;
 
    }
    
    if(IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_UP)) {
       if(wm->unid == 1)
        controls1 ^= STABILITY;
      else if(wm->unid == 2)
        controls2 ^= STABILITY;
        //controls ^= STABILITY;
    }
  
    if(IS_JUST_PRESSED(wm, WIIMOTE_BUTTON_A)) {
       // printf("\n Accel button is pressed \n "); 
       if(wm->unid == 1)
        controls1 ^= CRUISE;
      else if(wm->unid == 2)
        controls2 ^= CRUISE;
       //controls ^= CRUISE;
    }

    if (WIIUSE_USING_ACC(wm))
    {
        // obtain the steering angle input from wiimote
        float steer_temp =  wm->orient.pitch;
        if(steer_temp < -90)
            steer_temp = -90;
        else if(steer_temp > 90)
            steer_temp = 90;

        if(steer_temp != 0) {
           if(wm->unid == 1)
             carInputs1.steer = (steer_temp/abs(steer_temp))*pow(steer_temp/90,2)*50;
           else  if(wm->unid == 2)
             carInputs2.steer = (steer_temp/abs(steer_temp))*pow(steer_temp/90,2)*50;
        }
        else {
            if(wm->unid == 1)
              carInputs1.steer = 0;
            else if(wm->unid == 2)
              carInputs2.steer = 0;
        }
        //printf("steer: %d\n", carInputs.steer);
    }

    // obtain acceleration input from WiiMote
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_ONE))
    {
        if(wm->unid == 1)
          carInputs1.accel = 100;
        else if(wm->unid == 2)
          carInputs2.accel = 100;
    }
    else {
        if(wm->unid == 1)
          carInputs1.accel = 0;
        else if(wm->unid == 2)
          carInputs2.accel = 0;
        
    }
    // obtain brake input from WiiMote
    if (IS_PRESSED(wm, WIIMOTE_BUTTON_TWO))
    {
        if(wm->unid == 1) {
          carInputs1.brake = 100;
          controls1 &= ~(CRUISE);
        }
        else if(wm->unid == 2) {
          carInputs2.brake = 100;
          controls2 &= ~(CRUISE);

       }
    }
    else {
       if(wm->unid == 1)
        carInputs1.brake = 0;
       else if(wm->unid == 2)
        carInputs2.brake = 0;
    }

    if (IS_PRESSED(wm, WIIMOTE_BUTTON_LEFT)) {
        if(wm->unid == 1)
          carInputs1.gear = -1;
        else if(wm->unid == 2)
          carInputs2.gear = -1;
    }  
    else {
         if(wm->unid == 1)
          carInputs1.gear = 0;
        else if(wm->unid == 2)
          carInputs2.gear = 0;

    }
    carInputs1.clutch = 0;
    carInputs1.controls = controls1;

    carInputs2.clutch = 0;
    carInputs2.controls = controls2;
}

/**
 *	@brief Callback that handles a disconnection event.
 *
 *	@param wm				Pointer to a wiimote_t structure.
 *
 *	This can happen if the POWER button is pressed, or
 *	if the connection is interrupted.
 */
void handle_disconnect(wiimote* wm) {
    printf("\n\n--- DISCONNECTED [wiimote id %i] ---\n", wm->unid);
}

int main(void)
{
    wiimote *wm;
    wiimote** wiimotes;
    int found, connected;
    HANDLE canHandle;
    TPCANMsg Message;
    int wiimote_led_state;
    int exit = 0;

    canHandle = initCAN();
    if(canHandle == NULL)
    {
        printf("Error opening CAN device!\n");
        return -1;
    }

    wiimotes =  wiiuse_init(MAX_WIIMOTES);

    found = wiiuse_find(wiimotes, MAX_WIIMOTES, 5);
    if (!found)
    {
        printf ("No wiimotes found.\n");
        return 0;
    }

    connected = wiiuse_connect(wiimotes, MAX_WIIMOTES);
    if (connected)
        printf("Connected to %i wiimotes (of %i found).\n", connected, found);
    else
    {
        printf("Failed to connect to any wiimote.\n");
        return 0;
    }

    wm = wiimotes[0];
    wiiuse_status(wm);
    while(wm->event != WIIUSE_STATUS)
    {
        wiiuse_poll(wiimotes, MAX_WIIMOTES);
    }
    printf("Battery level for Wiimote 1: %f%%\n", wm->battery_level*100);

    #if 0
// Added for second Wiimote 
    wm1 = wiimotes[1];
    wiiuse_status(wm1);
    while(wm1->event != WIIUSE_STATUS)
    {
        wiiuse_poll(wiimotes, MAX_WIIMOTES);
    }
    printf("Battery level for Wiimote 2: %f%%\n", wm->battery_level*100);
// Added for second Wiimote 

    #endif

    while (1)
    {
        if(exit)
            break;
        if (wiiuse_poll(wiimotes, MAX_WIIMOTES))
        {
            /*
             *	This happens if something happened on any wiimote.
             *	So go through each one and check if anything happened.
             */
            int i = 0;
            for (; i < MAX_WIIMOTES; ++i)
            {
                switch (wiimotes[i]->event)
                {
                    case WIIUSE_EVENT:
                        /* a generic event occured */
                        handle_event(wiimotes[i]);
                        if(i==0) 
                       {
                       // printf("\n Wiimote 1 sending \n ");
                        Message.ID = CAN_INPUT_MSG_ID;
                        Message.MSGTYPE = MSGTYPE_STANDARD;
                        Message.LEN = 6;
                        Message.DATA[0] = carInputs1.accel;
                        Message.DATA[1] = carInputs1.brake;
                        Message.DATA[2] = carInputs1.steer;
                        Message.DATA[3] = carInputs1.gear;
                        Message.DATA[4] = carInputs1.clutch;
                        Message.DATA[5] = carInputs1.controls;
                        CAN_Write(canHandle,&Message);

                        // Show the status of ABS/TC/Cruise on the LEDs
                        wiimote_led_state = 0;
                        if(carInputs1.controls & ABS)
                            wiimote_led_state |= WIIMOTE_LED_1;
                        if(carInputs1.controls & TC)
                            wiimote_led_state |= WIIMOTE_LED_2;
                        if(carInputs1.controls & STABILITY)
                            wiimote_led_state |= WIIMOTE_LED_3;
                        if(carInputs1.controls & CRUISE)
                            wiimote_led_state |= WIIMOTE_LED_4;
                        wiiuse_set_leds(wiimotes[0], wiimote_led_state);
                        
                       }
                       #if 1
                       else if(i == 1) {
                        //printf("\n Wiimote 2 sending \n ");
                        Message.ID = CAN_INPUT_MSG_ID_2;
                        Message.MSGTYPE = MSGTYPE_STANDARD;
                        Message.LEN = 6;
                        Message.DATA[0] = carInputs2.accel;
                        Message.DATA[1] = carInputs2.brake;
                        Message.DATA[2] = carInputs2.steer;
                        Message.DATA[3] = carInputs2.gear;
                        Message.DATA[4] = carInputs2.clutch;
                        Message.DATA[5] = carInputs2.controls;
                        CAN_Write(canHandle,&Message);

                        // Show the status of ABS/TC/Cruise on the LEDs
                        wiimote_led_state = 0;
                        if(carInputs2.controls & ABS)
                            wiimote_led_state |= WIIMOTE_LED_1;
                        if(carInputs2.controls & TC)
                            wiimote_led_state |= WIIMOTE_LED_2;
                        if(carInputs2.controls & STABILITY)
                            wiimote_led_state |= WIIMOTE_LED_3;
                        if(carInputs2.controls & CRUISE)
                            wiimote_led_state |= WIIMOTE_LED_4;
                        wiiuse_set_leds(wiimotes[1], wiimote_led_state);
                        

                        }
                      #endif
                        break;
                    case WIIUSE_DISCONNECT:
                    case WIIUSE_UNEXPECTED_DISCONNECT:
                        /* the wiimote disconnected */
                        handle_disconnect(wiimotes[i]);
                        exit = 1;
                        break;
                    default:
                        break;
                }
            }
        }
    }
    wiiuse_cleanup(wiimotes, MAX_WIIMOTES);
    return 0;

}

HANDLE initCAN(void)
{
    const char *szDevNode = PCAN_DEVICE;
    uint16_t wBTR0BTR1 = CAN_BAUD_RATE_REG_VAL;
    HANDLE h;

    h = LINUX_CAN_Open(szDevNode, O_RDWR);
    if (!h)
    {
        printf("transmitest: can't open %s\n", szDevNode);
        return NULL;
    }
    // init to a user defined bit rate
    if (wBTR0BTR1)
    {
        errno = CAN_Init(h, wBTR0BTR1, CAN_INIT_TYPE_ST);
        if (errno)
        {
            perror("transmitest: CAN_Init()");
            return NULL;
        }
    }
    return h;
}
