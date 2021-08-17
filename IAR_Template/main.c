
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS

/*MACROS*/

/*VARIABLES GLOBALES*/
uint32_t sysFreq;

/*PROTOTIPO DE FUNCIONES*/


int main(void)
{
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOF;
  
  sysFreq = SystemCoreClock;
  
  while(1){
    
  }
}
