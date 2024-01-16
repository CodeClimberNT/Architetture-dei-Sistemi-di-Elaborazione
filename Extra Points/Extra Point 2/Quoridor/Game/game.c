/*--------------File Info---------------------------------------------------------------------------------
** File name:               game.c
** Descriptions:            Quoridor
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Nicolò Taormina
** Created date:            20/12/2023
** Version:                 v1.0
*********************************************************************************************************/

#include "game.h"

volatile struct Player player0;
volatile struct Player player1;
volatile struct UI timer_ui;
volatile struct UI player0_ui;
volatile struct UI player1_ui;

volatile struct Wall wall;

volatile uint8_t current_player = 0;
volatile uint8_t timeLeft = MAX_TIME;

char p0_wall_remaining[2];
char p1_wall_remaining[2];
char time_value[2];
volatile uint8_t cooldown;

WALL_DIRECTION WallMatrixPosition[NUM_COLUMNS_WALL][NUM_ROWS_WALL] = {NA};
// consider wall position in the junction between the player position (assume you cannot position a wall outside the board to have a 1 block lenght wall)
// e.g.  [] [] []
//  	     X  X
//			 [] [] []
// wall position will be in the X (where the [] rappresent the player position) and based on his propreties (Horizontal/Vertical) will block the respective Player Position

volatile GAME_STATE game_state = TRANSITION;
volatile MOVING_ENTITY moving_entity = PLAYER;

uint32_t lastMove;
volatile uint8_t started = 0;

void Setup() {
  Peripheral_Init();
  if (!started) {
    Title_Screen();
  } else {
    Start_Game();
    enable_timer(0);
  }
}

void Start_Game() {
  LCD_Clear(GameBG);
  Player_Init();
  Board_Init();
  UI_Init();
  Create_Hint_Move(player0);  // start player 0
  return;
}

void End_Turn(uint8_t timeout) {
  game_state = TRANSITION;

  lastMove = (current_player << 24);

  if (moving_entity == PLAYER) {
    // all 0 for moving the player and bc don't have direction in position
    lastMove |= (0 << 16);

    lastMove |= (timeout == 1) ? 1 : 0 << 12;
    lastMove |= (current_player == 0 ? player0.pos.y : player1.pos.y) << 8;
    lastMove |= current_player == 0 ? player0.pos.x : player1.pos.x;
  }
  if (moving_entity == WALL) {
    lastMove |= 1 << 16;
    lastMove |= (wall.direction == Vertical ? 0 : 1) << 12;
    lastMove |= wall.position.y << 8;
    lastMove |= wall.position.x;
  }

  if (Win_Condition()) {
    Game_Over();
    return;
  }

  // if turn timeout while moving wall
  if (moving_entity == WALL) {
    Remove_Wall(wall);
    moving_entity = PLAYER;
  }
  current_player = current_player == 0 ? 1 : 0;  // alternate between P0 and P1
  Create_Hint_Move(current_player == 0 ? player0 : player1);
  timeLeft = MAX_TIME;
  Update_Timer_UI(timeLeft);
}

void Timer_End_Turn() {
  if (moving_entity == PLAYER) {
    Remove_Hint_Move(current_player == 0 ? player0 : player1);
  } else {
    Remove_Wall(wall);
  }
  End_Turn(1);
}

uint8_t Win_Condition() {
  return (player0.pos.y == 0) || (player1.pos.y == NUM_ROWS - 1);
}

void Game_Over() {
  struct Player winner = player0.pos.y == 0 ? player0 : player1;
	uint8_t i,j;
	
	for (j=0; j<NUM_ROWS_WALL; j++){
		for (i=0; i<NUM_COLUMNS_WALL; i++){
			WallMatrixPosition[i][j] = NA;
		}
	}
	
  Show_Big_UI(40, -5, (winner.id == 0) ? (uint8_t *)"PLAYER 1 WINS!" : (uint8_t *)"PLAYER 2 WINS!", SUD_OVEST, 1, 5000000, Blue2, 1);
	
	current_player = 0;
	
  started = 0;

  Setup();
}



void Switch_Player_Wall() {
  game_state = TRANSITION;
  if (moving_entity == PLAYER) {
    // check if the player have no more wall left
    if ((current_player == 0 ? player0.wallsRemaining : player1.wallsRemaining) == 0) {
      Show_Big_UI(20, -5, (uint8_t *)"NO MORE WALL LEFT!", SUD_OVEST, 1, 5000000, Blue2, 1);
      Draw_Board();
      return;
    }

    moving_entity = WALL;
    Remove_Hint_Move(current_player == 0 ? player0 : player1);
    wall = Create_Wall(wall);
  } else {
    Remove_Wall(wall);
    Create_Hint_Move(current_player == 0 ? player0 : player1);
    moving_entity = PLAYER;
  }
}

void Move(DIRECTION dir) {
  game_state = TRANSITION;
  cooldown = COOLDOWN;

  switch (moving_entity) {
    case PLAYER:
      Create_Highlight_Move(current_player == 0 ? player0 : player1, dir);
      break;

    case WALL:
      wall = Move_Wall(wall, dir);
      break;
    default:
      GUI_Text(0, 0, (uint8_t *)"ERROR: \"moving_entity\" WRONG VALUE", Black, White);
  }
}

void Board_Init() {
  Draw_Checkers();

  Draw_Player(player0);
  Draw_Player(player1);
}

void UI_Init() {
  sprintf(p0_wall_remaining, "%u", MAX_WALLS);
  player0_ui = Create_UI(0, P0_UI_XPOS, P0_UI_YPOS, UI_HEIGHT, P_UI_WIDTH, P0_UI_XPOS + 5, P0_UI_YPOS + 5, "P1 WALL", P0_UI_XPOS + P_UI_WIDTH / 2 - 5, P0_UI_YPOS + UI_HEIGHT - 20, p0_wall_remaining);

  sprintf(p1_wall_remaining, "%u", MAX_WALLS);
  player1_ui = Create_UI(1, P1_UI_XPOS, P1_UI_YPOS, UI_HEIGHT, P_UI_WIDTH, P1_UI_XPOS + 5, P1_UI_YPOS + 5, "P2 WALL", P1_UI_XPOS + P_UI_WIDTH / 2 - 5, P1_UI_YPOS + UI_HEIGHT - 20, p1_wall_remaining);

  sprintf(time_value, "%u", timeLeft);
  timer_ui = Create_UI(2, T_UI_XPOS, T_UI_YPOS, UI_HEIGHT, T_UI_WIDTH, T_UI_XPOS + 5, T_UI_YPOS + 5, "TIME LEFT", T_UI_XPOS + T_UI_WIDTH / 2 - 5, T_UI_YPOS + UI_HEIGHT - 20, time_value);

  LCD_DrawRectWithShadow(player0_ui.box_pos.x, player0_ui.box_pos.y, player0_ui.width, player0_ui.height, Black, SUD_OVEST, Black);  // P0 UI
  GUI_Text(player0_ui.title_pos.x, player0_ui.title_pos.y, (uint8_t *)player0_ui.title_text, Black, White);

  LCD_DrawRectWithShadow(player1_ui.box_pos.x, player1_ui.box_pos.y, player1_ui.width, player1_ui.height, Black, SUD_OVEST, Black);  // P1 UI
  GUI_Text(player1_ui.title_pos.x, player1_ui.title_pos.y, (uint8_t *)player1_ui.title_text, Black, White);

  LCD_DrawRectWithShadow(timer_ui.box_pos.x, timer_ui.box_pos.y, timer_ui.width, timer_ui.height, Black, SUD_OVEST, Black);  // TIMER UI
  GUI_Text(timer_ui.title_pos.x, timer_ui.title_pos.y, (uint8_t *)timer_ui.title_text, Black, White);

  UI_Counter_Init();
}

void Player_Init() {
  player0 = Create_Player(0, 3, 6, P0_Color, MAX_WALLS);
  player1 = Create_Player(1, 3, 0, P1_Color, MAX_WALLS);
}

void UI_Counter_Init() {
  Update_UI(player0_ui);
  Update_UI(player1_ui);
  Update_UI(timer_ui);
}
