
/*INCLUDES*/
#include <stdint.h>
#include "inc/tm4c123gh6pm.h"  //tivaWare
#include "TM4C123.h"           // CMSIS
#include <stdio.h>
/*MACROS*/

/*VARIABLES GLOBALES*/
uint16_t data;

/*PROTOTIPO DE FUNCIONES*/
extern void UART0_Config(uint32_t UARTSysClk,uint32_t BaudRate);

/**
  * @fn ADC_ModuleInit()
  * @brief configura el modulo ADCx
  */
void ADC_ModuleInit(void);
/**
  * @fn ADC_SampleSeqConfig()
  * @brief configura la secuencia de conversion
  */
void ADC_SampleSeqConfig(void);

/**
  * @brief obtiene los valores de la conversion
  */
void ADC_ReadSeq0(void);

/**
  * @fn delay_ms
  * @brief genera retardos en milisegundos
  * @param time_ms: tiempo en milisegundos que se desea generar el retardo
  */
void delay_ms(uint32_t time_ms);

/**
  * @brief configura el timer3 en modo periodico
  */
void TIMER3_PeriodicMode(void);
/**
  * @brief configura los pines usados
  */
void GPIO_Config(void);

int main(void)
{
  /*uart*/
  UART0_Config(SystemCoreClock,115200);
  /*timer3*/
  TIMER3_PeriodicMode();
  /*gpio*/
  GPIO_Config();
  /*ADC*/
  ADC_ModuleInit();
  ADC_SampleSeqConfig();
  
  
  while(1){
    //ADC_ReadSeq0();
    //printf("valor leido->%d\r\n",data);
    //printf("%d\r\n",data);
    //delay_ms(100);
  }
}
/**
  * @fn ADC_ModuleInit()
  * @brief configura el modulo ADCx
  */
void ADC_ModuleInit(void){
  /*1. Enable the ADC clock using the RCGCADC */
  SYSCTL->RCGCADC |= 1U;
  while(!(SYSCTL->PRADC & SYSCTL_PRADC_R0));
  /*2. Enable the clock to the appropriate GPIO */
  SYSCTL->RCGC2 |= SYSCTL_RCGC2_GPIOB;
  while(!(SYSCTL->PRGPIO & SYSCTL_PRGPIO_R1));
  //PB4
  GPIOB->AFSEL |= 1U<<4;
  //2. DEN
  GPIOB->DEN &=~ (1U<<4);
  //3. AMSEL
  GPIOB->AMSEL |= 1U<<4;
  /*3. reconfigure the sample sequencer priorities in the ADCSSPRI*/
  
  return;
}

/**
  * @fn ADC_SampleSeqConfig()
  * @brief configura la secuencia de conversion
  */
void ADC_SampleSeqConfig(void){
  /*1. sample sequencer is disabled */
  ADC0->ACTSS &=~ (ADC_ACTSS_ASEN0 | ADC_ACTSS_ASEN1 | ADC_ACTSS_ASEN2 | ADC_ACTSS_ASEN3);
  /*2. Configure the trigger event for the sample sequencer */
  ADC0->EMUX &=~ (ADC_EMUX_EM0_M);
  ADC0->EMUX |= ADC_EMUX_EM0_TIMER;             //la conversion es disparado por el timer
  TIMER3->CTL |= TIMER_CTL_TAOTE;               //disparado por el timer3a
   /*3. For each sample in the sample sequence, configure the corresponding input source */
  ADC0->SSMUX0 = 0xA;                          //MUXO
  /*4. For each sample in the sample sequence, configure the sample control */
  ADC0->SSCTL0 |= ADC_SSCTL0_END0 | ADC_SSCTL0_IE0;
  /*5. If interrupts are to be used, set the corresponding MASK bit in the ADCIM register.*/
  ADC0->ISC |= ADC_ISC_IN0;
  ADC0->IM  |= ADC_IM_MASK0;
  NVIC_EnableIRQ(ADC0SS0_IRQn);
  /*6. Enable the sample sequencer logic by setting the corresponding ASENn */
  ADC0->ACTSS |= ADC_ACTSS_ASEN0;
  return;
}

/**
  * @brief obtiene los valores de la conversion
  */
void ADC_ReadSeq0(void){
  ADC0->PSSI |= ADC_PSSI_SS0;           //inicia la conversion por software
  while(!(ADC0->RIS & ADC_RIS_INR0));   //wait
  ADC0->ISC |= ADC_ISC_IN0;             //clear flag
  /*lectura*/
  data = ADC0->SSFIFO0;                 //sequencer 0
  return;
}

/**
  * @fn delay_ms
  * @brief genera retardos en milisegundos
  * @param time_ms: tiempo en milisegundos que se desea generar el retardo
  */
void delay_ms(uint32_t time_ms){
  /*1. enable = 0*/
  SysTick->CTRL = 0;
  /*2. configura el valor del registro RELOAD*/
  SysTick->LOAD = SystemCoreClock / 1000 - 1;
  /*3. poner el registro current a cero*/
  SysTick->VAL = 0;
  /*4. configurar la fuente de reloj y habilitar el conteo*/
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;
 
  /*retardo*/
  while(time_ms>0){
    while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));//retardo de 1ms
    time_ms--;
  }
  return;
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
  //uint32_t temp = 2500000 - 1;                  //periodo de conteo para el modo ascendente
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
  
  //TIMER3->TAILR = temp & 0xFFFF;              //16-bit del contador
  //TIMER3->TAPR = temp>>16 & 0xFF;             //bits 23:16 del contador
  TIMER3->TAPR = 250 - 1;                        //50MHZ / 250 = 200kh
  TIMER3->TAILR = 60000 - 1;                    //300ms -> periodo de muestreo
  /*CONFIGURAMOS INTERRUPCION*/
  TIMER3->ICR |= TIMER_ICR_TATOCINT;
  TIMER3->IMR |= TIMER_IMR_TATOIM;              //enable interrupt
  NVIC_SetPriority(TIMER3A_IRQn,2);
  //NVIC_EnableIRQ(TIMER3A_IRQn);
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
