/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS

void TIMER3A_Handler(void){
  if(TIMER3->RIS & TIMER_RIS_TATORIS){
    TIMER3->ICR |= TIMER_ICR_TATOCINT;
    //CODIGO
    GPIOF->DATA ^= 1U<<1;
  }
}
