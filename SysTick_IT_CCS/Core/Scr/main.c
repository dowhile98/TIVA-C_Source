

/**
 * main.c
 */

/*includes*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "Delay.h"
/*definicion de macros*/

/*variables globales*/


/*definicion de prototipo de funciones*/

/**
  * @brief configura los pines necesarios
  */
void GPIO_Config(void);

int main(void)
{
    /*pll config*/
	PLL_Config(Bus80MHz);
	/*delay init*/
	Delay_Init(SystemCoreClock);
	/*gpio_config*/
	GPIO_Config();

	while(1){

	    GPIO_PORTF_DATA_R ^= 1u<<1;
	    delay_ms(50);
	}
}
/**
  * @brief configura los pines necesarios
  */
void GPIO_Config(void){
    SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    while(!(SYSCTL_PRGPIO_R & 1U<<5));
    GPIO_PORTF_DEN_R |= 1u<<1;
    GPIO_PORTF_DIR_R |= 1u<<1;

    return;
}
