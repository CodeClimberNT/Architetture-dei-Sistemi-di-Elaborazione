#include "board.h"

extern struct Player player0;
extern struct Player player1;

void Board_Init() {
  Draw_Checkers();

  Position_Player(player0);
  Position_Player(player1);
}

void Draw_Checkers() {
  uint8_t i, j;
	
	for (j = 0; j < NUM_ROWS; j++) {
		for (i = 0; i < NUM_COLUMNS; i++) {
      uint16_t x = i * (SQUARE_SIZE + WALL_WIDTH);
      uint16_t y = j * (SQUARE_SIZE + WALL_WIDTH);
      LCD_DrawSquare(x, y, SQUARE_SIZE, Black);
			
			if(player0.board_pos.x == i && player0.board_pos.y == j)
				Position_Player(player0);
			
			if(player1.board_pos.x == i && player1.board_pos.y == j)
				Position_Player(player1);
    }
  }
}
