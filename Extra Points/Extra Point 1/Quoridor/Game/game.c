#include "game.h"

volatile struct Player player0;
volatile struct Player player1;
volatile struct UI timer_ui;
volatile struct UI player0_ui;
volatile struct UI player1_ui;
volatile DIRECTION direction;
volatile struct Wall wall;

volatile uint8_t current_player = 0;
volatile uint8_t timeLeft = MAX_TIME;

volatile char p0_wall_remaining[2];
volatile char p1_wall_remaining[2];
volatile char time_value[2];
volatile uint8_t cooldown;

volatile WALL_DIRECTION WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL] = {NA};
// consider wall position in the junction between the player position (assume you cannot position a wall outside the board to have a 1 block lenght wall)
// e.g.  [] [] []
//  	     X  X
//			 [] [] []
// wall position will be in the X (where the [] rappresent the player position) and based on his propreties (Horizontal/Vertical) will block the respective Player Position

volatile GAME_STATE game_state = TITLE_SCREEN;
volatile MOVING_ENTITY moving_entity = PLAYER;

volatile uint32_t lastMove;
volatile uint8_t game_over = 1;


void Title_Screen() {
	LCD_Clear(White);
	GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
	LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_WIDTH, READY_UI_HEIGHT, Blue2, SUD_OVEST, DarkGray);
	GUI_Text(READY_UI_XPOS + 20, READY_UI_YPOS + (READY_UI_HEIGHT / 2) - 5, (uint8_t *)"PRESS INT0 TO START", Black, White);

	GUI_Text(70, MAX_Y - 20, (uint8_t *)"FELIZ", ChristmasRed, White);
	GUI_Text(115, MAX_Y - 20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}

void End_Turn() {
  current_player ^= 1;  // alternate between 0 and 1
  timeLeft = MAX_TIME;
  Update_Timer_UI(timeLeft);
}

uint8_t Win_Condition(){
	return (player0.pos.y==0) || (player1.pos.y == NUM_ROWS-1);
}

void Game_Over(){
	struct Player winner = player0.pos.y == 0 ? player0 : player1;
	
	LCD_FillRect(READY_UI_XPOS,READY_UI_YPOS, READY_UI_XPOS + READY_UI_WIDTH, READY_UI_YPOS + READY_UI_HEIGHT, White);
	LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_WIDTH, READY_UI_HEIGHT, Blue2, SUD_OVEST, DarkGray);
	
	if(winner.id == 0){
		GUI_Text(READY_UI_XPOS + 40, READY_UI_YPOS + (READY_UI_HEIGHT / 2) - 5, (uint8_t *)"PLAYER 1 WINS!", Black, White);
	} else{
		GUI_Text(READY_UI_XPOS + 40, READY_UI_YPOS + (READY_UI_HEIGHT / 2) - 5, (uint8_t *)"PLAYER 2 WINS!", Black, White);
	}
	wait_delay(5000000);
	game_over = 1;
}

void Move(DIRECTION dir) {
	direction = dir;
	cooldown = COOLDOWN;

    switch (moving_entity) {
        case PLAYER:
            if (current_player == 0) {
                player0 = Move_Player(player0, dir, 0);
            } else {
                player1 = Move_Player(player1, dir, 0);
            }
						game_state = MOVE_PLAYER;
            break;

        case WALL:
					game_state = MOVE_WALL;
            wall = Move_Wall(wall, dir);
            break;
        default:
            GUI_Text(0, 0, (uint8_t *)"ERROR: \"moving_entity\" WRONG VALUE", Black, White);
    }
}
