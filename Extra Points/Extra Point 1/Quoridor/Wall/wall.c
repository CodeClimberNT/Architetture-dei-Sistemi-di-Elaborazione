#include "wall.h"

extern GAME_STATE game_state;
extern uint8_t current_player;
extern struct Player player0;
extern struct Player player1;

extern struct UI player0_ui;
extern struct UI player1_ui;

extern WALL_DIRECTION WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL];

void Draw_Wall() {
  uint8_t i, j;
  struct Wall m_wall;

  m_wall.discount = 0;
  m_wall.color = WallColor;

  for (j = 0; j < NUM_ROWS_WALL; j++) {
    for (i = 0; i < NUM_COLUMNS_WALL; i++) {
      if (WallMatrixPosition[i][j] == 0)  // If 0 no wall to place
        continue;

      m_wall.position.x = i;
      m_wall.position.y = j;
      m_wall.direction = WallMatrixPosition[i][j];
      Preview_Wall(m_wall);
    }
  }
}

struct Wall Place_Wall(struct Wall m_wall) {
  game_state = TRANSITION;

  if (!Can_Place_Wall(m_wall)) {
    return m_wall;  // If wall can't be place do nothing
  }

  m_wall.color = WallColor;
  m_wall.discount = 0;
  Preview_Wall(m_wall);
  WallMatrixPosition[m_wall.position.x][m_wall.position.y] = m_wall.direction;  // store wall direction to the matrix
  if (current_player == 0) {
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

  // Wrapping board to make positioning of wall more efficient
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

struct Wall Rotate_Wall(struct Wall m_wall) {
  Remove_Wall(m_wall);

  m_wall.direction = m_wall.direction == Horizontal ? Vertical : Horizontal;  // flip direction

  Preview_Wall(m_wall);

  return m_wall;
}

void Remove_Wall(struct Wall m_wall) {
  // Remove this wall and redraw all walls
  LCD_ClearRect(Get_Position_Of(m_wall));
  Draw_Wall();
}

struct Rect Get_Position_Of(struct Wall m_wall) {
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
