#include "game.h"

void LCD_DrawRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ){
	LCD_DrawLine(x0, y0, x1, y0, color); //top edge
	LCD_DrawLine(x1, y0, x1, y1, color); //right edge
	LCD_DrawLine(x1, y1, x0, y1, color); //down edge
	LCD_DrawLine(x0, y1, x0, y0, color); //left edge
}

void LCD_DrawShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , SHADOW_DIRECTION dir, uint16_t color){

	switch(dir){
		case NORTH:
			LCD_DrawLine(x0+1, y0-1, x1-1, y0-1, color);
			break;
		case NORTH_EAST:
			LCD_DrawLine(x0+1, y0-1, x1+1, y0-1, color);
			LCD_DrawLine(x1+1, y0-1, x1+1, y1-1, color);
			break;
		case EAST:
			LCD_DrawLine(x1+1, y0+1, x1+1, y1-1, color);
			break;
		case SUD_EAST:
			GUI_Text(x0,y0,(uint8_t*)"TO IMPLEMENT", White,Black);
			break;
		case SUD:
			LCD_DrawLine(x0+1, y1+1, x1+1, y1+1, color);
			break;
		case SUD_OVEST:
			GUI_Text(x0,y0,(uint8_t*)"TO IMPLEMENT", White,Black);
			break;
		case OVEST:
			LCD_DrawLine(x0-1, y0+1, x0-1, y1-1, color);
			break;
		case NORTH_OVEST:
			LCD_DrawLine(x0-1, y0-1, x1-1, y0-1, color);
			LCD_DrawLine(x0-1, y0-1, x0-1, y1-1, color);
			break;
		case OVEST_NORTH_EAST:
			LCD_DrawLine(x0-1, y0-1, x0-1, y1-1, color);	//ovest
			LCD_DrawLine(x0-1, y0-1, x1+1, y0-1, color);  //north
			LCD_DrawLine(x1+1, y0-1, x1+1, y1-1, color);	//east
			break;
	}
	
}


void LCD_DrawRectWithShadow( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color ){
	LCD_DrawRect(x0, y0, x1, y1, rect_color);
	LCD_DrawShadow(x0, y0, x1, y1, dir, shadow_color);
}

void LCD_DrawSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color ){
	LCD_DrawRect(x0, y0, x0+len, y0+len, color); //square is just a special Rect
}

void LCD_FillRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ){
	uint16_t i;
	
	for(i=y0;i<y1;i++){
		LCD_DrawLine(x0, i, x1, i, color);
	}
}

void LCD_FillSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color ){
	LCD_FillRect(x0, y0, x0+len, y0+len, color);
}

void Draw_Wall(uint16_t x0, uint16_t y0, WALL_DIRECTION wall_dir){
	uint16_t x1, y1;
	switch (wall_dir) {
		case Horizontal:
			x1 = x0 + WALL_LENGTH;
			y1 = y0 + WALL_WIDTH;
			break;
		case Vertical:
			y1 = y0 + WALL_LENGTH+1; //off by one (only vertical !!), fix for now, later will try find the culprit
			x1 = x0 + WALL_WIDTH;
			break;
		default:
			return;
	}
	LCD_FillRect(x0,y0,x1,y1,DarkGray);
}

