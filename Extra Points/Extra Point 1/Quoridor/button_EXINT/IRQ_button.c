#include "../Game/game.h"
#include "button.h"
#include "lpc17xx.h"

extern uint8_t started;
extern struct Wall wall;
extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;
extern uint8_t cooldown;


void EINT0_IRQHandler(void) /* INT0														 */
{
  if (!started) {
    started = 1;
    Start_Game();
  }
  LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt         */
}

void EINT1_IRQHandler(void) /* KEY1														 */
{
	if (game_state == TRANSITION || cooldown > 0) {
    LPC_SC->EXTINT &= (1 << 1);
    return;
  }
	
  Switch_Player_Wall();
	
  LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt         */
}

void EINT2_IRQHandler(void) /* KEY2														 */
{
	if (game_state == TRANSITION || moving_entity == PLAYER || cooldown > 0) {
    LPC_SC->EXTINT &= (1 << 2);
    return;
  }
	
	wall = Rotate_Wall(wall);
	
  LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt         */
}
