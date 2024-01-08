/*--------------File Info---------------------------------------------------------------------------------
** File name:               util_game.c
** Descriptions:            The Game Utility File
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Nicolò Taormina
** Created date:            20/12/2023
** Version:                 v1.0
*********************************************************************************************************/


#include "game.h"

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;
extern WALL_DIRECTION WallMatrixPosition[NUM_ROWS - 1][NUM_COLUMNS - 1];

extern struct UI timer_ui;
extern char time_value[2];
extern uint8_t timeLeft;

extern struct UI player0_ui;
extern char p0_wall_remaining[2];

extern struct UI player1_ui;
extern char p1_wall_remaining[2];

extern struct Player player0;
extern struct Player player1;

extern uint8_t current_player;



//uint16_t wall_color_buffer[WALL_COLOR_BUFFER_LENGTH][WALL_COLOR_BUFFER_HEIGHT];

void Peripheral_Init() {
  LCD_Initialization();
  //0x26259F
  init_timer(0, 0x26259F); 								/* 1s    * 25MHz = 25*10^6   = 0x17D7840 */
  // init_timer(0, 0x1312D0 ); 						/* 50ms  * 25MHz = 1.25*10^6 = 0x1312D0  */
  // init_timer(0, 0x6108 ); 						  /* 1ms   * 25MHz = 25*10^3   = 0x6108    */
  // init_timer(0, 0x4E2 ); 						  /* 500us * 25MHz = 1.25*10^3 = 0x4E2     */
	
	//0x7A120
  init_RIT(0x7A120); /* 50ms  * 100MHz = 5*10^6 = 0x4C4B40  */
	enable_RIT();
	disable_timer(0);
  BUTTON_init();
  joystick_init();
}

void wait_delay(int count) {
  while (count--)
    ;
}

void LCD_DrawRect(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, uint16_t color) {
  LCD_DrawLine(m_x0, m_y0, m_x1, m_y0, color);      // top edge
  LCD_DrawLine(m_x1, m_y0, m_x1, m_y1, color);    // right edge
  LCD_DrawLine(m_x1, m_y1, m_x0, m_y1, color);  // down edge
  LCD_DrawLine(m_x0, m_y1, m_x0, m_y0, color);    // left edge
}

void LCD_DrawShadow(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, SHADOW_DIRECTION direction, uint16_t color) {
  switch (direction) {
    case NORTH:
      LCD_DrawLine(m_x0 + 1, m_y0 - 1, m_x1 - 1, m_y0 - 1, color);
      break;
    case NORTH_EAST:
      LCD_DrawLine(m_x0 + 1, m_y0 - 1, m_x1 + 1, m_y0 - 1, color);
      LCD_DrawLine(m_x1 + 1, m_y0 - 1, m_x1 + 1, m_y1 - 1, color);
      break;
    case EAST:
      LCD_DrawLine(m_x1 + 1, m_y0 + 1, m_x1 + 1, m_y1 - 1, color);
      break;
    case SUD_EAST:
      LCD_DrawLine(m_x1 + 1, m_y0 + 1, m_x1 + 1, m_y1 + 1, color);
      LCD_DrawLine(m_x0 + 1, m_y1 + 1, m_x1 + 1, m_y1 + 1, color);
      break;
    case SUD:
      LCD_DrawLine(m_x0 + 1, m_y1 + 1, m_x1 - 1, m_y1 + 1, color);
      break;
    case SUD_OVEST:
      LCD_DrawLine(m_x0 - 1, m_y0 + 1, m_x0 - 1, m_y1 + 1, color);
      LCD_DrawLine(m_x0 - 1, m_y1 + 1, m_x1 - 1, m_y1 + 1, color);
      break;
    case OVEST:
      LCD_DrawLine(m_x0 - 1, m_y0 + 1, m_x0 - 1, m_y1 - 1, color);
      break;
    case NORTH_OVEST:
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x1 - 1, m_y0 - 1, color);
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x0 - 1, m_y1 - 1, color);
      break;
    case OVEST_NORTH_EAST:
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x0 - 1, m_y1 - 1, color);    
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x1 + 1, m_y0 - 1, color);    
      LCD_DrawLine(m_x1 + 1, m_y0 - 1, m_x1 + 1, m_y1 - 1, color);    
      break;
  }
}

void LCD_DrawRectWithShadow(uint16_t m_x0, uint16_t m_y0, uint16_t width, uint16_t heigth, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color) {
  LCD_DrawRect(m_x0, m_y0, m_x0 + width, m_y0 + heigth, rect_color);
  LCD_DrawShadow(m_x0, m_y0, m_x0 + width, m_y0 + heigth, dir, shadow_color);
}

void LCD_DrawSquare(uint16_t m_x0, uint16_t m_y0, uint16_t len, uint16_t color) {
  LCD_DrawRect(m_x0, m_y0, m_x0 + len, m_y0 + len, color);  // square is just a special Rect
}

void LCD_FillRect(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, uint16_t color) {
  uint16_t i;

  for (i = m_y0; i <= m_y1; i++) {
    LCD_DrawLine(m_x0, i, m_x1, i, color);
  }
}

void LCD_FillSquare(uint16_t m_x0, uint16_t m_y0, uint16_t len, uint16_t color) {
  LCD_FillRect(m_x0, m_y0, m_x0 + len, m_y0 + len, color);
}

void LCD_ClearRect(struct Rect m_rect){
	LCD_FillRect(m_rect.x0, m_rect.y0, m_rect.x1, m_rect.y1, GameBG);	
}

void LCD_ClearSquare(struct Vector2D m_start_pos, uint16_t m_len){
	LCD_FillSquare(m_start_pos.x, m_start_pos.y, m_len, GameBG);	
}

struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y, char *title_text,
                    uint16_t val_x, uint16_t val_y, char *value_text) {
  struct UI m_ui;

  m_ui.id = id;

  m_ui.ui_Position.x = ui_x;
  m_ui.ui_Position.y = ui_y;
  m_ui.height = height;
  m_ui.width = width;

  m_ui.title_position.x = tit_x;
  m_ui.title_position.y = tit_y;
  m_ui.title_text = title_text;
  m_ui.value_position.x = val_x;
  m_ui.value_position.y = val_y;
  m_ui.value_text = value_text;

  return m_ui;
}

void Update_UI(struct UI m_ui) {
  // erase previous text
  LCD_FillRect(m_ui.value_position.x, m_ui.value_position.y, m_ui.value_position.x + 20, m_ui.value_position.y + 15, GameBG);
  // update new text
  GUI_Text(m_ui.value_position.x, m_ui.value_position.y, (uint8_t *)m_ui.value_text, Black, White);
}

void Update_Timer_UI(uint8_t value) {
  sprintf(time_value, "%u", value);
  Update_UI(timer_ui);
}

void Update_Wall_UI(struct UI m_player_ui, uint8_t wall_value){
	switch(m_player_ui.id){
		case 0:
			sprintf(p0_wall_remaining, "%u", wall_value);
			Update_UI(player0_ui);
			break;
		
		case 1:
			sprintf(p1_wall_remaining, "%u", wall_value);
			Update_UI(player1_ui);
			break;
		
		default:
			GUI_Text(0,0, (uint8_t *)"WRONG PLAYER ID", Black, White);
	
	}
}

void Draw_Checkers() {
  uint8_t i, j;
	
	for (j = 0; j < NUM_ROWS; j++) {
		for (i = 0; i < NUM_COLUMNS; i++) {
      uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);
      LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
			
			if(player0.pos.x == i && player0.pos.y == j)
				Position_Player(player0);
			
			if(player1.pos.x == i && player1.pos.y == j)
				Position_Player(player1);
    }
  }
}



void Position_Player(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // board position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH) + 1;
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH) + 1;

  LCD_FillSquare(m_x0, m_y0, PLAYER_SIZE, m_player.color);
}

uint8_t Player_Collide_Wall(struct Vector2D p_pos, DIRECTION dir) {
  switch (dir) {
    case RIGHT:
      return !(((p_pos.x < (NUM_COLUMNS - 1)) && (p_pos.y == 0) &&
                (WallMatrixPosition[p_pos.x][p_pos.y] != Vertical)) ||
               ((p_pos.x < (NUM_COLUMNS - 1)) && (p_pos.y > 0) &&
                ((WallMatrixPosition[p_pos.x][p_pos.y] != Vertical) && (WallMatrixPosition[p_pos.x][p_pos.y - 1] != Vertical))));

    case LEFT:
      return !(((p_pos.x > 0) && (p_pos.y == 0) &&
                (WallMatrixPosition[p_pos.x - 1][p_pos.y] != Vertical)) ||
               ((p_pos.x > 0) && (p_pos.y > 0) &&
                ((WallMatrixPosition[p_pos.x - 1][p_pos.y] != Vertical) && (WallMatrixPosition[p_pos.x - 1][p_pos.y - 1] != Vertical))));

    case UP:
      return !(((p_pos.x == 0) && (p_pos.y > 0) &&
                (WallMatrixPosition[p_pos.x][p_pos.y - 1] != Horizontal)) ||
               ((p_pos.x > 0) && (p_pos.y > 0) &&
                ((WallMatrixPosition[p_pos.x][p_pos.y - 1] != Horizontal) && (WallMatrixPosition[p_pos.x - 1][p_pos.y - 1] != Horizontal))));

    case DOWN:
      return !(((p_pos.x == 0) && (p_pos.y < (NUM_ROWS - 1)) &&
                (WallMatrixPosition[p_pos.x][p_pos.y] != Horizontal)) ||
               ((p_pos.x > 0) && (p_pos.y < (NUM_ROWS - 1)) &&
                ((WallMatrixPosition[p_pos.x][p_pos.y] != Horizontal) && (WallMatrixPosition[p_pos.x - 1][p_pos.y] != Horizontal))));
    default:
      GUI_Text(0, 0, (uint8_t *)"Check_Move_Player ERROR DIR", Black, White);
      return 0;
  }
}

struct Player Move_Player(struct Player m_player, DIRECTION dir, uint8_t is_double) {
  struct Vector2D m_vec2d = Get_Vec_From_Dir(dir);
	
	if(is_double){
		m_vec2d.x = m_vec2d.x * 2;
		m_vec2d.y = m_vec2d.y * 2;
	}

	if (!Player_Collide_Wall(m_player.pos, dir)) {     
		//check if position overlap player, then try two move forward
		if(((m_player.pos.x + m_vec2d.x == player0.pos.x) && (m_player.pos.y + m_vec2d.y == player0.pos.y)) || 
				((m_player.pos.x + m_vec2d.x == player1.pos.x) && (m_player.pos.y + m_vec2d.y == player1.pos.y))){
			return Move_Player(m_player, dir, 1);
		 }		
				
		if(!m_player.ghost){
			Remove_Hint_Move(m_player);
			Remove_Player(m_player);
		}
			

    m_player.pos.x += m_vec2d.x;
    m_player.pos.y += m_vec2d.y;
    // draw player new position
    Position_Player(m_player);
		
		if(!m_player.ghost){
			if (current_player == 0){
				player0 = m_player;
			} else {
				player1 = m_player;
			}
			
			End_Turn(0);  
		}
  } 
	
  return m_player;
}

void Remove_Player(struct Player m_player) {
  m_player.color = White;
  Position_Player(m_player);
}

void Create_Hint_Move(struct Player m_player){
	Create_Highlight(m_player);
	m_player.color = PhantomPlayerColor;
	m_player.ghost = 1;
	Move_Player(m_player, UP,0);
	Move_Player(m_player, RIGHT,0);
	Move_Player(m_player, DOWN,0);
	Move_Player(m_player, LEFT,0);
}

void Remove_Hint_Move(struct Player m_player){
	Remove_Highlight(m_player);
	m_player.color = GameBG;
	m_player.ghost = 1;
	Move_Player(m_player, UP,0);
	Move_Player(m_player, RIGHT,0);
	Move_Player(m_player, DOWN,0);
	Move_Player(m_player, LEFT,0);
}

void Create_Highlight(struct Player m_player){
	uint16_t m_x0, m_y0;

  // player position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH);
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH);
	
	LCD_DrawSquare(m_x0, m_y0, SQUARE_SIZE, Yellow);
}

void Remove_Highlight(struct Player m_player){
	uint16_t m_x0, m_y0;

  // player position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH);
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH);
	
	LCD_DrawSquare(m_x0, m_y0, SQUARE_SIZE, Black);
}

void Draw_Board() {
  Draw_Checkers();

  Position_Player(player0);
  Position_Player(player1);
	Create_Hint_Move(current_player == 0 ? player0 : player1);
  Draw_Wall();
}

void Draw_Wall(){
	uint8_t i, j;
	struct Wall m_wall;
	
	m_wall.discount = 0;
	m_wall.color = WallColor;
	
	for (j = 0; j < NUM_ROWS_WALL; j++) {
		for (i = 0; i < NUM_COLUMNS_WALL; i++) {
			if(WallMatrixPosition[i][j] == 0) //If 0 no wall to place
				continue;
			
      m_wall.position.x = i;
      m_wall.position.y = j;
			m_wall.direction = WallMatrixPosition[i][j];
			Preview_Wall(m_wall);
			
    }
  }
}

struct Wall Place_Wall(struct Wall m_wall){
	game_state = TRANSITION;
	
	if(!Can_Place_Wall(m_wall)){
		return m_wall; //If wall can't be place do nothing
	}
	
	m_wall.color = WallColor;
	m_wall.discount = 0;
	Preview_Wall(m_wall);
	WallMatrixPosition[m_wall.position.x][m_wall.position.y] = m_wall.direction; //store wall direction to the matrix
	if(current_player == 0){
		Update_Wall_UI(player0_ui, --player0.wallsRemaining);
	} else {
		Update_Wall_UI(player1_ui, --player1.wallsRemaining);
	}
	Remove_Hint_Move(current_player == 0 ? player0 : player1);
	End_Turn(0);
	return m_wall;
}


struct Wall Preview_Wall(struct Wall m_wall) {
	struct Rect m_rect = Get_Position_Of(m_wall);
	
  LCD_FillRect(m_rect.x0, m_rect.y0, m_rect.x1, m_rect.y1, m_wall.color);
  return m_wall;
}

struct Wall Create_Wall(struct Wall m_wall) {

  m_wall.position.x = 2;
  m_wall.position.y = 3;
  m_wall.direction = Horizontal;
  m_wall.color = PhantomWallColor;
	m_wall.discount = WALL_DISCOUNT;

  return Preview_Wall(m_wall);
}

struct Wall Move_Wall(struct Wall m_wall, DIRECTION direction) {
  struct Vector2D m_vec2d = Get_Vec_From_Dir(direction);

  Remove_Wall(m_wall);

  // update new position
  m_wall.position.x = (m_wall.position.x + m_vec2d.x);
  m_wall.position.y = (m_wall.position.y + m_vec2d.y);
	
	//Wrapping board to make positioning of wall more efficient
  if (m_wall.position.x < 0)
    m_wall.position.x = NUM_COLUMNS_WALL - 1;
  if (m_wall.position.y < 0)
    m_wall.position.y = NUM_ROWS_WALL - 1;

  m_wall.position.x %= NUM_COLUMNS_WALL;
  m_wall.position.y %= NUM_ROWS_WALL;

  // draw new position
  Preview_Wall(m_wall);

  return m_wall;
}

struct Wall Rotate_Wall(struct Wall m_wall){
	
  Remove_Wall(m_wall);
	
  m_wall.direction = m_wall.direction == Horizontal ? Vertical : Horizontal; //flip direction
	
  Preview_Wall(m_wall);

  return m_wall;
}

void Remove_Wall(struct Wall m_wall) {
	//Remove this wall and redraw all walls
	LCD_ClearRect(Get_Position_Of(m_wall));
	Draw_Wall();
}


uint8_t Can_Place_Wall(struct Wall m_wall) {
  uint8_t x = m_wall.position.x, y = m_wall.position.y;
  // default all side are true need to check if something block the wall from being positioned
  uint8_t up_side = 1, right_side = 1, down_side = 1, left_side = 1;

  if (WallMatrixPosition[x][y] != NA)
    return 0;

  switch (m_wall.direction) {
    case Horizontal:
      if (x > 0)
        left_side = WallMatrixPosition[x - 1][y] != Horizontal;

      if (x < (NUM_COLUMNS_WALL - 1))
        right_side = WallMatrixPosition[x + 1][y] != Horizontal;

      break;

    case Vertical:
      if (y > 0)
        up_side = WallMatrixPosition[x][y - 1] != Vertical;

      if (y < (NUM_ROWS_WALL - 1))
        down_side = WallMatrixPosition[x][y + 1] != Vertical;

      break;
    default:
      GUI_Text(0, 0, (uint8_t *)"ERROR WALL POSITION LOGIC: Can_Place_Wall", Black, White);
      return 0;
  }
	//if overlap return 0, don't need to check if board is partitioned
  return ((up_side && right_side && down_side && left_side) == 0) ? 0 : !Wall_Partition_Board(m_wall);
}


uint8_t Wall_Partition_Board(struct Wall m_wall){
	uint8_t i, j, m, n;
	uint8_t isPartitioned = 0;
	uint8_t tempBoard[NUM_COLUMNS + NUM_COLUMNS_WALL][NUM_ROWS + NUM_ROWS_WALL] = {0};
	
	//temp add wall to matrix to check partition
	WallMatrixPosition[m_wall.position.x][m_wall.position.y] = m_wall.direction;
	
	
	//setup temp board to have wall in position
	for(j=1,n=0; n<NUM_ROWS_WALL; n++, j+=2){
		for(i=1,m=0; m<NUM_COLUMNS_WALL; m++, i+=2){
			if(tempBoard[i][j]!=0 || WallMatrixPosition[m][n] == NA)
				continue;
			
			if(WallMatrixPosition[m][n] == Horizontal){
				tempBoard[i-1][j] = 1;
				tempBoard[i][j] = 1;
				tempBoard[i+1][j] = 1;
			} else { //Vertical
				tempBoard[i][j-1] = 1;
				tempBoard[i][j] = 1;
				tempBoard[i][j+1] = 1;
			}
		}
	}
	

	//check if P0 can reach other side
	isPartitioned = !isReachable(player0.pos, 0, tempBoard);

	//check if P1 can reach other side
	isPartitioned = isPartitioned || !isReachable(player1.pos, 6, tempBoard);
	
	
	//remove temp wall added
	WallMatrixPosition[m_wall.position.x][m_wall.position.y] = NA;
	return isPartitioned;
}

uint8_t isReachable(struct Vector2D start, uint8_t row_target, uint8_t tempBoard[NUM_COLUMNS + NUM_COLUMNS_WALL][NUM_ROWS + NUM_ROWS_WALL]){
	uint8_t i,j,k=0;
	
	//reset tempBoard player flooding value to 0
	for(j=0; j<NUM_ROWS; j+=2){
		for(i=0; i<NUM_COLUMNS; i+=2){
			tempBoard[i][j]=0; 
		}
	}
	tempBoard[start.x*2][start.y*2] = 2;
	
	row_target*=2;

	// worst case scenario need to flood the board n^2 times for the player possible moves 
	// (actually don't have all those wall to make the traversal of the board so long, 
	// so can be reduce but the board size is so small that it doesn't change much)
	while(++k <= (NUM_COLUMNS*NUM_ROWS)){
		
		for(j=0; j<NUM_ROWS+NUM_ROWS_WALL; j+=2){
			for(i=0; i<NUM_COLUMNS+NUM_COLUMNS_WALL; i+=2){
				if(tempBoard[i][j]==0)
					continue;
				
				//currect cell have value to flood
				if(tempBoard[i][j] == 2){
					//if flooding algorithm reach target row exit with true value
					if(j == row_target){
						return 1;
					}
					//not left border and don't have wall on left
					if(i>1 && tempBoard[i-1][j]!=1)
						tempBoard[i-2][j] = 2;
					
					//not right border and don't have wall on right
					if(i<(NUM_COLUMNS+NUM_COLUMNS_WALL-1) && tempBoard[i+1][j]!=1)
						tempBoard[i+2][j] = 2;
					
					//not up border and don't have wall on up
					if(j>1 && (tempBoard[i][j-1]!=1))
						tempBoard[i][j-2] = 2;
					
					//not bottom border and don't have wall on bottom
					if(j<(NUM_ROWS+NUM_ROWS_WALL-1) && (tempBoard[i][j+1]!=1))
						tempBoard[i][j+2] = 2;
				}
			}
    }
		
	}
	
	return 0;
}

struct Rect Get_Position_Of(struct Wall m_wall){
	struct Rect m_rect;
	
	// Convert Matrix to Spatial position
  switch (m_wall.direction) {
    case Horizontal:
			m_rect.x0 = m_wall.position.x * (SQUARE_SIZE + WALL_WIDTH) + 1;
			m_rect.y0 = m_wall.position.y * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE + 1;
		
      m_rect.x1 = m_rect.x0 + WALL_LENGTH - 2;
      m_rect.y1 = m_rect.y0 + WALL_WIDTH - 2;
      break;
		
    case Vertical:
			m_rect.x0 = m_wall.position.x * (SQUARE_SIZE + WALL_WIDTH) + SQUARE_SIZE + 1;
			m_rect.y0 = m_wall.position.y * (SQUARE_SIZE + WALL_WIDTH) + 1;
      
			m_rect.x1 = m_rect.x0 + WALL_WIDTH - 2;
      m_rect.y1 = m_rect.y0 + WALL_LENGTH - 2;
      break;
		
    default:
      GUI_Text(0, 0, (uint8_t *)"ERROR WALL DIRECTION", Black, White);
  }
	
	m_rect.x0 += m_wall.discount;
  m_rect.y0 += m_wall.discount;
  m_rect.x1 -= m_wall.discount;
  m_rect.y1 -= m_wall.discount;
	
	return m_rect;
}


struct Vector2D Get_Vec_From_Dir(DIRECTION dir) {
  struct Vector2D m_vec2d;

  switch (dir) {
    case UP:
      m_vec2d.x = 0;
      m_vec2d.y = -1;
      break;
    case RIGHT:
      m_vec2d.x = +1;
      m_vec2d.y = 0;
      break;
    case DOWN:
      m_vec2d.x = 0;
      m_vec2d.y = +1;
      break;
    case LEFT:
      m_vec2d.x = -1;
      m_vec2d.y = 0;
      break;
  }
  return m_vec2d;
}

DIRECTION Get_Dir_From_Vec(struct Vector2D vec2d) {
	//cannot move diagonally
	if(vec2d.x > 0)
		return RIGHT;
	if(vec2d.x < 0)
		return LEFT;
	if(vec2d.y > 0)
		return UP;

	return DOWN;
}
	
/**************Failed Implementation of Color Buffer*************************/
/*
void Create_Color_Buffer(uint16_t *p_buffer_vec, struct Vector2D m_start_pos, uint16_t m_buffer_lenght, uint16_t m_buffer_height){
	uint16_t i, j, x, y;
	for(j=0; j<=m_buffer_height; j++){
		for(i=0; i<=m_buffer_lenght; i++){
			x = i + m_start_pos.x;
			y = j + m_start_pos.y;
			p_buffer_vec[x * m_buffer_lenght + y] = LCD_GetPoint(x, y);
		}
	}
}

void Draw_Color_Buffer(uint16_t *p_buffer_vec, struct Vector2D m_start_pos, uint16_t m_buffer_lenght, uint16_t m_buffer_height){
	uint16_t i,j, x, y;
	for(j=0; j<=m_buffer_height; j++){
		for(i=0; i<=m_buffer_lenght; i++){
			x = i + m_start_pos.x;
			y = j + m_start_pos.y;
			LCD_SetPoint(x, y, p_buffer_vec[x * m_buffer_lenght + y]);
		}
	}
}

void Create_Wall_Color_Buffer(struct Wall m_wall){
	struct Vector2D m_start_pos;
	m_start_pos.x = m_wall.position.x * (SQUARE_SIZE + WALL_WIDTH);
	m_start_pos.y = m_wall.position.y * (SQUARE_SIZE + WALL_WIDTH);
	
	Create_Color_Buffer(&wall_color_buffer[0][0], m_start_pos, WALL_COLOR_BUFFER_LENGTH, WALL_COLOR_BUFFER_HEIGHT);
}

void Draw_Wall_Color_Buffer(struct Wall m_wall){
	struct Vector2D m_start_pos;
	m_start_pos.x = m_wall.position.x * (SQUARE_SIZE + WALL_WIDTH);
	m_start_pos.y = m_wall.position.y * (SQUARE_SIZE + WALL_WIDTH);
	
	Draw_Color_Buffer(&wall_color_buffer[0][0], m_start_pos, WALL_COLOR_BUFFER_LENGTH, WALL_COLOR_BUFFER_HEIGHT);
}
*/
