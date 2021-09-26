/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h" 
#include <stdio.h>

/***********EXTERN DEFINITION*********/
extern int data;
extern uint32_t uwTick;    

/*************ISR**********************/
void UART0_Handler(void){
  if(UART0->RIS & UART_RIS_RXRIS){
    UART0->ICR |= UART_ICR_RXIC;
    /*CODIGO*/
    data = UART0->DR;
    printf("se recibio->%c\r\n",data);
  }
  return;
}

void UART3_Handler(void){
  if(UART3->RIS & UART_RIS_RXRIS){
    UART3->ICR |= UART_ICR_RXIC;
    /*CODIGO*/
    
  }
  return;
}


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