
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS

/*MACROS*/

/*VARIABLES GLOBALES*/
uint8_t button;

/*PROTOTIPO DE FUNCIONES*/
/**
  * @brief configura los pines necesarios
  */
void GPIOF_Config(void);


int main(void)
{
  
  
  GPIOF_Config();
  
  
  while(1){
    if(!(GPIOF->DATA & 1<<4)){
      GPIOF->DATA |= 1U<<3 | 1U<<2 | 1U<<1;
    }
    else if(!(GPIOF->DATA & 1)){
      GPIOF->DATA &=~( 1U<<3 | 1U<<2 | 1U<<1);
    }
  }
}

/**
  * @brief configura los pines necesarios
  */
void GPIOF_Config(void){
  /*1. habilitar el reloj y esperar que este listo*/
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOF;
  while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R5));
  /*2. unlock*/
  GPIOF->LOCK = 0x4C4F434B;
  GPIOF->CR = 0x1;
  /*3. Configurar la direccion*/
  //PF1, PF2, PF3 -> SALIDA
  GPIOF->DIR |= 1U<<3 | 1U<<2 | 1U<<1;
  //PF0, PF4 -> ENTRADA
  GPIOF->DIR &=~ (1U<<4 | 1U<<0); // GPIOF->DIR = GPIOF->DIR & (~(0x10 | 0x1));
                                  // GPIOF->DIR = GPIOF->DIR & (0xEF | 0xE)
  GPIOF->PUR |= 1U<<4 | 1U<<0;    //pull up      
  
  /*4. habilitar la funcion digital*/
  GPIOF->DEN |= 1U<<4 | 1U<<3 | 1U<<2 | 1U<<1 | 1U<<0; //0x1F
  return;
}