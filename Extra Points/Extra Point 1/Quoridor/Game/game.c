#include "game.h"



volatile struct Player player0;
volatile struct Player player1;
volatile struct UI timer_ui;
volatile struct UI player0_ui;
volatile struct UI player1_ui;

volatile struct Wall wall;

volatile uint8_t currPlayer = 0;
volatile uint8_t timeLeft = MAX_TIME;

struct Wall WallMatrixPosition [NUM_ROWS-1][NUM_COLUMNS-1] = {0}; 
//consider wall position in the junction between the player position (assume you cannot position a wall outside the board to have a 1 block lenght wall)
//e.g.  [] [] [] 
// 			  X  X  
//			[] [] []
//wall position will be in the X (where the [] rappresent the player position) and based on his propreties (Horizontal/Vertical) will block the respective Player Position

GAME_STATE game_state = TRANSISTION;

uint32_t lastMove;
//uint8_t started = 0;
uint8_t started = 1; /********************REMOVE THIS WHEN FINISH DEBUGGING***************/

void Setup(){
	if(!started){
		Peripheral_Init();
		Waiting_Player();
	}
	else{
		Peripheral_Init(); /******************REMOVE THIS WHEN FINISH DEBUGGING******************/
		Start_Game();
	}
}

void Waiting_Player(){
	GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
	LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_XPOS + READY_UI_WIDTH, READY_UI_YPOS + READY_UI_HEIGHT, Blue2, NORTH_OVEST, DarkGray);
	GUI_Text(READY_UI_XPOS+20, READY_UI_YPOS+(READY_UI_HEIGHT/2)-5, (uint8_t *)"PRESS INT0 TO START", Black, White);
	
	GUI_Text(70, MAX_Y-20, (uint8_t *)"FELIZ", ChristmasRed, White);
	GUI_Text(115, MAX_Y-20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}

void End_Turn(){
	currPlayer ^= 1; 			//alternate between 0 and 1
	timeLeft = MAX_TIME;
}

void Move(DIRECTION dir){
	struct Vector2D vec2d;
	
	//Mapping enum dir to actual relative direction for movement
	vec2d = GetPos(dir);
	
	switch (game_state){
		case PLAYER:
			if(currPlayer == 0){
				Move_Player(player0, vec2d);
			} else {
				Move_Player(player1, vec2d);
			}
			break;
		case WALL:
			wall = Move_Wall(wall, vec2d);
			break;
		default:
			GUI_Text(0,0,(uint8_t *)"ERROR: MOVING STATE NOT READY", Black, White);
	}
}

