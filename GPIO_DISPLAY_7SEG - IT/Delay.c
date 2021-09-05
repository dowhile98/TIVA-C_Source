/**
  * @brief configuracion del systick como generador de base de tiempo
  * @autor Quino B. Jeffry
  * @version v.1
  */

#include "Delay.h"


/*VARIABLES PRIVADAS*/
uint32_t uwTick;			//variable que incrementa cada 1ms

/*funciones privadas*/
/**
 * @brief Retorna el valor de la variabla ticks
 * @param none
 * @return ticks
 */
uint32_t GetTick(void){
  return uwTick;
}


/**
  * @fn Delay_Init
  * @brief configura el systick timer
  */
void Delay_Init(void){

  /*deshabilitar la systick*/
  SysTick->CTRL = 0;
  /*2. cofigurar el valor de reload*/
  SysTick->LOAD = SystemCoreClock/1000 - 1;
  /*3. seleccionar la fuente de reloj*/
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; 
  /*4. poner el registro val*/
  SysTick->VAL = 0;
  /*5. habilitar la interrupcion y el conteo*/
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
  /*6. (opcional) cambiar la prioridad*/
  NVIC_SetPriority(SysTick_IRQn,7);

}

/**
  * @fn delay_ms
  * @brief genera retardos en ms
  * @param time_us: retardo en us deseado
  */
void delay_ms(uint32_t time_ms){
  uint32_t tickstart = GetTick();
  uint32_t wait = time_ms;
  /*delay us*/
  while((GetTick() - tickstart) < wait);
  /*return*/
  return;
}




