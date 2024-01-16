#include "player.h"

extern uint8_t current_player;
extern struct Player player0;
extern struct Player player1;
extern WALL_DIRECTION WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL];

struct Player Create_Player(uint8_t id, uint16_t x, uint16_t y, uint16_t color, uint8_t walls) {
  struct Player m_player;

  m_player.id = id;
  m_player.pos.x = x;
  m_player.pos.y = y;
  m_player.wallsRemaining = walls;
  m_player.color = color;
  m_player.ghost = 0;

  return m_player;
};

void Draw_Player(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // board position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH) + 1;
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH) + 1;

  LCD_FillSquare(m_x0, m_y0, PLAYER_SIZE, m_player.color);
}

uint8_t Player_Collide_Wall(struct Vector2D p_pos, DIRECTION dir, uint8_t is_double) {
	if(!is_double)
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
	//if moving two square
	switch (dir) {
    case RIGHT:
      return !(((p_pos.x < (NUM_COLUMNS - 2)) && (p_pos.y == 0) &&
                (WallMatrixPosition[p_pos.x + 1][p_pos.y] != Vertical)) ||
               ((p_pos.x < (NUM_COLUMNS - 2)) && (p_pos.y > 0) &&
                ((WallMatrixPosition[p_pos.x + 1][p_pos.y] != Vertical) && (WallMatrixPosition[p_pos.x + 1][p_pos.y - 1] != Vertical))));

    case LEFT:
      return !(((p_pos.x > 1) && (p_pos.y == 0) &&
                (WallMatrixPosition[p_pos.x - 2][p_pos.y] != Vertical)) ||
               ((p_pos.x > 1) && (p_pos.y > 0) &&
                ((WallMatrixPosition[p_pos.x - 2][p_pos.y] != Vertical) && (WallMatrixPosition[p_pos.x - 2][p_pos.y - 1] != Vertical))));

    case UP:
      return !(((p_pos.x == 0) && (p_pos.y > 1) &&
                (WallMatrixPosition[p_pos.x][p_pos.y - 2] != Horizontal)) ||
               ((p_pos.x > 0) && (p_pos.y > 1) &&
                ((WallMatrixPosition[p_pos.x][p_pos.y - 2] != Horizontal) && (WallMatrixPosition[p_pos.x - 1][p_pos.y - 2] != Horizontal))));

    case DOWN:
      return !(((p_pos.x == 0) && (p_pos.y < (NUM_ROWS - 2)) &&
                (WallMatrixPosition[p_pos.x][p_pos.y - 1] != Horizontal)) ||
               ((p_pos.x > 0) && (p_pos.y < (NUM_ROWS - 2)) &&
                ((WallMatrixPosition[p_pos.x][p_pos.y - 1] != Horizontal) && (WallMatrixPosition[p_pos.x - 1][p_pos.y - 1] != Horizontal))));
    default:
      GUI_Text(0, 0, (uint8_t *)"Check_Move_Player ERROR DIR", Black, White);
      return 0;
  }
}

void Move_Player(struct Player *p_player, DIRECTION dir, uint8_t is_double) {
	struct Player temp_p;
  struct Vector2D m_vec2d = Get_Vec_From_Dir(dir);
	temp_p.color = p_player->color;
	temp_p.pos = p_player->pos;
	
  if (is_double) {
    m_vec2d.x = m_vec2d.x * 2;
    m_vec2d.y = m_vec2d.y * 2;
  }

  if (!Player_Collide_Wall(temp_p.pos, dir, is_double)) {
    // check if position overlap player, then try two move forward
    if ((((temp_p.pos.x + m_vec2d.x == player0.pos.x) && (temp_p.pos.y + m_vec2d.y == player0.pos.y)) ||
        ((temp_p.pos.x + m_vec2d.x == player1.pos.x) && (temp_p.pos.y + m_vec2d.y == player1.pos.y))) && !is_double) {
      Move_Player(p_player, dir, 1);
    }

    if (!temp_p.ghost) {
      Remove_Hint_Move(temp_p);
      Remove_Player(temp_p);
    }

    temp_p.pos.x += m_vec2d.x;
    temp_p.pos.y += m_vec2d.y;
    // draw player new position
    Draw_Player(temp_p);

    if (!p_player->ghost) {
      p_player->pos.x = temp_p.pos.x;
			p_player->pos.x = temp_p.pos.y;
			
      End_Turn(0);
    }
  }
}

void Remove_Player(struct Player m_player) {
  m_player.color = White;
  Draw_Player(m_player);
}

void Create_Hint_Move(struct Player m_player) {
  Create_Highlight_Player(m_player);
  m_player.color = PhantomPlayerColor;
  m_player.ghost = 1;
  Move_Player(&m_player, UP, 0);
  Move_Player(&m_player, RIGHT, 0);
  Move_Player(&m_player, DOWN, 0);
  Move_Player(&m_player, LEFT, 0);
}

void Remove_Hint_Move(struct Player m_player) {
  Remove_Highlight_Player(m_player);
  m_player.color = GameBG;
  m_player.ghost = 1;
  Move_Player(&m_player, UP, 0);
  Move_Player(&m_player, RIGHT, 0);
  Move_Player(&m_player, DOWN, 0);
  Move_Player(&m_player, LEFT, 0);
}

void Create_Highlight_Player(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // player position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH);
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH);

  LCD_DrawSquare(m_x0, m_y0, SQUARE_SIZE, Yellow);
}

void Remove_Highlight_Player(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // player position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH);
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH);

  LCD_DrawSquare(m_x0, m_y0, SQUARE_SIZE, Black);
}

void Create_Highlight_Move(struct Player m_player, DIRECTION dir){
	m_player.color = Yellow;
	m_player.ghost = 1;
	Move_Player(&m_player, dir, 0);
}
