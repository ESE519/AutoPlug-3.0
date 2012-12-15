/*
 * File: Subsystem.c
 *
 * Code generated for Simulink model 'Subsystem'.
 *
 * Model version                  : 1.1049
 * Simulink Coder version         : 8.2 (R2012a) 29-Dec-2011
 * TLC version                    : 8.2 (Dec 29 2011)
 * C/C++ source code generated on : Fri Dec 07 20:58:54 2012
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->HC(S)12
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "Subsystem.h"
#include "Subsystem_private.h"

/* Block states (auto storage) */
D_Work_Subsystem Subsystem_DWork;

/* External inputs (root inport signals with auto storage) */
ExternalInputs_Subsystem Subsystem_U;

/* External outputs (root outports fed by signals with auto storage) */
ExternalOutputs_Subsystem Subsystem_Y;

/* Real-time model */
//RT_MODEL_Subsystem Subsystem_M_;
//RT_MODEL_Subsystem *const Subsystem_M = &Subsystem_M_;

/* Model step function */
real32_T Subsystem_step(int8_T speed,int8_T setSpeed1,uint16_T distance ,int8_T des_distance , int8_T frontCarSpeed)   // uint16_T
{
  //int8_T des_distance;
  real_T deltaX;
  Subsystem_U.In1 = speed;
  Subsystem_U.In2 = setSpeed1;
  Subsystem_U.In3 = distance;
  Subsystem_U.In4 = 16;//frontCarSpeed;

  /* MATLAB Function: '<S1>/          ' incorporates:
   *  Inport: '<Root>/In1'
   *  Inport: '<Root>/In2'
   *  Inport: '<Root>/In3'
   *  Inport: '<Root>/In4'
   */
  /* MATLAB Function 'Subsystem/          ': '<S2>:1' */
  /* '<S2>:1:2' timeC=0.02; */
  /* '<S2>:1:3' acc_C = 6.5; */
  /* '<S2>:1:4' des_distance=30; */
  /* '<S2>:1:6' if isempty(setSpeed) */
  /* '<S2>:1:10' minSpeed = 10; */
  /* '<S2>:1:12' deltaX=((frontSpeed*frontSpeed)-(speed*speed))/(2*(-acc_C)); */
  deltaX = (Subsystem_U.In4 * Subsystem_U.In4 - Subsystem_U.In1 * Subsystem_U.In1) / -13.0;

  /* '<S2>:1:15' if(distance<=(des_distance + (deltaX))) */
  if (Subsystem_U.In3 <= des_distance + deltaX) {
    /* '<S2>:1:16' if(distance <= deltaX + des_distance) && (distance  >= des_distance) */
    if ((Subsystem_U.In3 <= deltaX + des_distance) && (Subsystem_U.In3 >= des_distance)) {
      /* '<S2>:1:18' tmpSpeed1 = sqrt((speed * speed) + (2*(-acc_C)*abs((deltaX + des_distance)-distance))); */
      /* '<S2>:1:19' setSpeed = max(frontSpeed,tmpSpeed1); */
      Subsystem_DWork.setSpeed = sqrt(fabs((deltaX + des_distance) - Subsystem_U.In3) *
        -13.0 + Subsystem_U.In1 * Subsystem_U.In1);
      if ((Subsystem_U.In4 >= Subsystem_DWork.setSpeed) || rtIsNaN
          (Subsystem_DWork.setSpeed)) {
        Subsystem_DWork.setSpeed = Subsystem_U.In4;
      }

      /*  stops the reduction at front_speed. */
    } else {
      if (Subsystem_U.In3 < des_distance) {
        /* '<S2>:1:22' elseif(distance <des_distance) */
        /* '<S2>:1:23' tmpSpeed2 = sqrt((speed * speed) + (2*(-acc_C)*abs(des_distance-distance))); */
        /* '<S2>:1:24' setSpeed = max(minSpeed,tmpSpeed2); */
        Subsystem_DWork.setSpeed = sqrt(fabs(des_distance - Subsystem_U.In3) * -13.0 +
          Subsystem_U.In1 * Subsystem_U.In1);
        if ((10.0 >= Subsystem_DWork.setSpeed) || rtIsNaN
            (Subsystem_DWork.setSpeed)) {
          Subsystem_DWork.setSpeed = 10.0;
        }
      }
    }
  } else {
    if (Subsystem_U.In3 > des_distance + deltaX) {
      /* '<S2>:1:27' elseif(distance>(des_distance + (deltaX))) */
      /* '<S2>:1:28' setSpeed = speed + (acc_C*timeC); */
      Subsystem_DWork.setSpeed = Subsystem_U.In1 + 0.1300000000000002 ;
      //0.1300000000000002;// 1.7000000000000002;

      /*  v = u + at */
      /* setSpeed = min(setSpeed1,tmpSpeed3); */
    }
  }

  /* '<S2>:1:32' setSpeed = min(setSpeed1,setSpeed); */
  if ((Subsystem_U.In2 <= Subsystem_DWork.setSpeed) || rtIsNaN
      (Subsystem_DWork.setSpeed)) {
    Subsystem_DWork.setSpeed = Subsystem_U.In2;
  }

  /* Outport: '<Root>/Out1' incorporates:
   *  Inport: '<Root>/In1'
   *  MATLAB Function: '<S1>/          '
   */
  /* '<S2>:1:33' diffSpeed = setSpeed - speed; */
  Subsystem_Y.Out1 = Subsystem_DWork.setSpeed;
  return (real32_T)Subsystem_Y.Out1;
}

/* Model initialize function */
void Subsystem_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));
}

/* Model terminate function */
void Subsystem_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
