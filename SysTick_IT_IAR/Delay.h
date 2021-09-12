/**
  * @brief configuracion del systick como generador de base de tiempo
  * @autor Quino B. Jeffry
  * @version v.1
  */
#ifndef __DELAY_H
#define __DELAY_H
/*includes*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"           //TivaWare
#include "TM4C123.h"                    //CMSIS
#include <stdbool.h>
#include "driverlib/sysctl.h"
/*macros*/

/*1 = 1us
  0 = 1ms*/
#define USE_MICROSECONDS       0
/*function prototype*/
/**
  * @fn Delay_Init
  * @brief configura el systick timer
  */
void Delay_Init(void);

/**
  * @fn delay_us
  * @brief genera retardos en us
  * @param time_us: retardo en us deseado
  */
#if (USE_MICROSECONDS == 1)

void delay_us(uint32_t time_us);

#endif/*USE_MICROSECONDS*/

/**
  * @fn delay_ms
  * @brief genera retardos en ms
  * @param time_ms: retardo en ms deseado
  */
void delay_ms(uint32_t time_ms);


#endif /*__DELAY_H*/

