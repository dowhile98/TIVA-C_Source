#include "TM4C123.h"

extern uint32_t uwTick;

/*ISR*/
void SysTick_Handler(void){
  if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
    uwTick+= 1;   //se incrementa el valor de la variable en 1
  
  return;
}