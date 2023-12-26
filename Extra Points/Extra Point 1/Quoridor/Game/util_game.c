#include "game.h"

void wait_delay(int count)
{
	while(count--);
}


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
			LCD_DrawLine(x1+1, y0+1, x1+1, y1+1, color);
			LCD_DrawLine(x0+1, y1+1, x1+1, y1+1, color);
			break;
		case SUD:
			LCD_DrawLine(x0+1, y1+1, x1-1, y1+1, color);
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


void LCD_DrawRectWithShadow( uint16_t x0, uint16_t y0, uint16_t width, uint16_t heigth, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color ){
	LCD_DrawRect(x0, y0, x0+width, y0+heigth, rect_color);
	LCD_DrawShadow(x0, y0, x0+width, y0+heigth, dir, shadow_color);
}

void LCD_DrawSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color ){
	LCD_DrawRect(x0, y0, x0+len, y0+len, color); //square is just a special Rect
}

void LCD_FillRect( uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1 , uint16_t color ){
	uint16_t i;
	
	for(i=y0;i<=y1;i++){
		LCD_DrawLine(x0, i, x1, i, color);
	}
}

void LCD_FillSquare( uint16_t x0, uint16_t y0, uint16_t len, uint16_t color ){
	LCD_FillRect(x0, y0, x0+len, y0+len, color);
}

void Update_Board(struct Player player, struct Wall *WallMatrixPosition){
	uint8_t i,j;
	uint16_t rows = NUM_ROWS*2-1;
	uint16_t cols = NUM_COLUMNS*2-1;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);

			LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
		}
	}
	
}


void Update_UI(struct UI ui){
		GUI_Text(ui.value_position.x, ui.value_position.y, (uint8_t *) ui.value_text, Black, White);
}

void Draw_Checkers(){
	uint8_t i,j;

		for(i=0; i<NUM_ROWS; i++){
			for(j=0; j<NUM_COLUMNS; j++){
				uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
				uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);

				LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
			}
		}
}
void Position_Player(struct Player player){
	uint16_t x0,y0;
	
	//board position to spatial position
	x0 = player.Position.x * (SQUARE_SIZE + WALL_WIDTH)+1;
	y0 = player.Position.y * (SQUARE_SIZE + WALL_WIDTH)+1;

	LCD_FillSquare(x0,y0,PLAYER_SIZE, player.color);
}

struct Player Move_Player(struct Player player, uint8_t new_x, uint8_t new_y){
	uint16_t x0,y0,x1,y1;
	
	//board position to spatial position
	x0 = player.Position.x * (SQUARE_SIZE + WALL_WIDTH)+1;
	y0 = player.Position.y * (SQUARE_SIZE + WALL_WIDTH)+1;
	//erase player old position
	LCD_FillSquare(x0,y0,PLAYER_SIZE, GameBG);
	
	//update player to new position
	player.Position.x = new_x;
	player.Position.y = new_y;
	
	//board position to spatial position
	x1 = player.Position.x * (SQUARE_SIZE + WALL_WIDTH)+1;
	y1 = player.Position.y * (SQUARE_SIZE + WALL_WIDTH)+1;
	
	//draw player new position
	LCD_FillSquare(x1,y1,PLAYER_SIZE, player.color);
	
	return player;
}

void Position_Wall(uint8_t x, uint8_t y, WALL_DIRECTION wall_dir, uint8_t isPhantom, uint16_t color){
	
	uint16_t x0,y0,x1,y1,discount = 2;
	
	
	// Convert Matrix to Spatial position
	if(wall_dir == Vertical){
		x0 = x * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE; 
		y0 = y * (SQUARE_SIZE + WALL_WIDTH);
	} else { //if Horizontal
		x0 = x * (SQUARE_SIZE + WALL_WIDTH);
		y0 = y * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE;
	}
		
	
	//Draw Wall

	
	switch (wall_dir) {
		case Horizontal:
			x1 = x0 + WALL_LENGTH;
			y1 = y0 + WALL_WIDTH ;
			break;
		case Vertical:
			x1 = x0 + WALL_WIDTH;
			y1 = y0 + WALL_LENGTH;
			break;
		default:
			return;
	}
	if(isPhantom && color != GameBG){
		color = PhantomWallColor;
	}
	if(isPhantom){
		x0 += discount; 
		y0 += discount;
		x1 -= discount; 
		y1 -= discount;
	}
	
	
	LCD_FillRect(x0, y0, x1, y1, color);
}

void Move_Wall(uint8_t prev_x, uint8_t prev_y, WALL_DIRECTION prev_wall_dir, uint8_t new_x, uint8_t new_y, WALL_DIRECTION new_wall_dir){
	uint8_t isPhantom = 1;
	Position_Wall(prev_x, prev_y, prev_wall_dir, isPhantom, GameBG);
	Position_Wall(new_x, new_y, new_wall_dir, isPhantom, PhantomWallColor);
}
