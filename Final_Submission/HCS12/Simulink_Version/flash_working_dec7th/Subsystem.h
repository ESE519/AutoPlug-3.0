/*
 * File: Subsystem.h
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

#ifndef RTW_HEADER_Subsystem_h_
#define RTW_HEADER_Subsystem_h_
#ifndef Subsystem_COMMON_INCLUDES_
# define Subsystem_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* Subsystem_COMMON_INCLUDES_ */

#include "Subsystem_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

/* Block signals and states (auto storage) for system '<Root>' */
typedef struct {
  real_T setSpeed;                     /* '<S1>/          ' */
  real_T flag;                         /* '<S1>/          ' */
  real_T d1;                           /* '<S1>/          ' */
  real_T range1;                       /* '<S1>/          ' */
  boolean_T range1_not_empty;          /* '<S1>/          ' */
} D_Work_Subsystem;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  real_T In1;                          /* '<Root>/In1' */
  real_T In2;                          /* '<Root>/In2' */
  real_T In3;                          /* '<Root>/In3' */
  real_T In4;                          /* '<Root>/In4' */
} ExternalInputs_Subsystem;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  real_T Out1;                         /* '<Root>/Out1' */
} ExternalOutputs_Subsystem;

/* Block signals and states (auto storage) */
extern D_Work_Subsystem Subsystem_DWork;

/* External inputs (root inport signals with auto storage) */
extern ExternalInputs_Subsystem Subsystem_U;

/* External outputs (root outports fed by signals with auto storage) */
extern ExternalOutputs_Subsystem Subsystem_Y;

/* Model entry point functions */
//extern 
void Subsystem_initialize(void);
//extern
 real32_T Subsystem_step(int8_T,int8_T,uint16_T);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('ACC_Model_backup/Subsystem')    - opens subsystem ACC_Model_backup/Subsystem
 * hilite_system('ACC_Model_backup/Subsystem/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ACC_Model_backup'
 * '<S1>'   : 'ACC_Model_backup/Subsystem'
 * '<S2>'   : 'ACC_Model_backup/Subsystem/          '
 */
#endif                                 /* RTW_HEADER_Subsystem_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
