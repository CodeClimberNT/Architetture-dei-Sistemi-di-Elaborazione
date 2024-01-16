#include "ui.h"

extern struct UI timer_ui;
extern char time_value[2];
extern uint8_t timeLeft;

extern struct UI player0_ui;
extern char p0_wall_remaining[2];

extern struct UI player1_ui;
extern char p1_wall_remaining[2];

extern uint8_t current_player;

struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y, char *title_text,
                    uint16_t val_x, uint16_t val_y, char *value_text) {
  struct UI m_ui;

  m_ui.id = id;

  m_ui.box_pos.x = ui_x;
  m_ui.box_pos.y = ui_y;
  m_ui.height = height;
  m_ui.width = width;

  m_ui.title_pos.x = tit_x;
  m_ui.title_pos.y = tit_y;
  m_ui.title_text = title_text;
  m_ui.value_pos.x = val_x;
  m_ui.value_pos.y = val_y;
  m_ui.value_text = value_text;

  return m_ui;
}

void Update_UI(struct UI m_ui) {
  // erase previous text
  LCD_FillRect(m_ui.value_pos.x, m_ui.value_pos.y, m_ui.value_pos.x + 20, m_ui.value_pos.y + 15, GameBG);
  // update new text
  GUI_Text(m_ui.value_pos.x, m_ui.value_pos.y, (uint8_t *)m_ui.value_text, Black, White);
}

void Update_Timer_UI(uint8_t value) {
  sprintf(time_value, "%u", value);
  Update_UI(timer_ui);
}

void Update_Wall_UI(struct UI m_player_ui, uint8_t wall_value) {
  switch (m_player_ui.id) {
    case 0:
      sprintf(p0_wall_remaining, "%u", wall_value);
      Update_UI(player0_ui);
      break;

    case 1:
      sprintf(p1_wall_remaining, "%u", wall_value);
      Update_UI(player1_ui);
      break;

    default:
      GUI_Text(0, 0, (uint8_t *)"ERROR: Update_Wall_UI WRONG PLAYER ID", Black, White);
  }
}

void Show_Big_UI(int16_t txt_xoff, int16_t txt_yoff, uint8_t *p_text, SHADOW_DIRECTION shadow_dir, uint8_t temp_show, uint32_t delay, uint16_t border_color, uint8_t need_to_fill) {
  if(need_to_fill)
		LCD_FillRect(BIG_UI_XPOS, BIG_UI_YPOS, BIG_UI_XPOS + BIG_UI_WIDTH, BIG_UI_YPOS + BIG_UI_HEIGHT, GameBG);
  LCD_DrawRectWithShadow(BIG_UI_XPOS, BIG_UI_YPOS, BIG_UI_WIDTH, BIG_UI_HEIGHT, Blue2, shadow_dir, DarkGray);

  GUI_Text(BIG_UI_XPOS + txt_xoff, BIG_UI_YPOS + (BIG_UI_HEIGHT / 2) + txt_yoff, p_text, Black, GameBG);
  if (temp_show) {
		#ifndef SIMULATOR
			delay *= 10;
		#endif
    wait_delay(delay);
    // rect and shadows
    LCD_FillRect(BIG_UI_XPOS - 2, BIG_UI_YPOS - 2, BIG_UI_XPOS + BIG_UI_WIDTH + 2, BIG_UI_YPOS + BIG_UI_HEIGHT + 2, GameBG);
  }
}



void Title_Screen() {
  LCD_Clear(White);
	
	GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
  
	Show_Big_UI(20, -5, (uint8_t *)"PRESS INT0 TO START", SUD_OVEST, 0, 0, Blue2, 0);
	
	GUI_Text(70, MAX_Y - 20, (uint8_t *)"FELIZ", ChristmasRed, White);
  GUI_Text(115, MAX_Y - 20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}

void Game_Mode_UI(void){
	LCD_Clear(White);
	
	GUI_Text((MAX_X / 2) - 38, 20, (uint8_t *)"Select the", Black, White);
	GUI_Text((MAX_X / 2) - 35, 40, (uint8_t *)"GAME MODE", Black, White);
}

void Opponent_Selection_UI(void){

}
