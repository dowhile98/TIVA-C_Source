/**
  * @brief configuracion del systick como generador de base de tiempo
  * @autor Quino B. Jeffry
  * @version v.1
  */
#include "Delay.h"
    
    
/*VARIABLES PRIVADAS*/
uint32_t uwTick;			//variable que incrementa cada 1us/1ms

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
  //__disable_irq();
  /*disable SysTick*/
  SysTick->CTRL = 0;
  /*configurar el registro reload*/
#if (USE_MICROSECONDS == 1)
  SysTick->LOAD = SysCtlClockGet()/1000000 - 1u; //us
#else   
  SysTick->LOAD = SysCtlClockGet()/1000 - 1U;    //ms        
#endif /*USE_MICROSECONDS*/
  /*Seleccionar la fuente de reloj*/
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
  /*poner a cero el registro current*/
  SysTick->VAL  = 0;
  /*habilitar el conteo y la interrupcion*/
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
  /*cambiar la prioridad*/
  NVIC_SetPriority(SysTick_IRQn,7);
  //__enable_irq();
}

#if (USE_MICROSECONDS == 1)
/**
  * @fn delay_us
  * @brief genera retardos en us
  * @param time_us: retardo en us deseado
  */
void delay_us(uint32_t time_us){
  uint32_t tickstart = GetTick();
  uint32_t wait = time_us;
  /*delay us*/
  while((GetTick() - tickstart) < wait);
  /*return*/
  return;
}
/**
  * @fn delay_ms
  * @brief genera retardos en ms
  * @param time_ms: retardo en ms deseado
  */
void delay_ms(uint32_t time_ms){
  uint32_t i;
  for(i = 0;i<time_ms;i++){
    delay_us(1000);                     //1ms
  }
}
#else 
/**
  * @fn delay_ms
  * @brief genera retardos en ms
  * @param time_ms: retardo en ms deseado
  */
void delay_ms(uint32_t time_ms){
  uint32_t tickstart = GetTick();
  uint32_t wait = time_ms;
  /*delay us*/
  while((GetTick() - tickstart) < wait);
  /*return*/
  return;
}

#endif /*SE_MICROSECONDS*/