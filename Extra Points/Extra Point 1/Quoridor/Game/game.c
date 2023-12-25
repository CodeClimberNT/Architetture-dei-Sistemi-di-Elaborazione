#include "game.h"



volatile struct Player player0;
volatile struct Player player1;
volatile struct UI timer_ui;
volatile struct UI player0_ui;
volatile struct UI player1_ui;

volatile uint8_t currPlayer = 0;
volatile uint8_t timeLeft = MAX_TIME;
volatile uint8_t currWallUI[2];

uint8_t WallMatrixPosition [NUM_ROWS-1][NUM_COLUMNS-1] = {0}; 
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

void Peripheral_Init(){
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
