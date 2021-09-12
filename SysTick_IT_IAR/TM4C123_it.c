/**
  * @brief manejo de todas las interrupciones
  */
#include "TM4C123.h"
#include "inc/tm4c123gh6pm.h"
   
extern uint32_t uwTick;    
/**
  * @fn SysTick_Handler
  * @brief ISR de la systick
  */
void SysTick_Handler(void){
  if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
    uwTick+= 1;   //se incrementa el valor de la variable en 1
  }
  return;
}   