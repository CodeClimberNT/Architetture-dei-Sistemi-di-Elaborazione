#include "game.h"

extern struct Player player0;
extern struct Player player1;

extern struct UI timer_ui;
extern struct UI player0_ui;
extern struct UI player1_ui;

volatile struct Wall wall;

volatile uint8_t currPlayer = 0;
volatile uint8_t timeLeft = MAX_TIME;

volatile char p1_wall_remaining[2];
volatile char p2_wall_remaining[2];
volatile char time_value[2];
volatile uint8_t cooldown;

volatile uint8_t WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL] = {0};
// consider wall position in the junction between the player position (assume you cannot position a wall outside the board to have a 1 block lenght wall)
// e.g.  [] [] []
//  	   X  X
//		 [] [] []
// wall position will be in the X (where the [] rappresent the player position) and based on his propreties (Horizontal/Vertical) will block the respective Player Position

volatile GAME_STATE game_state = TRANSITION;
volatile MOVING_ENTITY moving_entity = PLAYER;

volatile uint32_t lastMove;
// volatile uint8_t started = 0;
volatile uint8_t started = 1; /********************REMOVE THIS WHEN FINISH DEBUGGING***************/

void Setup() {
    Peripheral_Init();
    if (!started) {
        Waiting_UI();
    } else {
        Start_Game();
        Peripheral_Enable();
    }
}


void End_Turn() {
  game_state = TRANSITION;
	
	
	moving_entity = PLAYER; //make sure player is in control
  currPlayer ^= 1;  // alternate between 0 and 1
  timeLeft = MAX_TIME;
  Update_Timer(timeLeft);
}

void Switch_Player_Wall() {
    game_state = TRANSITION;
    if (moving_entity == PLAYER) {
        moving_entity = WALL;
        wall = Create_Wall(wall);
    } else {
        Remove_Wall(wall);
        moving_entity = PLAYER;
    }
}

void Move(DIRECTION dir) {
    game_state = TRANSITION;
		cooldown = COOLDOWN;

    switch (moving_entity) {
        case PLAYER:
            if (currPlayer == 0) {
                player0 = Move_Player(player0, dir);
            } else {
                player1 = Move_Player(player1, dir);
            }
            break;

        case WALL:
            wall = Move_Wall(wall, dir);
            break;
        default:
            GUI_Text(0, 0, (uint8_t *)"ERROR: \"moving_entity\" WRONG VALUE", Black, White);
    }
}
