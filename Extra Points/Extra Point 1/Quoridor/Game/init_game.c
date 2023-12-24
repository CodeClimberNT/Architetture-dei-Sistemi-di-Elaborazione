/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "game.h"


/*
 White          0xFFFF
 Black          0x0000
 Grey           0xF7DE
 Blue           0x001F
 Blue2          0x051F
 Red            0xF800
 Magenta        0xF81F
 Green          0x07E0
 Cyan           0x7FFF
 Yellow         0xFFE0
*/
extern struct Player player0;
extern struct Player player1;
extern uint8_t timeLeft;

void Game_Init(){
	LCD_Clear(GameBG);
	Board_Init();
	UI_Init();
	Player_Init();
	Wall_Counter_Init();
	Timer_Counter_Init();
	return;
}

void Board_Init(){
	uint8_t i,j;

	for(i=0; i<NUM_ROWS; i++){
		for(j=0; j<NUM_COLUMNS; j++){
			uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);

			LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
		}
	}
	
	Position_Wall(0,0,Vertical);
	Position_Wall(5,5,Horizontal);
	Position_Wall(5,5,Vertical);
}

void UI_Init(){
	
	LCD_DrawRectWithShadow(P1_UI_XPOS, P1_UI_YPOS, P1_UI_XPOS + P_UI_WIDTH, P1_UI_YPOS + UI_HEIGHT, Black, NORTH_OVEST, Black); //P1 UI
	GUI_Text(P1_UI_XPOS+5, P1_UI_YPOS+5,(uint8_t *) "P1 WALL", Black, White);

	LCD_DrawRectWithShadow(P2_UI_XPOS, P2_UI_YPOS, P2_UI_XPOS + P_UI_WIDTH, P2_UI_YPOS + UI_HEIGHT, Black, NORTH_EAST, Black); //P2 UI
	GUI_Text(P2_UI_XPOS+5, P2_UI_YPOS+5,(uint8_t *) "P2 WALL", Black, White);
	
	LCD_DrawRectWithShadow(T_UI_XPOS, T_UI_YPOS, T_UI_XPOS + T_UI_WIDTH, T_UI_YPOS + UI_HEIGHT, Black, OVEST_NORTH_EAST, Black); //TIMER
	GUI_Text(T_UI_XPOS+7, T_UI_YPOS+5,(uint8_t *) "TIME LEFT", Black, White);
}

void Player_Init(){
	player0 = Create_Player(0);
	player1 = Create_Player(1);
}

struct Player Create_Player(uint8_t Id){
	uint16_t x = 3 ,y = 6; //value for player0
	struct Player defaultPlayer; 
	
	if(Id == 1)
		y=0;
	
  defaultPlayer.Id = Id;
  defaultPlayer.Position.x = x;
  defaultPlayer.Position.y = y;
  defaultPlayer.wallsRemaining = MAX_WALLS;
	
	 return defaultPlayer;
};


void Wall_Counter_Init(){
	Update_Wall_Counter();
}

void Timer_Counter_Init(){
	Update_Timer_Left();
}
