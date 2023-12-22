/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../joystick/joystick.h"
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


void GameInit(){
	BoardInit();
	UIInit();
	return;
}

void BoardInit(){
	uint8_t i,j;

	for(i=0; i<NUM_ROWS; i++){
		for(j=0; j<NUM_COLUMNS; j++){
			uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);
			LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
		}
	}

}

void UIInit(){
	LCD_DrawRect(P1_UI_XPOS, P1_UI_YPOS, P1_UI_XPOS + P_UI_WIDTH, P1_UI_YPOS + P_UI_HEIGHT, Yellow);		//P1 UI
	LCD_DrawRect(T_UI_XPOS, T_UI_YPOS, T_UI_XPOS + T_UI_WIDTH, T_UI_YPOS + T_UI_HEIGHT, Red); 					//TIMER
	LCD_DrawRect(P2_UI_XPOS, P2_UI_YPOS, P2_UI_XPOS + P_UI_WIDTH, P2_UI_YPOS + P_UI_HEIGHT, Cyan); 			//P2 UI
	
	return;
}
