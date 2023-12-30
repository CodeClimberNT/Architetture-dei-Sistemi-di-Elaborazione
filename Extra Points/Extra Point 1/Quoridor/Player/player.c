#include "player.h"

volatile struct Player player0;
volatile struct Player player1;

void Player_Init() {
  player0 = Create_Player(0, 3, 6, P0_Color);
  player1 = Create_Player(1, 3, 0, P1_Color);
}

struct Player Create_Player(uint8_t m_id, uint16_t m_x, uint16_t m_y, uint16_t m_color) {
  struct Player m_player;

  m_player.id = m_id;
  m_player.board_pos.x = m_x;
  m_player.board_pos.y = m_y;
	
	m_player.screen_pos.x = m_player.board_pos.x * (SQUARE_SIZE + WALL_WIDTH) + 1;
	m_player.screen_pos.y = m_player.board_pos.y * (SQUARE_SIZE + WALL_WIDTH) + 1;
	m_player.size = PLAYER_SIZE;
	m_player.color = m_color;
	m_player.wallsRemaining = MAX_WALLS;
	

  return m_player;
};


void Position_Player(struct Player m_player) {
  LCD_FillSquare(m_player.screen_pos.x, m_player.screen_pos.y, m_player.size, m_player.color);
}

struct Player Move_Player(struct Player m_player, DIRECTION dir) {
  struct Vector2D m_vec2d = Get_Relative_Pos(dir);

  // check movement overflow
  if ((dir == RIGHT && m_player.board_pos.x != (NUM_COLUMNS - 1)) ||   // RIGHT border
      (dir == LEFT  && m_player.board_pos.x != (0))               ||   // LEFT border
      (dir == UP    && m_player.board_pos.y != (0))               ||   // TOP border
      (dir == DOWN  && m_player.board_pos.y != (NUM_ROWS - 1)))        // BOTTOM border
  {
    Remove_Player(m_player);

    m_player.board_pos.x += m_vec2d.x;
    m_player.board_pos.y += m_vec2d.y;
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
