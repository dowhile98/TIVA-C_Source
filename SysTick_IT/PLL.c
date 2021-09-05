/**
  * @brief PLL CONFIGURATION
  * @autor Quino B. Jeffry
  * @version v.10
  */

#include "PLL.h"


/**
  * @fn PLL_Config
  * @brief configura  el systemCoreClock
  * @param freq: frecuencia de referencia deseada
  */
void PLL_Config(uint32_t freq){
  /*establecer el bit USERCC2 para usar el registro RCC2*/
  SYSCTL->RCC2 |= SYSCTL_RCC2_USERCC2;
  /*1. cambiar la fuente del sysctl*/
  SYSCTL->RCC2 |= SYSCTL_RCC2_BYPASS2;
  /*2. Seleccionar la fuente del pll y establecer la frecuencia*/
  SYSCTL->RCC &=~ SYSCTL_RCC_XTAL_M;
  SYSCTL->RCC |= SYSCTL_RCC_XTAL_16MHZ;
  SYSCTL->RCC2 &=~ SYSCTL_RCC2_OSCSRC2_M;
  /*3. Encender el PLL*/
  SYSCTL->RCC2 &=~ SYSCTL_RCC2_PWRDN2;
  /*4. establecer los valores de SYSDIV2 Y SYSDIVLSB*/
  SYSCTL->RCC2 |= SYSCTL_RCC2_DIV400;
  SYSCTL->RCC2 &=~ (SYSCTL_RCC2_SYSDIV2_M | SYSCTL_RCC2_SYSDIV2LSB);
  SYSCTL->RCC2 |= freq<<22;
  /*5. Esperar que el PLL este listo para ser usado*/
  while(!(SYSCTL->RIS & SYSCTL_RIS_PLLLRIS));
  /*6. Seleccionar el pll como fuente de reloj*/
  SYSCTL->RCC2 &=~ SYSCTL_RCC2_BYPASS2;
  /**************************************************************************/
  SystemCoreClock = SysCtlClockGet();
  return;
}