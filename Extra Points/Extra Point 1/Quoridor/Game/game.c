#include "game.h"

volatile struct Player player0;
volatile struct Player player1;

uint32_t lastMove;
uint8_t started = 0;

void Setup(){
	Waiting_Player();
}

void Waiting_Player(){
	GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
	LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_XPOS + READY_UI_WIDTH, READY_UI_YPOS + READY_UI_HEIGHT, Black, NORTH_OVEST, DarkGray);
	GUI_Text(READY_UI_XPOS+20, READY_UI_YPOS+(READY_UI_HEIGHT/2)-5, (uint8_t *)"PRESS INT0 TO START", Black, White);
	
	GUI_Text(70, MAX_Y-20, (uint8_t *)"FELIZ", ChristmasRed, White);
	GUI_Text(115, MAX_Y-20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}

uint8_t Update_Wall_Counter(uint8_t Id, uint8_t value){
	char valueString[2];
	sprintf(valueString, "%u", value);
	
	GUI_Text(P1_UI_XPOS+P_UI_WIDTH/2 - 5, P1_UI_YPOS+UI_HEIGHT-20, (uint8_t *)valueString, Black, White);
	GUI_Text(P2_UI_XPOS+P_UI_WIDTH/2 - 5, P2_UI_YPOS+UI_HEIGHT-20, (uint8_t *)valueString, Black, White);
	return value;
}
