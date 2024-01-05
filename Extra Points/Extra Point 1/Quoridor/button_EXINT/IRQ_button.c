#include "../Game/game.h"
#include "button.h"
#include "lpc17xx.h"

extern uint8_t game_over;
extern struct Wall wall;
extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;
extern uint8_t cooldown;


void EINT0_IRQHandler(void) /* INT0														 */
{
  if (game_over) {
    game_over = 0;
    game_state = START_GAME;
  }
  LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt         */
}

void EINT1_IRQHandler(void) /* KEY1														 */
{
	if (game_state != IDLE  || cooldown > 0) {
    LPC_SC->EXTINT &= (1 << 1);
    return;
  }
	cooldown = COOLDOWN;
  game_state = CHANGE_MOVING_ENTITY;
	
  LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt         */
}

void EINT2_IRQHandler(void) /* KEY2														 */
{
	if (game_state != IDLE  || moving_entity == PLAYER || cooldown > 0) {
    LPC_SC->EXTINT &= (1 << 2);
    return;
  }
	cooldown = COOLDOWN;
	game_state = ROTATE_WALL;
	
  LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt         */
}
