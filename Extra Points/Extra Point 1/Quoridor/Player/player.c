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
	m_player.curr_dir = CENTER;
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

struct Player Move_Player(struct Player m_player, uint8_t is_double) {
  DIRECTION dir = m_player.curr_dir;
	struct Vector2D m_vec2d = Get_Vec_From_Dir(dir);
	
  if (is_double) {
    m_vec2d.x = m_vec2d.x * 2;
    m_vec2d.y = m_vec2d.y * 2;
  }

  if (!Player_Collide_Wall(m_player.pos, dir, is_double)) {
    // check if position overlap player, then try two move forward
    if (((m_player.pos.x + m_vec2d.x == player0.pos.x) && (m_player.pos.y + m_vec2d.y == player0.pos.y)) ||
        ((m_player.pos.x + m_vec2d.x == player1.pos.x) && (m_player.pos.y + m_vec2d.y == player1.pos.y))) {
      return Move_Player(m_player, 1);
    }

    if (!m_player.ghost) {
      Remove_Hint_Move(m_player);
      Remove_Player(m_player);
    }

    m_player.pos.x += m_vec2d.x;
    m_player.pos.y += m_vec2d.y;
    // draw player new position
    Draw_Player(m_player);

    if (!m_player.ghost) {
      if (current_player == 0) {
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
  Draw_Player(m_player);
}

void Highlight_Move(struct Player *p_player, DIRECTION dir){
	struct Player temp_p;
	temp_p.pos = p_player->pos;
	temp_p.ghost = 1;
	
	if(p_player->curr_dir != CENTER){ //If curr_dir not center means an higlight move already exist
		temp_p.curr_dir = p_player->curr_dir;
		temp_p.color = PhantomPlayerColor;
		Move_Player(temp_p, 0);
	}
	p_player->curr_dir = dir;
	temp_p.curr_dir = p_player->curr_dir;
	temp_p.color = Yellow;
	Move_Player(temp_p, 0);
	
}

void Create_Hint_Move(struct Player m_player) {
  Create_Highlight(m_player);
  m_player.color = PhantomPlayerColor;
  m_player.ghost = 1;
	
	m_player.curr_dir = UP;
  Move_Player(m_player, 0);
	m_player.curr_dir = RIGHT;
  Move_Player(m_player, 0);
  m_player.curr_dir = DOWN;
	Move_Player(m_player, 0);
	m_player.curr_dir = LEFT;
  Move_Player(m_player, 0);
}

void Remove_Hint_Move(struct Player m_player) {
  Remove_Highlight(m_player);
  m_player.color = GameBG;
  m_player.ghost = 1;
	
  m_player.curr_dir = UP;
  Move_Player(m_player, 0);
	m_player.curr_dir = RIGHT;
  Move_Player(m_player, 0);
  m_player.curr_dir = DOWN;
	Move_Player(m_player, 0);
	m_player.curr_dir = LEFT;
  Move_Player(m_player, 0);
}

void Create_Highlight(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // player position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH);
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH);

  LCD_DrawSquare(m_x0, m_y0, SQUARE_SIZE, Yellow);
}

void Remove_Highlight(struct Player m_player) {
  uint16_t m_x0, m_y0;

  // player position to spatial position
  m_x0 = m_player.pos.x * (SQUARE_SIZE + WALL_WIDTH);
  m_y0 = m_player.pos.y * (SQUARE_SIZE + WALL_WIDTH);

  LCD_DrawSquare(m_x0, m_y0, SQUARE_SIZE, Black);
}
