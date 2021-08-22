
/*INCLUDES*/
#include <stdint.h>
#include <inc/tm4c123gh6pm.h>
#include <TM4C123.h>

/*MACROS*/
#define SYSCTL_BASE     0x400FE000UL

#define SYSCTL_RCGC2_R  *((volatile uint32_t*)(SYSCTL_BASE + 0x108))
#define SYSCTL_PRGPIO_R  *((volatile uint32_t*)(SYSCTL_BASE + 0xA08))

#define GPIOF_BASE      0x40025000UL
#define GPIOF_DIR_R     *((volatile uint32_t*)(GPIOF_BASE + 0x400))
#define GPIOF_DEN_R     *((volatile uint32_t*)(GPIOF_BASE + 0x51C))
#define GPIOF_DATA_R    *((volatile uint32_t*)(GPIOF_BASE + 0x000))
/*VARIABLES GLOBALES*/


/*PROTOTIPO DE FUNCIONES*/


int main(void)
{
  /*GPIOF ENABLE CLOCK*/
  SYSCTL_RCGC2_R |= 1U<<5;
  while(!(SYSCTL_PRGPIO_R & 1U<<5));
  /*GPIOF */
  GPIOF_DIR_R |= 1U<<1;   //PF1 -> salida
  GPIOF_DEN_R |= 1U<<1;   //pf1 -> digital

  GPIO_PORTF_DATA_R |= 1U<<1;
//  GPIOF_DATA_R |= 1U<<1;
  while(1){
    
  }
}
