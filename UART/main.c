
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include <stdio.h>
/*MACROS*/
#define __GET_IBRD(UARTSysClk, ClkDiv, BaudRate)      ((uint16_t)(UARTSysClk/(ClkDiv*BaudRate)))
#define __GET_FRACPART(UARTSysClk, ClkDiv, BaudRate)  ((float)UARTSysClk/(ClkDiv*BaudRate) - __GET_IBRD(UARTSysClk, ClkDiv, BaudRate) )
#define __GET_FBRD(UARTSysClk, ClkDiv, BaudRate)      ((uint8_t)(  __GET_FRACPART(UARTSysClk, ClkDiv, BaudRate) * 64 + 0.5  ))

/*VARIABLES GLOBALES*/
int data;

/*PROTOTIPO DE FUNCIONES*/
/**
  * @fn UART0_Config
  * @brief configura el uart0
  * @param UARTSysClk: la frecuencia para la generacion de baudios
  * @param BaudRate: taza de baudios deseado
  */
void UART0_Config(uint32_t UARTSysClk,uint32_t BaudRate);

/**
  * @brief funcion para transmitir un byte
  */
void UART0_SendByte(uint8_t ch);

/**
  * @brief funcion para recibir un byte
  */
uint8_t UART0_GetByte(void);



int main(void)
{
  UART0_Config(SystemCoreClock,115200);
  printf("CONFIGURACION EXITOSA\r\n");

  while(1){
    //printf("Ingrese un numero entero->");
    //scanf("%d",&data);
    //printf("\nse ingreso->%d\r\n",data);
  }
}


/**
  * @fn UART0_Config
  * @brief configura el uart0
  * @param UARTSysClk: la frecuencia para la generacion de baudios
  * @param BaudRate: taza de baudios deseado
  */

void UART0_Config(uint32_t UARTSysClk,uint32_t BaudRate){
  /*1. habilitar el reloj*/
  SYSCTL->RCGCUART |= SYSCTL_RCGCUART_R0;
  while(!(SYSCTL->PRUART & 1U));
  /*2. Habilitar el reloj para el puerto GPIO que contenga los pines Rx y Tx*/
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOA;
  while(!(SYSCTL->PRGPIO & 1u));
  /*3. Configurar los pines Tx y Rx*/
  GPIOA->AFSEL |= 1U<<1 | 1U;
  GPIOA->PCTL &=~ (0xF<<4 | 0xF);       //reset
  GPIOA->PCTL |= GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;
  GPIOA->DEN |= 1U<<1 | 1U;
  /*deshabilitar el uart*/
  UART0->CTL &=~ UART_CTL_UARTEN;
  /*5. Configurar los registros de generacion de baudios*/
  UART0->IBRD = __GET_IBRD(UARTSysClk,16,BaudRate);
  UART0->FBRD = __GET_FBRD(UARTSysClk,16,BaudRate);
  /*6. Configurar el registro UARTLCRH*/
  UART0->LCRH &=~ UART_LCRH_WLEN_M;
  UART0->LCRH |= UART_LCRH_WLEN_8;      //8-bit data
  /*7. configurar la fuente de reloj para la generacion de baudios*/
  UART0->CC &=~ 0xFU;                   //system clock
  /*8. configurar la generacion de interrupciones*/
  UART0->IM |= UART_IM_RXIM;
  UART0->ICR |= UART_ICR_RXIC;
  NVIC_SetPriority(UART0_IRQn,1);
  NVIC_EnableIRQ(UART0_IRQn);
  /*9. habilitar el uart*/
  UART0->CTL |= UART_CTL_UARTEN;
  return;
}

/**
  * @brief funcion para transmitir un byte
  */
void UART0_SendByte(uint8_t ch){
  while(!(UART0->FR & UART_FR_TXFE));   //wait for fifo empty
  UART0->DR = ch;
  return;
}

/**
  * @brief funcion para recibir un byte
  */
uint8_t UART0_GetByte(void){
  while(UART0->FR & UART_FR_RXFE);     //Wait
  return UART0->DR;
}

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