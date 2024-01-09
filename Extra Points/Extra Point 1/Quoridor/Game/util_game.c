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

extern MOVING_ENTITY moving_entity;
extern WALL_DIRECTION WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL];

extern struct Player player0;
extern struct Player player1;

extern uint8_t current_player;

void Peripheral_Init() {
  LCD_Initialization();
  // 0x26259F
  init_timer(0, 0x26259F); /* 1s    * 25MHz = 25*10^6   = 0x17D7840 */

  // 0x7A120
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

void Draw_Checkers() {
  uint8_t i, j;

  for (j = 0; j < NUM_ROWS; j++) {
    for (i = 0; i < NUM_COLUMNS; i++) {
      uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);
      LCD_DrawSquare(x, y, SQUARE_SIZE, Black);

      if (player0.pos.x == i && player0.pos.y == j)
        Draw_Player(player0);

      if (player1.pos.x == i && player1.pos.y == j)
        Draw_Player(player1);
    }
  }
}

void Draw_Board() {
  Draw_Checkers();

  Draw_Player(player0);
  Draw_Player(player1);
  Create_Hint_Move(current_player == 0 ? player0 : player1);
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
  // if overlap return 0, don't need to check if board is partitioned
  return ((up_side && right_side && down_side && left_side) == 0) ? 0 : !Wall_Partition_Board(m_wall);
}

uint8_t Wall_Partition_Board(struct Wall m_wall) {
  uint8_t i, j, m, n;
  uint8_t isPartitioned = 0;
  uint8_t tempBoard[NUM_COLUMNS + NUM_COLUMNS_WALL][NUM_ROWS + NUM_ROWS_WALL] = {0};

  // temp add wall to matrix to check partition
  WallMatrixPosition[m_wall.position.x][m_wall.position.y] = m_wall.direction;

  // setup temp board to have wall in position
  for (j = 1, n = 0; n < NUM_ROWS_WALL; n++, j += 2) {
    for (i = 1, m = 0; m < NUM_COLUMNS_WALL; m++, i += 2) {
      if (tempBoard[i][j] != 0 || WallMatrixPosition[m][n] == NA)
        continue;

      if (WallMatrixPosition[m][n] == Horizontal) {
        tempBoard[i - 1][j] = 1;
        tempBoard[i][j] = 1;
        tempBoard[i + 1][j] = 1;
      } else {  // Vertical
        tempBoard[i][j - 1] = 1;
        tempBoard[i][j] = 1;
        tempBoard[i][j + 1] = 1;
      }
    }
  }

  // check if P0 can reach other side
  isPartitioned = !isReachable(player0.pos, 0, tempBoard);

  // check if P1 can reach other side
  isPartitioned = isPartitioned || !isReachable(player1.pos, 6, tempBoard);

  // remove temp wall added
  WallMatrixPosition[m_wall.position.x][m_wall.position.y] = NA;
  return isPartitioned;
}

uint8_t isReachable(struct Vector2D start, uint8_t row_target, uint8_t tempBoard[NUM_COLUMNS + NUM_COLUMNS_WALL][NUM_ROWS + NUM_ROWS_WALL]) {
  uint8_t i, j, k = 0;

  // reset tempBoard player flooding value to 0
  for (j = 0; j < NUM_ROWS; j += 2) {
    for (i = 0; i < NUM_COLUMNS; i += 2) {
      tempBoard[i][j] = 0;
    }
  }
  tempBoard[start.x * 2][start.y * 2] = 2;

  row_target *= 2;

  // worst case scenario need to flood the board n^2 times for the player possible moves
  // (actually don't have all those wall to make the traversal of the board so long,
  // so can be reduce but the board size is so small that it doesn't change much)
  while (++k <= (NUM_COLUMNS * NUM_ROWS)) {
    for (j = 0; j < NUM_ROWS + NUM_ROWS_WALL; j += 2) {
      for (i = 0; i < NUM_COLUMNS + NUM_COLUMNS_WALL; i += 2) {
        if (tempBoard[i][j] == 0)
          continue;

        // currect cell have value to flood
        if (tempBoard[i][j] == 2) {
          // if flooding algorithm reach target row exit with true value
          if (j == row_target) {
            return 1;
          }
          // not left border and don't have wall on left
          if (i > 1 && tempBoard[i - 1][j] != 1)
            tempBoard[i - 2][j] = 2;

          // not right border and don't have wall on right
          if (i < (NUM_COLUMNS + NUM_COLUMNS_WALL - 1) && tempBoard[i + 1][j] != 1)
            tempBoard[i + 2][j] = 2;

          // not up border and don't have wall on up
          if (j > 1 && (tempBoard[i][j - 1] != 1))
            tempBoard[i][j - 2] = 2;

          // not bottom border and don't have wall on bottom
          if (j < (NUM_ROWS + NUM_ROWS_WALL - 1) && (tempBoard[i][j + 1] != 1))
            tempBoard[i][j + 2] = 2;
        }
      }
    }
  }

  return 0;
}
