
/*INCLUDES*/
#include <stdint.h>
//#include <inc/tm4c123gh6pm.h>
#include <TM4C123.h>

/*MACROS*/
#define SYSCTL_BASE     0x400FE000UL

#define SYSCTL_RCGC2_R  *((volatile uint32_t*)(SYSCTL_BASE + 0x108))
#define SYSCTL_PRGPIO_R  *((volatile uint32_t*)(SYSCTL_BASE + 0xA08))

#define GPIOF_BASE      0x40025000UL
#define GPIOF_DIR_R     *((volatile uint32_t*)(GPIOF_BASE + 0x400))
#define GPIOF_DEN_R     *((volatile uint32_t*)(GPIOF_BASE + 0x51C))
#define GPIOF_DATA_R    *((volatile uint32_t *)0x400253FC)          //VERIFICAR ESTA DIRECCION
#define GPIOF_LOCK_R    *((volatile uint32_t*)(GPIOF_BASE + 0x520))
#define GPIOF_CR_R      *((volatile uint32_t*)(GPIOF_BASE + 0x524))
#define GPIOF_PUR_R     *((volatile uint32_t*)(GPIOF_BASE + 0x510))

/*VARIABLES GLOBALES*/


/*PROTOTIPO DE FUNCIONES*/


int main(void)
{
  /*GPIOF ENABLE CLOCK*/
  SYSCTL_RCGC2_R |= 1U<<5;
  while(!(SYSCTL_PRGPIO_R & 1U<<5));
  /*GPIOF */
  /*1. UNLOCK*/
  GPIOF_LOCK_R = 0x4C4F434B;
  GPIOF_CR_R = 0x1U;
  /*3. Configurar la direccion*/
  //PF1, PF2, PF3 -> SALIDA
  GPIOF_DIR_R |= 1U<<1 | 1U<<2 | 1U<<3;   //PF1,PF2, PF3 -> salida
  //PF0, PF4 -> ENTRADA
  GPIOF_DIR_R &=~ (1U<<4 | 1U<<0);
  GPIOF_PUR_R |= 1U<<4 | 1U<<0;
  /*4. habilitar la funcion digital*/
  GPIOF_DEN_R |= 1U<<4 | 1U<<3 | 1U<<2 | 1U<<1 | 1U<<0; //0x1F

  while(1){
    if(!(GPIOF_DATA_R & 1U<<4)){
      GPIOF_DATA_R |= 1U<<3 | 1U<<2 | 1U<<1;
    }
    else if(!(GPIOF_DATA_R & 1)){
      GPIOF_DATA_R &=~ ( 1U<<3 | 1U<<2 | 1U<<1);
    }
  }
}
