#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include <stdio.h>
extern uint16_t data;


void ADC0SS0_Handler(void){
  if(ADC0->RIS & 1<<0){
    ADC0->ISC |= 1<<0;
    data = ADC0->SSFIFO0;               //LECTURA
    printf("valor leido->%d\r\n",data);
    GPIOF->DATA ^= 1U<<1;
  }
  return;
}