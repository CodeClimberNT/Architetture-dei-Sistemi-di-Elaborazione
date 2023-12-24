#include "game.h"

volatile struct Player player0;
volatile struct Player player1;
volatile uint8_t currPlayer = 0;
volatile uint8_t timeLeft = MAX_TIME;
volatile uint8_t currWallUI[2];
uint8_t PlayerMatrix [NUM_ROWS][NUM_COLUMNS] = {0};
uint8_t WallMatrix [NUM_ROWS-1][NUM_COLUMNS-1] = {0}; 
//consider wall position in the junction between the player position (assume you cannot position a wall outside the board to have a 1 block lenght wall)
//e.g. 	[] []
// 			  X
//			[] []
//wall position will be in the X and based on his propreties (Horizontal/Vertical) will block the respective Player Position

uint32_t lastMove;
//uint8_t started = 0;
uint8_t started = 1; /********************REMOVE THIS WHEN FINISH DEBUGGING***************/

void Setup(){
	if(!started){
		PeripheralInit();
		Waiting_Player();
	}
	else{
		PeripheralInit(); /******************REMOVE THIS WHEN FINISH DEBUGGING******************/
		Game_Init();
	}
}

void Waiting_Player(){
	GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
	LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_XPOS + READY_UI_WIDTH, READY_UI_YPOS + READY_UI_HEIGHT, Blue2, NORTH_OVEST, DarkGray);
	GUI_Text(READY_UI_XPOS+20, READY_UI_YPOS+(READY_UI_HEIGHT/2)-5, (uint8_t *)"PRESS INT0 TO START", Black, White);
	
	GUI_Text(70, MAX_Y-20, (uint8_t *)"FELIZ", ChristmasRed, White);
	GUI_Text(115, MAX_Y-20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}


void Position_Wall(uint8_t x, uint8_t y, WALL_DIRECTION wall_dir){
	struct Point pos; 
	// Convert Matrix to Spatial position
	if(wall_dir == Vertical){
		pos.x = x * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE; 
		pos.y = y * (SQUARE_SIZE + WALL_WIDTH);
	} else { //if Horizontal
		pos.x = x * (SQUARE_SIZE + WALL_WIDTH);
		pos.y = y * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE;
	}
		
	
	Draw_Wall(pos.x, pos.y, wall_dir);
}

void Update_Wall_Counter(){
	char WallCounter0[2],WallCounter1[2];
	
	currWallUI[0] = player0.wallsRemaining;
	currWallUI[1] = player1.wallsRemaining;
	
	sprintf(WallCounter0, "%u", player0.wallsRemaining);
	sprintf(WallCounter1, "%u", player1.wallsRemaining);
	
	GUI_Text(P1_UI_XPOS+P_UI_WIDTH/2 - 5, P1_UI_YPOS+UI_HEIGHT-20, (uint8_t *)WallCounter0, Black, White);
	GUI_Text(P2_UI_XPOS+P_UI_WIDTH/2 - 5, P2_UI_YPOS+UI_HEIGHT-20, (uint8_t *)WallCounter1, Black, White);
}

void Update_Timer_Left(){
	char valueString[2];
	sprintf(valueString, "%u", timeLeft);
	

	GUI_Text(T_UI_XPOS+T_UI_WIDTH/2 - 5, T_UI_YPOS+UI_HEIGHT-20, (uint8_t *)valueString, Black, White);

}

void End_Turn(){
	currPlayer ^= 1; 			//alternate between 0 and 1
	timeLeft = MAX_TIME;
	Update_Timer_Left();
}

void PeripheralInit(){
	LCD_Initialization();
	
	init_timer(0, 0x17D7840 );							/* 1s    * 25MHz = 25*10^6   = 0x17D7840 */
	//init_timer(0, 0x1312D0 ); 						/* 50ms  * 25MHz = 1.25*10^6 = 0x1312D0  */
	//init_timer(0, 0x6108 ); 						  /* 1ms   * 25MHz = 25*10^3   = 0x6108    */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2     */
	
	init_RIT(0x1312D0); 										/* 50ms  * 25MHz = 1.25*10^6 = 0x1312D0  */
	BUTTON_init();
	disable_button(1);
	disable_button(2);
	joystick_init();
}
