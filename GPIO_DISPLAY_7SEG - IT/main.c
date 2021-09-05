
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include "PLL.h"
#include "Delay.h"
/*MACROS*/
#define a       6               //PC6
#define b       2               //PA2
#define c       4               //PA4
#define d       6               //PD6
#define e       7               //PC7
#define f       7               //PD7
#define g       3               //PA3
#define EN1     7               //PA7
#define EN2     1               //PF1

typedef enum{
  DISPLAY1 = 1,
  DISPLAY2 = 2
}Display_t;

/*VARIABLES GLOBALES*/
const uint8_t Table7seg[] = {0X3F , 0X06 , 0X5B, 0X4F, 0X66, 0X6D, 0X7D, 0X07, 0X7F, 0X67 } ;
uint8_t count;
/*PROTOTIPO DE FUNCIONES*/
/**
  * @fn GPIO_Config
  * @brief Configura los pines necesarios para el manejo de los displays
  */
void GPIO_Config(void);
/**
  * @fn sendDisplay
  * @brief envia un numero decimal al display
  * @param numDisplay: numero de display que se quiere encender
  * @param num: numero que se quiere enviar
  */
void sendDisplay(Display_t numDisplay,uint8_t num);

/**
  * @brief configura los pines PF0 y PF4
  */
void GPIO_SW_IT_Config(void);

int main(void)
{
  __disable_irq();
  Delay_Init();
  /*GPIO INIT*/
  GPIO_Config();
  GPIO_SW_IT_Config ();
  __enable_irq();
  while(1){
    
  }
}

/**
  * @fn GPIO_Config
  * @brief Configura los pines necesarios para el manejo de los displays
  */
void GPIO_Config(void){
  /*1. ENABLE CLOCK*/
  SYSCTL->RCGCGPIO |=  SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R2 | \
                        SYSCTL_RCGCGPIO_R3 | SYSCTL_RCGCGPIO_R5;
  while(!(SYSCTL->PRGPIO & 1U<<0));   //GPIOA
  while(!(SYSCTL->PRGPIO & 1U<<2));   //GPIOC
  while(!(SYSCTL->PRGPIO & 1U<<3));   //GPIOD
  while(!(SYSCTL->PRGPIO & 1U<<5));   //GPIOF
  /*2. Unlock*/
  GPIOD->LOCK = 0x4C4F434B;
  GPIOD->CR |= 1U<<7;
  //PD7, PD6
  GPIOD->DIR |= 1U<<7 | 1U<<6;
  GPIOD->DEN |= 1U<<7 | 1U<<6;
  //PA2, PA3, PA4, PA7
  GPIOA->DIR |= 1U<<7 | 1U<<4 | 1U<<3 | 1U<<2;
  GPIOA->DEN |= 1U<<7 | 1U<<4 | 1U<<3 | 1U<<2;
  //PC6 y PC7  
  GPIOC->DIR |= 1U<<7 | 1U<<6;
  GPIOC->DEN |= 1U<<7 | 1U<<6;
  //PF1
  GPIOF->DIR |= 1U<<1;
  GPIOF->DEN |= 1U<<1;
  return;
}
/**
  * @fn sendDisplay
  * @brief envia un numero decimal al display
  * @param numDisplay: numero de display que se quiere encender
  * @param num: numero que se quiere enviar
  */
void sendDisplay(Display_t numDisplay,uint8_t num){
  uint8_t data = Table7seg[num];
  /*reset*/
  GPIOC->DATA &=~ (1U<<a | 1U<<e);
  GPIOA->DATA &=~ (1U<<b | 1U<<c | 1U<<g | 1u<<EN1);
  GPIOD->DATA &=~ (1U<<d | 1u<<f);
  GPIOF->DATA &=~ (1U<<EN2);
  /*send data*/
  //GPIOC->DATA |= ((data & 0x1)>>1)<<a;
  if(data & 0x1)                //a
    GPIOC->DATA |= 1U<<a;
  if(data & 1U<<1)              //b
    GPIOA->DATA |= 1U<<b;
  if(data & 1U<<2)              //c
    GPIOA->DATA |= 1U<<c;
  if(data & 1U<<3)              //d
    GPIOD->DATA |= 1U<<d;
  if(data & 1U<<4)              //e
    GPIOC->DATA |= 1U<<e;
  if(data & 1u<<5)              //f
    GPIOD->DATA |= 1U<<f;
  if(data & 1u<<6)              //g
    GPIOA->DATA |= 1U<<g;
  /*Multiplexacion*/
  if(numDisplay == DISPLAY1)
    GPIOA->DATA |= 1u<<EN1;
  else if(numDisplay == DISPLAY2)
    GPIOF->DATA |= 1U<<EN2;
  return;
}


/**
  * @brief configura los pines PF0 y PF4
  */
void GPIO_SW_IT_Config(void){
  /*UNLOCK*/
  GPIOF->LOCK = 0x4C4F434B;
  GPIOF->CR |= 1u<<4 | 1u<<0;
  
  //PF0, PF4
  GPIOF->DIR &=~ (1<<4 | 1); //10001
  GPIOF->DEN |= (1<<4 | 1);
  GPIOF->PUR |= (1<<4 | 1);
  
  GPIOF->IM &=~ (1U<<4 | 1U<<0);
  GPIOF->IS &=~ (1U<<4 | 1U<0);
  GPIOF->IBE &=~ (1U<<4 | 1U<<0);
  GPIOF->IEV &=~ (1U<<4 | 1U<<0);
  GPIOF->ICR |= 1U<<4 | 1U<<0;
  GPIOF->IM |= 1U<<4 | 1U<<0;
  //NVIC
  NVIC_SetPriority (GPIOF_IRQn,1);
  NVIC_EnableIRQ(GPIOF_IRQn);
  return;
}
