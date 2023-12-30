#include "game.h"

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;
extern uint8_t WallMatrixPosition[NUM_ROWS - 1][NUM_COLUMNS - 1];

extern struct UI timer_ui;
extern char time_value[2];
extern uint8_t timeLeft;

extern struct Player player0;
extern struct Player player1;


void wait_delay(int count) {
  while (count--)
    ;
}
