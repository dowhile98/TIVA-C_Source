
/**
  *
  * @note
  * UART3: TX: PC7
  *        RX: PC6
  */
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include <stdio.h>
#include <string.h>
#include "UART.h"
#include "Delay.h"
/*MACROS*/


/*VARIABLES GLOBALES*/
int data;
char txt[100];
uint8_t rxBuffer[100];

uint8_t Cmd_End[3] = {0xFF,0xFF,0xFF};  // command end sequence

/*PROTOTIPO DE FUNCIONES*/
void NEXTION_SendString (char *ID, char *string)
{
  char buf[50];
  int len = sprintf (buf, "%s.txt=\"%s\"", ID, string);
  /*send data*/
  UART_SendData(UART3,(uint8_t*)buf,len);
  /*sen end off command*/
  UART_SendData(UART3,Cmd_End,3);
}

/**
  * @brief configura los pines usados
  */
void GPIOF_Config(void);

int main(void)
{

/******************************************************************************/
  //UART
  UART0_Config(SystemCoreClock,115200);
  UART3_Config(SystemCoreClock,9600);
  //GPIO
  GPIOF_Config();
  //ADC
  
  //DELAY
  Delay_Init();
/*******************************************************************************/
  NEXTION_SendString("t1", "LED OFF");
  
  while(1){
    UART_ReceiveData(UART3,rxBuffer,5);
    
    if(rxBuffer[2] == 0x02){
      NEXTION_SendString("t1", "LED ON");
      GPIOF->DATA |= 1U<<1;
    }
    else if(rxBuffer[2] == 0x03){
      NEXTION_SendString("t1", "LED OFF");
      GPIOF->DATA &=~ 1U<<1;
    }
    
  }
}

/**
  * @brief configura los pines usados
  */
void GPIOF_Config(void){
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5));
    GPIO_PORTF_DIR_R |= 1<<3 | 1<<2 | 1<<1;
    GPIO_PORTF_DEN_R |= 1<<3 | 1<<2 | 1<<1;
    return;
}




