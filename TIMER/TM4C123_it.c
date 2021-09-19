/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS


extern uint32_t pulses;

/*50ms*/
void TIMER3A_Handler(void){
  if(TIMER3->RIS & TIMER_RIS_TATORIS){
    TIMER3->ICR |= TIMER_ICR_TATOCINT;
    //CODIGO
    GPIOF->DATA ^= 1U<<1;               //PF1-> PB4
    pulses = TIMER1->TAR;
    //TIMER1->TAV = 0;
  }
}


/**
  * @fn TIMER1A ISR
  */
void TIMER1A_Handler(void){
  if(TIMER1->RIS & TIMER_RIS_CAMRIS){
    TIMER1->ICR |= TIMER_ICR_CAMCINT;   //clear flag
    /*codigo*/
    GPIOF->DATA ^= 1U<<2;
  }
  return;
}
