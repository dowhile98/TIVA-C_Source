/**
  * @autor QUINO B. JEFFRY
  * @version v1
  */

#ifndef __UART_H
#define __UART_H

/*includes*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include <stdio.h>
/*macro defines*/
#define __GET_IBRD(UARTSysClk, ClkDiv, BaudRate)      ((uint16_t)(UARTSysClk/(ClkDiv*BaudRate)))
#define __GET_FRACPART(UARTSysClk, ClkDiv, BaudRate)  ((float)UARTSysClk/(ClkDiv*BaudRate) - __GET_IBRD(UARTSysClk, ClkDiv, BaudRate) )
#define __GET_FBRD(UARTSysClk, ClkDiv, BaudRate)      ((uint8_t)(  __GET_FRACPART(UARTSysClk, ClkDiv, BaudRate) * 64 + 0.5  ))

/*function prototype*/
/**
  * @fn UART0_Config
  * @brief configura el uart0
  * @param UARTSysClk: la frecuencia para la generacion de baudios
  * @param BaudRate: taza de baudios deseado
  */
void UART0_Config(uint32_t UARTSysClk,uint32_t BaudRate);
/**
  * @fn UART3_Config
  * @brief configura el uart3
  * @param UARTSysClk: la frecuencia para la generacion de baudios
  * @param BaudRate: taza de baudios deseado
  */
void UART3_Config(uint32_t UARTSysClk,uint32_t BaudRate);
/**
  * @brief funcion para transmitir un byte
  */
void UART0_SendByte(uint8_t ch);

/**
  * @brief funcion para recibir un byte
  */
uint8_t UART0_GetByte(void);

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
void UART_SendData(UART0_Type *UARTx, uint8_t *pTxBuffer, uint32_t Len);



#endif /*UART_H*/
