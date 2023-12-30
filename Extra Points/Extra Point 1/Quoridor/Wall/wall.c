#include "wall.h"

extern uint8_t WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL];
extern GAME_STATE game_state;


void Update_Wall(struct Rect m_rect){
	LCD_ClearRect(m_rect);
	Draw_Wall();
}

void Draw_Wall(){
	uint8_t i, j;
	struct Wall m_wall;
	
	m_wall.color = WallColor;
	
	for (j = 0; j < NUM_ROWS_WALL; j++) {
		for (i = 0; i < NUM_COLUMNS_WALL; i++) {
			if(WallMatrixPosition[i][j]==0)
				continue;
			
      m_wall.position.x = i;
      m_wall.position.y = j;
			m_wall.direction = WallMatrixPosition[i][j] == 1 ? Horizontal : Vertical; //if 1 horizontal else Vertical
			Preview_Wall(m_wall);
			
    }
  }
}

void Place_Wall(struct Wall m_wall){
	game_state = TRANSITION;
	if(!Can_Place_Wall(m_wall))
	 return; //If wall can't be place do nothing

	m_wall.color = WallColor;
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
	
  return Preview_Wall(m_wall);
}

struct Wall Move_Wall(struct Wall m_wall, DIRECTION direction) {
  struct Vector2D m_vec2d = Get_Relative_Pos(direction);

  Remove_Wall(m_wall);

  // update new position
  m_wall.position.x = (m_wall.position.x + m_vec2d.x);
  m_wall.position.y = (m_wall.position.y + m_vec2d.y);

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

  // remove previous preview
  Remove_Wall(m_wall);

  // update new direction
  m_wall.direction ^= 1; //flip direction

  // draw new preview
  Preview_Wall(m_wall);

  return m_wall;
}

void Remove_Wall(struct Wall m_wall) {
	struct Rect rect = Get_Position_Of(m_wall);
	rect.x0-=1;
	rect.x1+=1;
	rect.y1+=1;
  Update_Wall(rect);
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
