
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS

/*MACROS*/

/*VARIABLES GLOBALES*/


/*PROTOTIPO DE FUNCIONES*/
/**
  * @fn delay_ms
  * @brief funcion que genera retardos en milisegundos
  * @param time_ms: retardo en milisegundos deseado
  */
void delay_ms(uint32_t time_ms);


int main(void)
{
  /*enable clock gpiof*/
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOF;
  while(!(SYSCTL->PRGPIO & 1U<<5));
  //PF1 -> salida
  GPIOF->DIR |= 1U<<3;
  GPIOF->DEN |= 1U<<3;
  
  
  while(1){
    GPIOF->DATA ^= 1U<<3;
    delay_ms(500);
  }
}

/**
  * @fn delay_ms
  * @brief funcion que genera retardos en milisegundos
  * @param time_ms: retardo en milisegundos deseado
  */
void delay_ms(uint32_t time_ms){

  /*1. enable = 0*/
  SysTick->CTRL = 0;
  /*2. configura el valor del registro RELOAD*/
  SysTick->LOAD = SystemCoreClock / 1000 - 1;
  /*3. poner el registro current a cero*/
  SysTick->VAL = 0;
  /*4. configurar la fuente de reloj y habilitar el conteo*/
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
 
  /*retardo*/
  while(time_ms>0){
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));//retardo de 1ms
    time_ms--;
  }
}
