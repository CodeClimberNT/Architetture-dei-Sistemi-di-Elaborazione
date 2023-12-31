#include "LPC17xx.h"
#include "game.h"

extern GAME_STATE game_state;

extern struct Player player0;
extern struct Player player1;
extern struct UI timer_ui;
extern struct UI player0_ui;
extern struct UI player1_ui;

extern uint8_t timeLeft;
extern char p0_wall_remaining[2];
extern char p1_wall_remaining[2];
extern char time_value[2];

void Start_Game() {
  LCD_Clear(GameBG);
  Player_Init();
  Board_Init();
  UI_Init();
	Create_Hint_Move(player0); //start player 0
  return;
}

void Board_Init() {
  Draw_Checkers();

  Position_Player(player0);
  Position_Player(player1);
}

void UI_Init() {
  sprintf(p0_wall_remaining, "%u", MAX_WALLS);
  player0_ui = Create_UI(0, P0_UI_XPOS, P0_UI_YPOS, UI_HEIGHT, P_UI_WIDTH, P0_UI_XPOS + 5, P0_UI_YPOS + 5, "P1 WALL", P0_UI_XPOS + P_UI_WIDTH / 2 - 5, P0_UI_YPOS + UI_HEIGHT - 20, p0_wall_remaining);

  sprintf(p1_wall_remaining, "%u", MAX_WALLS);
  player1_ui = Create_UI(1, P1_UI_XPOS, P1_UI_YPOS, UI_HEIGHT, P_UI_WIDTH, P1_UI_XPOS + 5, P1_UI_YPOS + 5, "P2 WALL", P1_UI_XPOS + P_UI_WIDTH / 2 - 5, P1_UI_YPOS + UI_HEIGHT - 20, p1_wall_remaining);

  sprintf(time_value, "%u", timeLeft);
  timer_ui = Create_UI(2, T_UI_XPOS, T_UI_YPOS, UI_HEIGHT, T_UI_WIDTH, T_UI_XPOS + 5, T_UI_YPOS + 5, "TIME LEFT", T_UI_XPOS + T_UI_WIDTH / 2 - 5, T_UI_YPOS + UI_HEIGHT - 20, time_value);

  LCD_DrawRectWithShadow(player0_ui.ui_Position.x, player0_ui.ui_Position.y, player0_ui.width, player0_ui.height, Black, SUD_OVEST, Black);  // P0 UI
  GUI_Text(player0_ui.title_position.x, player0_ui.title_position.y, (uint8_t *)player0_ui.title_text, Black, White);

  LCD_DrawRectWithShadow(player1_ui.ui_Position.x, player1_ui.ui_Position.y, player1_ui.width, player1_ui.height, Black, SUD_OVEST, Black);  // P1 UI
  GUI_Text(player1_ui.title_position.x, player1_ui.title_position.y, (uint8_t *)player1_ui.title_text, Black, White);

  LCD_DrawRectWithShadow(timer_ui.ui_Position.x, timer_ui.ui_Position.y, timer_ui.width, timer_ui.height, Black, SUD_OVEST, Black);  // TIMER UI
  GUI_Text(timer_ui.title_position.x, timer_ui.title_position.y, (uint8_t *)timer_ui.title_text, Black, White);

  UI_Counter_Init();
}

void Player_Init() {
  player0 = Create_Player(0, 3, 6, P0_Color);
  player1 = Create_Player(1, 3, 0, P1_Color);
}

struct Player Create_Player(uint8_t id, uint16_t x, uint16_t y, uint16_t color) {
  struct Player player;

  player.id = id;
  player.pos.x = x;
  player.pos.y = y;
  player.wallsRemaining = MAX_WALLS;
  player.color = color;
	player.ghost = 0;

  return player;
};

void UI_Counter_Init() {
  Update_UI(player0_ui);
  Update_UI(player1_ui);
  Update_UI(timer_ui);
}
