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
  /********** 0) configure the system to use RCC2 for advanced features****************/
  SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
  /********** 1) set BYPASS (bit 11) **************************************************/
  SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
  /********** 2) specify the crystal frequency, the OSCSRC2 bist are cleared to select 
								 the main oscillator as the oscillator clock source*******************/
  SYSCTL_RCC_R &=~(uint32_t) SYSCTL_RCC_XTAL_M;
  SYSCTL_RCC_R |= SYSCTL_RCC_XTAL_16MHZ;							//16MHz
  SYSCTL_RCC2_R &=~ (uint32_t) SYSCTL_RCC2_OSCSRC2_M;	//Main oscillator
  /********* 3) clear PWWDN2 (active PLL)		******************************************/
  SYSCTL_RCC2_R &=~ (uint32_t) SYSCTL_RCC2_PWRDN2;
  /********* 4) set the desired system divider and the system divider least significant bit**/
  SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;								//use 400MHz pll
  SYSCTL_RCC2_R &=~(uint32_t) SYSCTL_RCC2_SYSDIV2LSB & ~(uint32_t) SYSCTL_RCC2_SYSDIV2_M;
  SYSCTL_RCC2_R |= freq << 22;
	
  /********* 5) wait for PLL to stabilize by waiting for PLLRIS (bit5) -> SYSCTL_RIS_R */
  while(!(SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS));
	
  /********* 6) enable use of PLL by clearing BYPASS ********************************/
  SYSCTL_RCC2_R &=~ (uint32_t) SYSCTL_RCC2_BYPASS2;
  /**************************************************************************/
  SystemCoreClock = SysCtlClockGet();
  return;
}