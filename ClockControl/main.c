
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include "PLL.h"
/*MACROS*/

/*VARIABLES GLOBALES*/
uint32_t sysFreq;

/*PROTOTIPO DE FUNCIONES*/
/**
  * @brief cambio de la fuente de reloj del sistema
  */
void ChangeClockSysclk(void);

int main(void)
{
  sysFreq = SystemCoreClock;
  
  PLL_Config(Bus40MHz);
  
  sysFreq = SystemCoreClock;
  
  PLL_Config(Bus40MHz);
  
  sysFreq = SystemCoreClock;
  ChangeClockSysclk();
  sysFreq = SysCtlClockGet();
  
  while(1){
    
  }
}

/**
  * @brief cambio de la fuente de reloj del sistema
  */
void ChangeClockSysclk(void){
  SYSCTL->RCC2 &=~ SYSCTL_RCC2_USERCC2;
  
  SYSCTL->RCC &=~ SYSCTL_RCC_OSCSRC_M;
  SYSCTL->RCC |= SYSCTL_RCC_OSCSRC_INT4;
  SYSCTL->RCC &=~ SYSCTL_RCC_USESYSDIV;
  SYSCTL->RCC |= SYSCTL_RCC_BYPASS;
}
