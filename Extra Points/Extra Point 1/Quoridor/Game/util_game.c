#include "game.h"

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;
extern uint8_t WallMatrixPosition [NUM_ROWS-1][NUM_COLUMNS-1];

void Peripheral_Init(){
	LCD_Initialization();
	//0x17D7840
	init_timer(0, 0x26259F); 								/* 1s    * 25MHz = 25*10^6   = 0x17D7840 */
	//init_timer(0, 0x1312D0 ); 						/* 50ms  * 25MHz = 1.25*10^6 = 0x1312D0  */
	//init_timer(0, 0x6108 ); 						  /* 1ms   * 25MHz = 25*10^3   = 0x6108    */
	//init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2     */
	
	init_RIT(0x7A120); 										/* 50ms  * 100MHz = 5*10^6 = 0x4C4B40  */
	BUTTON_init();
	disable_button(1);
	disable_button(2);
	joystick_init();
}

void Peripheral_Enable(){
	enable_timer(0);
	enable_RIT();
	enable_button(1);
	enable_button(2);
}

void wait_delay(int count){
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
			LCD_DrawLine(x0-1, y0+1, x0-1, y1+1, color);
			LCD_DrawLine(x0-1, y1+1, x1-1, y1+1, color);
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


struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y ,char *title_text,	
									uint16_t val_x, uint16_t val_y, char *value_text){
	struct UI ui; 
	
  ui.id = id;
										
	ui.ui_Position.x = ui_x;
	ui.ui_Position.y = ui_y;
	ui.height = height;
	ui.width = width;
	
  ui.title_position.x = tit_x;
	ui.title_position.y = tit_y;
	ui.title_text = title_text;
	ui.value_position.x = val_x;
	ui.value_position.y = val_y;
	ui.value_text = value_text;

	return ui;
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
	//erase previous text
	LCD_FillRect(ui.value_position.x, ui.value_position.y, ui.value_position.x + 20, ui.value_position.y + 15, GameBG);
	//update new text
	GUI_Text(ui.value_position.x, ui.value_position.y, (uint8_t *) ui.value_text, Black, White);
}

void Draw_Checkers(){
	uint8_t i,j;

		for(i=0; i<NUM_ROWS; i++){
			for(j=0; j<NUM_COLUMNS; j++){
				uint16_t x = j * (SQUARE_SIZE + WALL_WIDTH);
				uint16_t y = i * (SQUARE_SIZE + WALL_WIDTH);

				LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
			}
		}
}


void Position_Player(struct Player player){
	uint16_t x0,y0;
	
	//board position to spatial position
	x0 = player.Position.x * (SQUARE_SIZE + WALL_WIDTH)+1;
	y0 = player.Position.y * (SQUARE_SIZE + WALL_WIDTH)+1;

	LCD_FillSquare(x0, y0, PLAYER_SIZE, player.color);
}

struct Player Move_Player(struct Player player, struct Vector2D vec2d){
	//update player to new position
	//check movement overflow
	if(	( vec2d.x ==  1   && player.Position.x != (NUM_COLUMNS-1)) 	|| //RIGHT border
			( vec2d.x == -1   && player.Position.x != (0))							|| //LEFT border
			( vec2d.y == -1   && player.Position.y != (0))							|| //TOP border
			( vec2d.y ==  1   && player.Position.y != (NUM_ROWS-1))     ){ //BOTTOM border
		
		Remove_Player(player);

		player.Position.x += vec2d.x;
		player.Position.y += vec2d.y;
		//draw player new position
		Position_Player(player);
		End_Turn(); 																										//don't end turn unless player move
	}
	

	return player;
}

void Remove_Player(struct Player player){
	player.color = White;
	Position_Player(player);
}

struct Wall Create_Wall(struct Wall wall){
	uint8_t i=0,j=0;
	
	//find first free position spawn a wall
	for(i=0; WallMatrixPosition[i][j]==1; i++){
		for(j=0; WallMatrixPosition[i][j]==1 && j<NUM_COLUMNS_WALL; j++){ //increas j untill find a spot in the matrix or check all the rows
		}
	}
	
	wall.position.x = i;
	wall.position.y = j;
	wall.direction = Horizontal;
	wall.color = PhantomWallColor;
	
	return wall;
}

void Preview_Wall(struct Wall wall){
	uint16_t x0,y0,x1,y1,discount = 2;
	
	// Convert Matrix to Spatial position
	if(wall.direction == Vertical){
		x0 = wall.position.x * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE; 
		y0 = wall.position.y * (SQUARE_SIZE + WALL_WIDTH);
	} else { //if Horizontal
		x0 = wall.position.x * (SQUARE_SIZE + WALL_WIDTH);
		y0 = wall.position.y * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE;
	}
		
	//Draw Wall
	switch (wall.direction) {
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
	
	if(wall.isPhantom){
		x0 += discount; 
		y0 += discount;
		x1 -= discount; 
		y1 -= discount;
	}
	
	LCD_FillRect(x0, y0, x1, y1, wall.color);
}

struct Wall Move_Wall(struct Wall wall, struct Vector2D vec2d){
	struct Vector2D newPos;
	uint8_t i,j;
	newPos.x = (wall.position.x + vec2d.x) % NUM_COLUMNS_WALL;
	newPos.y = (wall.position.y + vec2d.y) % NUM_ROWS_WALL;
	
	//loop unless found a free spot
	while(WallMatrixPosition[newPos.x][newPos.y]){
		newPos.x = (++newPos.x)%NUM_COLUMNS_WALL;
		
		newPos.x = (++newPos.x)%NUM_COLUMNS_WALL;
	}
	
	for(; WallMatrixPosition[newPos.x][newPos.y]==1 && newPos.x < newPos.x;){}
	//remove previous position
	Remove_Wall(wall);
	
	
	
	
	//update new position
	wall.position = newPos;
	
	//draw new position
	Preview_Wall(wall);
	return wall;
}

struct Wall Rotate_Wall(struct Wall wall, WALL_DIRECTION new_dir){
	//remove previous preview
	Remove_Wall(wall);
	
	//update new direction
	wall.direction = new_dir;
	
	//draw new preview
	Preview_Wall(wall);
	
	return wall;
}

void Remove_Wall(struct Wall wall){
	wall.color = White;
	Preview_Wall(wall);
}


struct Vector2D GetPos(DIRECTION dir){
	struct Vector2D vec2d;
	
	switch(dir){
		case UP:
			vec2d.x = 0;
			vec2d.y = -1;
			break;
		case RIGHT:
			vec2d.x = +1;
			vec2d.y = 0;
			break;
		case DOWN:
			vec2d.x = 0;
			vec2d.y = +1;
			break;
		case LEFT:
			vec2d.x = -1;
			vec2d.y = 0;
			break;
	}
	return vec2d;
}
