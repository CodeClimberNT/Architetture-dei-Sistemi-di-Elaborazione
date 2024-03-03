/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "lpc17xx.h"
#include "RIT.h"
#include "../led/led.h"
#include "../timer/timer.h"
#include "../../common.h"

#define N 8

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

/*variabili per gestione debouncing dei buttons*/
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;

volatile unsigned int VETT[N]={0};
volatile int n;
volatile unsigned int VAR1;
volatile unsigned int VAR2;

unsigned int result;

volatile char alarm;

extern unsigned int calcola_somma_prodotto(volatile unsigned int VETT[], 
							volatile unsigned int i, volatile char *alarm);

uint8_t view_mode = 0;


void RIT_IRQHandler (void)
{		
	static int j_left=0, j_down=0;
	
	/*Gestione Joystick Polling*/
	
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0 && view_mode == 0){ //joystick down
		j_down++;
		switch(j_down){
			case 1:
				add_vett();
				break;			
			default:
				break;
		}
	}else
		j_down=0;
	
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0 &&
     (LPC_GPIO1->FIOPIN & (1<<29)) == 0 && view_mode == 0){ //joystick up-left
		j_left++;
		switch(j_left){
			case 1:
				VAR2+=1;
				break;			
			default:
				break;
		}
	}else
		j_left=0;

	
	/*Gestione BUTTONS*/

	if(down_1!=0){  
			down_1 ++;  
			//controllo il piedino fisico 10->INT0, 11->KEY1, 12->KEY2
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ 
				switch(down_1){
				case 2:
					if(view_mode > 0)
						view_mode = 1;//start exiting view mode
					//codice da eseguire in caso di pressione del button
					break;				
				default:
					//se voglio gestire il button on X click
					//if(down_1 % X == 0){
					//}					
					break;
			}
		}
		else {	/* button released */
			
			if(view_mode == 0){
				//1s / 50ms = 20 
				if(down_1<40){ //2s
					VAR1+=2;
				} else if (down_1<60){//3s
					VAR1+=3;
				} else{//>3s
					call_asm();
				}
			} else if (view_mode==1){
				view_mode = 0; //exit view mode
				disable_timer(1);
				reset_timer(1);
				LED_Out(0);
				reset_timer(0);
				enable_timer(0);
			}
			
			
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}

	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

void add_vett(){
	VETT[n] = VAR1;
	n++;	
	VETT[n] = VAR2;
	n++;
	if(n>=N){
		call_asm();
	}
}

unsigned int call_asm(void){
	result = calcola_somma_prodotto(VETT, n, &alarm);

	disable_timer(0);
	
	VAR1 = 0;
	VAR2 = 0;
	
	enable_timer(1);
	
	for(n = 0; n < N; n++){
		VETT[n]=0;
	}
	
	n = 0;
	
	view_mode = 2;
	return result;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
