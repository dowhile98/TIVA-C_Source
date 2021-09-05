/**
  * @brief configuracion del systick como generador de base de tiempo
  * @autor Quino B. Jeffry
  * @version v.1
  */
#ifndef __DELAY_H
#define __DELAY_H
/*includes*/
#include "TM4C123.h"
/*macros*/

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
void delay_ms(uint32_t time_ms);

#endif /*__DELAY_H*/
