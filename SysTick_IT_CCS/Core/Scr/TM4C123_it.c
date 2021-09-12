/*
 * TM4C123_it.c
 *
 *  Created on: 11 set. 2021
 *      Author: Lenovo
 */
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"


extern uint32_t uwTick;
/**
  * @fn SysTick_Handler
  * @brief ISR de la systick
  */
void SysTick_Handler(void){
    if(NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT){
        uwTick+= 1;   //se incrementa el valor de la variable en 1
    }
    return;
}

