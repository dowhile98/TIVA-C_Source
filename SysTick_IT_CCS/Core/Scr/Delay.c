/**
  * @brief configuracion del systick como generador de base de tiempo
  * @autor Quino B. Jeffry
  * @version v.1
  */
#include "Delay.h"


/*VARIABLES PRIVADAS*/
uint32_t uwTick;            //variable que incrementa cada 1us/1ms

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
  * @param sysFreq: frecuencia de reloj de la fuente del systick
  */
void Delay_Init(uint32_t sysFreq){
    /*disable SysTick*/
    NVIC_ST_CTRL_R = 0;
    /*configurar el registro reload*/
#if (USE_MICROSECONDS == 1)
    NVIC_ST_RELOAD_R = sysFreq/1000000 - 1u; //us
#else
    NVIC_ST_RELOAD_R = sysFreq/1000 - 1U;    //ms
#endif /*USE_MICROSECONDS*/
    /*Seleccionar la fuente de reloj*/
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;
    /*poner a cero el registro current*/
    NVIC_ST_CURRENT_R = 0;
    /*habilitar el conteo y la interrupcion*/
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE | NVIC_ST_CTRL_INTEN;
    /*cambiar la prioridad*/
    NVIC_SYS_PRI3_R &=~ NVIC_SYS_PRI3_TICK_M;
    NVIC_SYS_PRI3_R |= 7U<<NVIC_SYS_PRI3_TICK_S;
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


