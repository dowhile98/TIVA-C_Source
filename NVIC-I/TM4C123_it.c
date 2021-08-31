/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"  

void UART3_Handler(void){
  NVIC->STIR = GPIOE_IRQn;
  return;
}

void GPIOE_Handler(void){
  
  return;
}
  
void UART2_Handler(void){
  
  return;
}