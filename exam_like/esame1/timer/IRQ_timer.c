/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "timer.h"
#include "../led/led.h"
#include "../RIT/RIT.h"
#include "../../common.h"

extern volatile unsigned int VAR1;
extern volatile unsigned int VAR2;

extern unsigned char led_value;


extern volatile char alarm;
extern unsigned int result;



/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER0_IRQHandler (void)
{
	uint8_t val=0, ov1=0, ov2=0;
	
	if(VAR1 >= 16 ){
		ov1=1;
	}
	if(VAR2 >= 16){
		ov2=1;
	}
	//0000 0000
	val = VAR2;
	
	val |= 0xF0;
	
	val ^= ~(VAR1)<<4;

	
	//0001 0000
	
	//1010 0000
	
	
	
	if(ov1 && ov2){
		val = led_value == 0 ? 0xFF : 0;
	} else if(ov1){
		if((led_value & 0xF0) == 0){
			val |= 0xF<<4;
		} else{
			val &= 0x0F;
		}
	} else if(ov2){
		if((led_value & 0x0F) == 0){
			val |= 0xF;
		} else{
			val &= 0xF0;
		}
	}
	
	LED_Out(val);

	
	LPC_TIM0->IR = 1;
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	if(alarm == 1){
		LED_Out(led_value == 0 ? 0xFF : 0);
	} else{
		LED_Out(result);
		disable_timer(1);
	}
  LPC_TIM1->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 2 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER2_IRQHandler (void)
{
  LPC_TIM2->IR = 1;			/* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer3_IRQHandler
**
** Descriptions:		Timer/Counter 3 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER3_IRQHandler (void)
{
  LPC_TIM3->IR = 1;			/* clear interrupt flag */
  return;
}





/******************************************************************************
**                            End Of File
******************************************************************************/
