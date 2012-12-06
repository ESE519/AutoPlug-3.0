#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

volatile double j;

void main(void) {
  /* put your own code here */
  


	EnableInterrupts;


  for(;;) { j=.980*j+1.098;
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
