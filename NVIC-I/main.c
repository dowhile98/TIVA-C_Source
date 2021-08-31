/**
  *  @brief
  * UART3_IRQn = 75
  */
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
  /**********************CMSIS***************************/
  uint8_t bit, reg;
  /*it enable for UART3*/
  reg = UART3_IRQn >> 5U;
  bit = UART3_IRQn & 0x1FU;
  NVIC->ISER[reg] |= 1U<<bit;   //Habilita la interrupcion
  NVIC->IP[UART3_IRQn] = 3U<<5; //prioridad 3 para el  uart3
  NVIC->STIR = UART3_IRQn;
  //GPIOE
  NVIC_EnableIRQ(GPIOE_IRQn);   //habilita la interrupcion
  NVIC_SetPriority(GPIOE_IRQn,7);//prioridad de 7
  /*********************TIVA WARE*********************/
  //UART2 
  NVIC_PRI8_R &=~ 0x7U<<13;     //CLEAR BITS 13,14,15
  NVIC_PRI8_R |= 2U<<13;        //uart2 prioridad 2
  NVIC_EN1_R |= 1u<<1;          //se habilita la interruptcion del uart2
  NVIC_SW_TRIG_R = UART2_IRQn;
  while(1){
    
  }
}
