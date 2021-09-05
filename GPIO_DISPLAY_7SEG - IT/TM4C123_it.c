#include "TM4C123.h"

extern uint32_t uwTick;
extern uint8_t count;
typedef enum{
  DISPLAY1 = 1,
  DISPLAY2 = 2
}Display_t;
extern void sendDisplay(Display_t numDisplay,uint8_t num);

uint8_t display;
/*ISR*/
void SysTick_Handler(void){
  if(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk){
    uwTick+= 1;   //se incrementa el valor de la variable en 1
    if(display == 0){
      sendDisplay(DISPLAY1,count % 10);
      display++;
    }else if(display == 1){
      sendDisplay(DISPLAY2,count / 10);
      display = 0;
    }
  }
  return;
}

void GPIOF_Handler(void){
  if(GPIOF->RIS & 1)            //IT PF0
  {
    GPIOF->ICR |= 1;           //CLEAR
    //CODIGO
    if(count>= 0)
      count--;
  }
  else if(GPIOF->RIS & 1U<<4)   //IT PF4
  {
    GPIOF->ICR |= 1U<<4;        //CLEAR
    //CODIGO
    if(count<100)
      count ++;
  }
}