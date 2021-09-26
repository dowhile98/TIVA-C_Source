
#include "UART.h"

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
/******************************************************************************/
void UART3_Config(uint32_t UARTSysClk,uint32_t BaudRate){
  /*1. habilitar el reloj*/
  SYSCTL->RCGCUART |= SYSCTL_RCGCUART_R3;
  while(!(SYSCTL->PRUART & 1U<<3));
  /*2. Habilitar el reloj para el puerto GPIO que contenga los pines Rx y Tx*/
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOC;
  while(!(SYSCTL->PRGPIO & 1u<<2));
  /*3. Configurar los pines Tx y Rx*/
  //PC6 - PC7
  GPIOC->AFSEL |= 1<<7 | 1<<6;;
  GPIOC->PCTL |= GPIO_PCTL_PC7_U3TX | GPIO_PCTL_PC6_U3RX;
  GPIOC->DEN |= 1<<7 | 1<<6;
  /*deshabilitar el uart*/
  UART3->CTL &=~ UART_CTL_UARTEN;
  /*5. Configurar los registros de generacion de baudios*/
  UART3->IBRD = __GET_IBRD(UARTSysClk,16,BaudRate);
  UART3->FBRD = __GET_FBRD(UARTSysClk,16,BaudRate);
  /*6. Configurar el registro UARTLCRH*/
  UART3->LCRH &=~ UART_LCRH_WLEN_M;
  UART3->LCRH |= UART_LCRH_WLEN_8;      //8-bit data
  /*7. configurar la fuente de reloj para la generacion de baudios*/
  UART3->CC &=~ 0xFU;                   //system clock
  /*8. configurar la generacion de interrupciones*/
  //UART3->IM |= UART_IM_RXIM;
  //UART3->ICR |= UART_ICR_RXIC;
  //NVIC_SetPriority(UART3_IRQn,1);
  //NVIC_EnableIRQ(UART0_IRQn);
  /*9. habilitar el uart*/
  UART3->CTL |= UART_CTL_UARTEN;
  return;
}





/*********************************************************************
 * @fn      		  - UART_ReceiveData
 *
 * @brief             - recibe una serie de bytes
 *
 * @param[in]         - UARTx : Uart por el cual se desea recibir los datos
 * @param[in]         - pRxBuffer: Buffer donde se almacenan los datos recibidos
 * @param[in]         - Len: Cantidad de bytes recibidos
 *
 * @return            - none
 *
 * @Note              -

 */

void UART_ReceiveData(UART0_Type *UARTx, uint8_t *pRxBuffer, uint32_t Len){
  
  while(Len>0){
    
    while(UARTx->FR & UART_FR_RXFE);     //Wait
    
    //This is 8bit data transfer
    *pRxBuffer = (uint8_t) (UARTx->DR  & (uint8_t)0xFF);
    //Now , increment the pRxBuffer
    pRxBuffer++;
    Len--;
  }
  return;
}

void UART_ReceiveData(UART0_Type *UARTx, uint8_t *pRxBuffer, uint32_t Len);
/*********************************************************************
 * @fn      		  - UART_SendData
 *
 * @brief             - enviar bytes por el UARTx
 *
 * @param[in]         - UARTx: Uart por donde se desea enviar los bytes
 * @param[in]         - pTxBuffer: Buffer a enviar
 * @param[in]         - Len: Cantidad de bytes
 *
 * @return            - none
 *
 * @Note              - Resolve all the TODOs

 */
void UART_SendData(UART0_Type *UARTx, uint8_t *pTxBuffer, uint32_t Len){
  
  while(Len>0){
    while(!(UARTx->FR & UART_FR_TXFE));   //wait for fifo empty
    
    UARTx->DR =  *pTxBuffer  ;
    //Implement the code to increment the buffer address
    while(UARTx->FR & UART_FR_BUSY);   //Wait while UART Busy
    /*inclement pointer*/
    pTxBuffer++;
    /*Decrement len*/
    Len--;
  }
  
  
  return;
}
