#include "game.h"

volatile struct Player player0;
volatile struct Player player1;

uint32_t lastMove;

void Setup(){
	Game_Init();
}

uint8_t Update_Wall_Counter(uint8_t Id, uint8_t value){
	char valueString[2];
	sprintf(valueString, "%u", value);
	
	GUI_Text(P1_UI_XPOS+P_UI_WIDTH/2 - 5, P1_UI_YPOS+UI_HEIGHT-20, (uint8_t *)valueString, Black, White);
	GUI_Text(P2_UI_XPOS+P_UI_WIDTH/2 - 5, P2_UI_YPOS+UI_HEIGHT-20, (uint8_t *)valueString, Black, White);
	return value;
}
