#include "ui.h"

volatile struct UI timer_ui;
volatile struct UI player0_ui;
volatile struct UI player1_ui;


void Waiting_UI() {
    GUI_Text(40, 20, (uint8_t *)"\"CHRISTMAS\" QUORIDOR", QuoridorRed, White);
    LCD_DrawRectWithShadow(READY_UI_XPOS, READY_UI_YPOS, READY_UI_XPOS + READY_UI_WIDTH, READY_UI_YPOS + READY_UI_HEIGHT, Blue2, NORTH_OVEST, DarkGray);
    GUI_Text(READY_UI_XPOS + 20, READY_UI_YPOS + (READY_UI_HEIGHT / 2) - 5, (uint8_t *)"PRESS INT0 TO START", Black, White);

    GUI_Text(70, MAX_Y - 20, (uint8_t *)"FELIZ", ChristmasRed, White);
    GUI_Text(115, MAX_Y - 20, (uint8_t *)"NAVIDAD", ChristmasGreen, White);
}

struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y, char *title_text,
                    uint16_t val_x, uint16_t val_y, char *value_text) {
  struct UI m_ui;

  m_ui.id = id;

  m_ui.ui_Position.x = ui_x;
  m_ui.ui_Position.y = ui_y;
  m_ui.height = height;
  m_ui.width = width;
  
  m_ui.title_position.x = tit_x;
  m_ui.title_position.y = tit_y;
  m_ui.title_text = title_text;
  m_ui.value_position.x = val_x;
  m_ui.value_position.y = val_y;
  m_ui.value_text = value_text;

  return m_ui;
}

void Update_UI(struct UI m_ui) {
  // erase previous text
  LCD_FillRect(m_ui.value_position.x, m_ui.value_position.y, m_ui.value_position.x + 20, m_ui.value_position.y + 15, GameBG);
  // update new text
  GUI_Text(m_ui.value_position.x, m_ui.value_position.y, (uint8_t *)m_ui.value_text, Black, White);
}

void Update_Timer(uint8_t value) {
  sprintf(time_value, "%u", value);
  Update_UI(timer_ui);
}