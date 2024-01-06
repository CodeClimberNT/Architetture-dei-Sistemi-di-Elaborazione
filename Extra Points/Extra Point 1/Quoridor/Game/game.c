#include "game.h"

volatile struct Player player0;
volatile struct Player player1;
volatile struct UI timer_ui;
volatile struct UI player0_ui;
volatile struct UI player1_ui;

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

volatile GAME_STATE game_state = TRANSITION;
volatile MOVING_ENTITY moving_entity = PLAYER;

volatile uint32_t lastMove;
volatile uint8_t started = 0;

void Setup() {
	Peripheral_Init();
	if (!started) {
		Title_Screen();
	} else {
		Start_Game();
		Peripheral_Enable();
	}
}

void Title_Screen() {
	LCD_Clear(White);
	GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
	LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_WIDTH, READY_UI_HEIGHT, Blue2, SUD_OVEST, DarkGray);
	GUI_Text(READY_UI_XPOS + 20, READY_UI_YPOS + (READY_UI_HEIGHT / 2) - 5, (uint8_t *)"PRESS INT0 TO START", Black, White);

	GUI_Text(70, MAX_Y - 20, (uint8_t *)"FELIZ", ChristmasRed, White);
	GUI_Text(115, MAX_Y - 20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}

void End_Turn() {
  game_state = TRANSITION;

	if(Win_Condition()){
		Game_Over();
		return;
	}
	
	//if turn timeout while moving wall
	if(moving_entity == WALL){
		Remove_Wall(wall);
		moving_entity = PLAYER;
	}
  current_player = current_player == 0 ? 1 : 0;  // alternate between P0 and P1
	Create_Hint_Move(current_player == 0 ? player0 : player1);
  timeLeft = MAX_TIME;
  Update_Timer_UI(timeLeft);
}

void Timer_End_Turn(){
	if(moving_entity == PLAYER){
		Remove_Hint_Move(current_player == 0 ? player0 : player1);
	} else {
		Remove_Wall(wall);
	}
	End_Turn();
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
	started = 0;
	Setup();
}

void Switch_Player_Wall() {
	game_state = TRANSITION;
	if (moving_entity == PLAYER) {
		//check if the player have no more wall left
		if ((current_player == 0 ? player0.wallsRemaining : player1.wallsRemaining) == 0) {
			LCD_FillRect(READY_UI_XPOS, READY_UI_YPOS, READY_UI_XPOS + READY_UI_WIDTH, READY_UI_YPOS + READY_UI_HEIGHT, White);
			LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_WIDTH, READY_UI_HEIGHT, Blue2, SUD_OVEST, DarkGray);
			GUI_Text(READY_UI_XPOS + 20, READY_UI_YPOS + (READY_UI_HEIGHT / 2) - 5, (uint8_t *)"NO MORE WALL LEFT!", Black, White);
			wait_delay(5000000);
			//remove also shadow
			LCD_FillRect(READY_UI_XPOS-2, READY_UI_YPOS-2, READY_UI_XPOS + READY_UI_WIDTH+2, READY_UI_YPOS + READY_UI_HEIGHT+2, White);
			Draw_Board();
			return;
		}
		moving_entity = WALL;
		Remove_Hint_Move(current_player == 0 ? player0 : player1);
		wall = Create_Wall(wall);
	} else {
		Remove_Wall(wall);
		Create_Hint_Move(current_player == 0 ? player0 : player1);
		moving_entity = PLAYER;
	}
}

void Move(DIRECTION dir) {
    game_state = TRANSITION;
		cooldown = COOLDOWN;

    switch (moving_entity) {
        case PLAYER:
            if (current_player == 0) {
                player0 = Move_Player(player0, dir, 0);
            } else {
                player1 = Move_Player(player1, dir, 0);
            }
            break;

        case WALL:
            wall = Move_Wall(wall, dir);
            break;
        default:
            GUI_Text(0, 0, (uint8_t *)"ERROR: \"moving_entity\" WRONG VALUE", Black, White);
    }
}
