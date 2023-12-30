#include "game.h"

extern GAME_STATE game_state;
extern MOVING_ENTITY moving_entity;
extern uint8_t WallMatrixPosition[NUM_ROWS - 1][NUM_COLUMNS - 1];

extern struct UI timer_ui;
extern char time_value[2];
extern uint8_t timeLeft;

extern struct Player player0;
extern struct Player player1;

uint16_t wall_color_buffer[WALL_COLOR_BUFFER_LENGTH][WALL_COLOR_BUFFER_HEIGHT];

void Peripheral_Init() {
  LCD_Initialization();
  // 0x17D7840
  init_timer(0, 0x26259F); /* 1s    * 25MHz = 25*10^6   = 0x17D7840 */
  // init_timer(0, 0x1312D0 ); 						/* 50ms  * 25MHz = 1.25*10^6 = 0x1312D0  */
  // init_timer(0, 0x6108 ); 						  /* 1ms   * 25MHz = 25*10^3   = 0x6108    */
  // init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2     */

  init_RIT(0x7A120); /* 50ms  * 100MHz = 5*10^6 = 0x4C4B40  */
  BUTTON_init();
  disable_button(1);
  disable_button(2);
  joystick_init();
}

void Peripheral_Enable() {
  enable_timer(0);
  enable_RIT();
  enable_button(1);
  enable_button(2);
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

void Update_Timer(uint8_t value) {
  sprintf(time_value, "%u", value);
  Update_UI(timer_ui);
}


void Draw_Checkers() {
  uint8_t i, j;
	
	for (j = 0; j < NUM_ROWS; j++) {
		for (i = 0; i < NUM_COLUMNS; i++) {
      uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);
      LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
			
			if(player0.Position.x == i && player0.Position.y == j)
				Position_Player(player0);
			
			if(player1.Position.x == i && player1.Position.y == j)
				Position_Player(player1);
    }
  }
}

void Position_Player(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // board position to spatial position
  m_x0 = m_player.Position.x * (SQUARE_SIZE + WALL_WIDTH) + 1;
  m_y0 = m_player.Position.y * (SQUARE_SIZE + WALL_WIDTH) + 1;

  LCD_FillSquare(m_x0, m_y0, PLAYER_SIZE, m_player.color);
}

struct Player Move_Player(struct Player m_player, DIRECTION dir) {
  struct Vector2D m_vec2d = Get_Relative_Pos(dir);

  // check movement overflow
  if ((dir == RIGHT && m_player.Position.x != (NUM_COLUMNS - 1)) ||   // RIGHT border
      (dir == LEFT  && m_player.Position.x != (0))               ||   // LEFT border
      (dir == UP    && m_player.Position.y != (0))               ||   // TOP border
      (dir == DOWN  && m_player.Position.y != (NUM_ROWS - 1)))        // BOTTOM border
  {
    Remove_Player(m_player);

    m_player.Position.x += m_vec2d.x;
    m_player.Position.y += m_vec2d.y;
    // draw m_player new position
    Position_Player(m_player);
    End_Turn();  // don't end turn unless m_player move
  }

  return m_player;
}

void Remove_Player(struct Player m_player) {
  m_player.color = White;
  Position_Player(m_player);
}

void Draw_Wall(){
	uint8_t i, j;
	struct Wall m_wall;
	
	m_wall.discount = 0;
	m_wall.color = WallColor;
	
	for (j = 0; j < NUM_ROWS_WALL; j++) {
		for (i = 0; i < NUM_COLUMNS_WALL; i++) {
			if(WallMatrixPosition[i][j]==0) //If 0 no wall to place
				continue;
			
      m_wall.position.x = i;
      m_wall.position.y = j;
			m_wall.direction = (WallMatrixPosition[i][j] == 1) ? Horizontal : Vertical; //if 1 horizontal else Vertical
			Preview_Wall(m_wall);
			
    }
  }
}

void Place_Wall(struct Wall m_wall){
	game_state = TRANSITION;
	if(!Can_Place_Wall(m_wall))
	 return; //If wall can't be place do nothing

	m_wall.color = WallColor;
	m_wall.discount = 0;
	Preview_Wall(m_wall);
	WallMatrixPosition[m_wall.position.x][m_wall.position.y] = m_wall.direction == Horizontal ? 1 : 2; //if horizontal 1, else if vertical 2
	End_Turn();
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
	//m_wall.discount = 0;
  return Preview_Wall(m_wall);
}

struct Wall Move_Wall(struct Wall m_wall, DIRECTION direction) {
  struct Vector2D m_vec2d = Get_Relative_Pos(direction);

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
  m_wall.direction ^= 1; //flip direction
	
  Preview_Wall(m_wall);

  return m_wall;
}

void Remove_Wall(struct Wall m_wall) {
	//Remove this wall and redraw all walls
	LCD_ClearRect(Get_Position_Of(m_wall));
	Draw_Wall();
}

uint8_t Can_Place_Wall(struct Wall m_wall){
	//TODO IMPLEMENT LOGIC
	return 1; 
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


struct Vector2D Get_Relative_Pos(DIRECTION dir) {
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
