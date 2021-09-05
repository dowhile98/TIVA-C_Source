
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include "Delay.h"
/*MACROS*/

/*VARIABLES GLOBALES*/
uint32_t sysFreq;

/*PROTOTIPO DE FUNCIONES*/


int main(void)
{
  /*delay init*/
  Delay_Init();

  /*gpio init*/
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOF;
  while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R5));
  
  /*PF2 OUTPUT*/
  GPIOF->DIR |= 1U<<1;                  //OUTPUT
  GPIOF->DEN |= 1U<<1;                  //ENABLE DIGITAL FUNCTION
  
  while(1){
    GPIOF->DATA ^=1U<<1;
    delay_ms(30);
  }
}
