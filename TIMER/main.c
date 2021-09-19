
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS

/*MACROS*/

/*VARIABLES GLOBALES*/

uint32_t pulses;

/*PROTOTIPO DE FUNCIONES*/
/**
  * @brief configura el timer3 en modo periodico
  */
void TIMER3_PeriodicMode(void);

/**
  * @brief configura los pines usados
  */
void GPIO_Config(void);

void TIMER1_EdgeCount_Config(void);

int main(void)
{
  /*gpio*/
  GPIO_Config();
  /*timer*/
  TIMER1_EdgeCount_Config();
  TIMER3_PeriodicMode();
  
  while(1){
    
  }
}

/**
  * @brief configura el timer3 en modo periodico
  * @note
  * 1. cuando se trabaja en modo periodico en conteo descendente  el precaler funciona como verdadero prescalor,
  *    lo que quiere decir que funciona como divisor de la frecuencia del reloj de entrada al timer. (System Clock / PSC)
  * 2. cuando se trabaja en modo periodico, el prescaler funciona como una extension del timer y mantiene el MSB del contador
  *    esto quiere decir que el timer se convierte en un timer de 24-bit donde los bits 23:16 serán cargados al registro TIMERnPR
  */
void TIMER3_PeriodicMode(void){
  uint32_t temp = 2500000 - 1;                  //periodo de conteo para el modo ascendente
                                                /* TIMFREQ /UpdateEvent*/
  /*enable clock*/
  SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R3;
  while(!(SYSCTL->PRTIMER & SYSCTL_PRTIMER_R3));
  /*DISABLE TIMER*/
  TIMER3->CTL &=~ TIMER_CTL_TAEN;
  /*ESTABLECER LA LONGITUD DE CONTEO*/
  TIMER3->CFG |= TIMER_CFG_16_BIT;              //16-BIT TIMER
  /*CONFIGURAR EL MODO*/
  TIMER3->TAMR |= TIMER_TAMR_TAMR_PERIOD;       //modo periodico
  TIMER3->TAMR |= TIMER_TAMR_TACDIR;            //Conteo ascendente
  /*CONFIGURAR EL PRESCALER Y EL VALOR DE AUTORECARGA*/
  
  TIMER3->TAILR = temp & 0xFFFF;              //16-bit del contador
  TIMER3->TAPR = temp>>16 & 0xFF;             //bits 23:16 del contador
  //TIMER3->TAPR = 50 - 1;                        //50MHZ / 50 = 1MHZ
  //TIMER3->TAILR = 50000 - 1;
  /*CONFIGURAMOS INTERRUPCION*/
  TIMER3->ICR |= TIMER_ICR_TATOCINT;
  TIMER3->IMR |= TIMER_IMR_TATOIM;              //enable interrupt
  NVIC_SetPriority(TIMER3A_IRQn,2);
  NVIC_EnableIRQ(TIMER3A_IRQn);
  /*enable timer*/
  TIMER3->CTL |= TIMER_CTL_TAEN;
}


/**
  * @brief configura los pines usados
  */
void GPIO_Config(void){
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOF;
  while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R5));
  
  GPIOF->DIR |= 1<<1 | 1U<<2;
  GPIOF->DEN |= 1<<1 | 1U<<2;
  
  return;
}
/**
  * @brief configura el timer 1
  */
void TIMER1_EdgeCount_Config(void){
  /*enable clock*/
  SYSCTL->RCGCTIMER |= SYSCTL_RCGCTIMER_R1;
  while(!(SYSCTL->PRTIMER & SYSCTL_PRTIMER_R1));
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
  while(!(SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1));           //gpiob listo
  /*PB4 CANAL INPUT*/
  GPIOB->AFSEL |= 1U<<4;                                //FUNCION ALTERNATIVA
  GPIOB->PCTL &=~ 0xFU<<4*4;                            //Clear
  GPIOB->PCTL |= GPIO_PCTL_PB4_T1CCP0;                  //PB4 -> TIMER1A Input
  GPIOB->DEN |= 1U<<4;
  /*TIMER1 DISABLE*/
  TIMER1->CTL &=~ (TIMER_CTL_TAEN | TIMER_CTL_TBEN);
  /*Configurar la longitud de los timers*/
  TIMER1->CFG = TIMER_CFG_16_BIT;                       //16 bit independent mode
  /*Configurar el modo en edge count*/
  TIMER1->TAMR |= TIMER_TAMR_TAMR_CAP;                  //modo de captura
  TIMER1->TAMR &=~ TIMER_TAMR_TACMR;                    //edge count
  /*configurar el flanco de deteccion*/
  TIMER1->CTL |= TIMER_CTL_TAEVENT_POS;                 //Rising edge
  /*configurar la direccion de conteo*/
  TIMER1->TAMR |= TIMER_TAMR_TACDIR;                    //conteo ascendente
  /*configurar los registros GPTMAMATCHR y GPTMTAPMR
    GPTMTAILR y GPTMTAPR*/
  TIMER1->TAMATCHR = 0xFFFF;
  TIMER1->TAPMR = 0;
  TIMER1->TAPR = 0xFF;
  TIMER1->TAILR = 0xFFFF;
  /*Configurar la interrupciones*/
  TIMER1->ICR |= TIMER_ICR_CAMCINT;                     //CLEAR FLAG
  TIMER1->IMR |= TIMER_IMR_CAMIM;                       //enable capture event
  //NVIC
  NVIC_EnableIRQ(TIMER1A_IRQn);
  NVIC_SetPriority(TIMER1A_IRQn,1);                     //prioridad 1
  /*enable timer count*/
  TIMER1->CTL |= TIMER_CTL_TAEN;                        //enable count
  
  return;
}