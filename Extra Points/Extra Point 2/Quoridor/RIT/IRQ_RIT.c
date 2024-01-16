/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "../Game/game.h"
#include "RIT.h"
#include "lpc17xx.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;

extern uint8_t started;
extern struct Wall wall;

extern uint8_t current_player;
extern struct Player player0;
extern struct Player player1;

extern uint8_t cooldown;

/*button debouncing variables*/
volatile uint8_t down_0 = 0;
volatile uint8_t down_1 = 0;
volatile uint8_t down_2 = 0;


void RIT_IRQHandler(void) {
	static uint8_t j_select=0, j_left=0, j_right=0, j_down=0, j_up=0;

  if (cooldown > 0)
    --cooldown;

  // If game is processing exit and do nothing
  if (game_state == TRANSITION || cooldown > 0) {
    LPC_RIT->RICTRL |= 0x1;
    return;
  }
	
	//POLLING PERIPHERAL BEFORE STARTING
	if(!started){
		if(down_0!=0){  
			down_0 ++;
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){ 
				switch(down_0){
					case 2:
						started = 1;
						Setup();
						break;
					default:		
						break;
				}
			}
			else {	/* button released */
				down_0=0;			
				NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
			}
		}
	
	} else {
	
	/******JOYSTICK POLLING*******/
	
  /* Joytick J_Select pressed p1.25*/
  if ((LPC_GPIO1->FIOPIN & (1 << 25)) == 0) {
		j_select++;
		switch(j_select){
			case 1:
				if(moving_entity == PLAYER)
					switch(current_player){
						case 0:
							player0 = Move_Player(player0, )
					}
				if(moving_entity == WALL)
					wall = Place_Wall(wall);
				break;
			default:
				break;
		}
	}else
		j_select=0;

  /* Joytick UP p1.29*/
  if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
		j_up++;
		switch(j_up){
			case 1:
				Move(UP);
				break;			
			default:
				break;
		}
	}else
		j_up=0;

  /* Joytick RIGHT p1.28*/
  if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
		j_right++;
		switch(j_right){
			case 1:
				Move(RIGHT);
				break;			
			default:
				break;
		}
	}else
		j_right=0;

  /* Joytick DOWN p1.26*/
  if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {
		j_down++;
		switch(j_down){
			case 1:
				Move(DOWN);
				break;			
			default:
				break;
		}
	}else
		j_down=0;

  /* Joytick LEFT p1.27*/
  if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {
		j_left++;
		switch(j_left){
			case 1:
				Move(LEFT);
				break;			
			default:
				break;
		}
	}else
		j_left=0;
	
	/********************BUTTON POLLING***************************/

	
	if(down_1!=0){  
			down_1 ++;
			if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){ 
				switch(down_1){
				case 2:
					if (game_state == TRANSITION || cooldown > 0) {
						LPC_SC->EXTINT &= (1 << 1);
						return;
					}
					cooldown = COOLDOWN;
					Switch_Player_Wall();
					break;				
				default:			
					break;
			}
		}
		else {	/* button released */
			down_1=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
	
	if(down_2!=0){  
			down_2 ++;  
			if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){ 
				switch(down_2){
				case 2:
					if (game_state == TRANSITION || moving_entity == PLAYER || cooldown > 0) {
						LPC_SC->EXTINT &= (1 << 2);
						return;
					}
					cooldown = COOLDOWN;
					wall = Rotate_Wall(wall);
					break;				
				default:				
					break;
			}
		}
		else {	/* button released */
			down_2=0;			
			NVIC_EnableIRQ(EINT2_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
		}
	}
	
	}
	reset_RIT();
	
  LPC_RIT->RICTRL |= 0x1; /* clear interrupt flag */
}
/******************************************************************************
**                            End Of File
******************************************************************************/
