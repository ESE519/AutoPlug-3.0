/*
 * File: Subsystem.c
 *
 * Code generated for Simulink model 'Subsystem'.
 *
 * Model version                  : 1.1018
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Thu Dec 06 00:11:32 2012
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->HC(S)12
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. ROM efficiency
 *    3. RAM efficiency
 * Validation result: Not run
 */

#include "Subsystem.h"
//#include "rtwtypes.h"
#include "Subsystem_private.h"
#include "types.h"

/* Block signals and states (auto storage) */
D_Work_Subsystem Subsystem_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Subsystem Subsystem_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Subsystem Subsystem_Y;

/* Model step function */
float Subsystem_step(INT8 setSpeed1)
{
  real_T des_speed;
  real_T deltaX;
  Subsystem_U.In1=3;//   distance;
  Subsystem_U.In2=3;//    speed;
  Subsystem_U.In3=3 ;//   distance1;
  Subsystem_U.In4= 3;//   setSpeed1;

  /* MATLAB Function: '<S1>/          ' incorporates:
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   *  Inport: '<Root>/In3'
   *  Inport: '<Root>/In4'
   */
  /* MATLAB Function 'Subsystem/          ': '<S2>:1' */
  /* '<S2>:1:4' des_distance=30; */
  /* '<S2>:1:7' if isempty(setSpeed) */
  /* '<S2>:1:14' if isempty(flag) */
  /* '<S2>:1:19' if isempty(d1) */
  /* '<S2>:1:24' if isempty(range1) */
  if (!Subsystem_DWork.range1_not_empty) {
    /* '<S2>:1:25' range1=distance; */
    Subsystem_DWork.range1 = Subsystem_U.In1;
    Subsystem_DWork.range1_not_empty = TRUE;
  }

  /*  deltaX=0.0; */
  /*  des_speed=0.0; */
  /* '<S2>:1:32' if (flag==1) */
  if (Subsystem_DWork.flag == 1.0) {
    /* '<S2>:1:34' d2=dist1; */
    /* '<S2>:1:35' time=(d2-d1)/speed; */
    /* '<S2>:1:37' range2=distance; */
    /* '<S2>:1:38' des_speed=((range2-range1)/time)+ speed; */
    des_speed = (Subsystem_U.In1 - Subsystem_DWork.range1) / ((Subsystem_U.In3 -
      Subsystem_DWork.d1) / Subsystem_U.In2) + Subsystem_U.In2;

    /* '<S2>:1:39' deltaX=((des_speed*des_speed)-(speed*speed))/(2*(-8.5)); */
    deltaX = (des_speed * des_speed - Subsystem_U.In2 * Subsystem_U.In2) / -17.0;

    /*  braking distance */
    /* '<S2>:1:41' if(((distance - des_distance) <= deltaX) && (distance >= des_distance)) */
    if ((Subsystem_U.In1 - 30.0 <= deltaX) && (Subsystem_U.In1 >= 30.0)) {
      /* '<S2>:1:42' setSpeed = des_speed; */
      Subsystem_DWork.setSpeed = des_speed;
    } else if (Subsystem_U.In1 < 30.0) {
      /* '<S2>:1:43' elseif((distance) < des_distance) */
      /* '<S2>:1:44' setSpeed = des_speed; */
      Subsystem_DWork.setSpeed = des_speed;
    } else {
      if (Subsystem_U.In1 - 30.0 > deltaX) {
        /* '<S2>:1:45' elseif(((distance - des_distance) > deltaX)) */
        /* '<S2>:1:46' setSpeed = setSpeed1; */
        Subsystem_DWork.setSpeed = Subsystem_U.In4;
      }
    }

    /* '<S2>:1:49' d1=dist1; */
    Subsystem_DWork.d1 = Subsystem_U.In3;

    /* '<S2>:1:50' range1=distance; */
    Subsystem_DWork.range1 = Subsystem_U.In1;
  } else {
    if (Subsystem_DWork.flag == 0.0) {
      /* '<S2>:1:54' elseif(flag==0) */
      /*  only for 1st iteration */
      /* '<S2>:1:55' d1=dist1; */
      Subsystem_DWork.d1 = Subsystem_U.In3;

      /* '<S2>:1:56' range1=distance; */
      Subsystem_DWork.range1 = Subsystem_U.In1;

      /* '<S2>:1:57' flag=1; */
      Subsystem_DWork.flag = 1.0;
    }
  }

  /* Outport: '<Root>/Out1' incorporates:
   *  Inport: '<Root>/In2'
   *  MATLAB Function: '<S1>/          '
   */
  /* '<S2>:1:60' diffSpeed=setSpeed-speed; */
  //Subsystem_Y.Out1 = Subsystem_DWork.setSpeed - Subsystem_U.In2;
Subsystem_Y.Out1 = Subsystem_DWork.setSpeed; 
return 1.0; //Subsystem_Y.Out1;
  //diffSpeed=Subsystem_Y.Out1;
}

/* Model initialize function */
void Subsystem_initialize(void)
{
  /* (no initialization code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
