#include "draw.h"

void LCD_DrawRect(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, uint16_t color) {
  LCD_DrawLine(m_x0, m_y0, m_x1, m_y0, color);  // top edge
  LCD_DrawLine(m_x1, m_y0, m_x1, m_y1, color);  // right edge
  LCD_DrawLine(m_x1, m_y1, m_x0, m_y1, color);  // down edge
  LCD_DrawLine(m_x0, m_y1, m_x0, m_y0, color);  // left edge
}

void LCD_DrawShadow(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, SHADOW_DIRECTION direction, uint16_t color) {
  switch (direction) {
    case NORTH:
      LCD_DrawLine(m_x0 + 1, m_y0 - 1, m_x1 - 1, m_y0 - 1, color);
      break;
    case NORTH_EAST:
      LCD_DrawLine(m_x0 + 1, m_y0 - 1, m_x1 + 1, m_y0 - 1, color);
      LCD_DrawLine(m_x1 + 1, m_y0 - 1, m_x1 + 1, m_y1 - 1, color);
      break;
    case EAST:
      LCD_DrawLine(m_x1 + 1, m_y0 + 1, m_x1 + 1, m_y1 - 1, color);
      break;
    case SUD_EAST:
      LCD_DrawLine(m_x1 + 1, m_y0 + 1, m_x1 + 1, m_y1 + 1, color);
      LCD_DrawLine(m_x0 + 1, m_y1 + 1, m_x1 + 1, m_y1 + 1, color);
      break;
    case SUD:
      LCD_DrawLine(m_x0 + 1, m_y1 + 1, m_x1 - 1, m_y1 + 1, color);
      break;
    case SUD_OVEST:
      LCD_DrawLine(m_x0 - 1, m_y0 + 1, m_x0 - 1, m_y1 + 1, color);
      LCD_DrawLine(m_x0 - 1, m_y1 + 1, m_x1 - 1, m_y1 + 1, color);
      break;
    case OVEST:
      LCD_DrawLine(m_x0 - 1, m_y0 + 1, m_x0 - 1, m_y1 - 1, color);
      break;
    case NORTH_OVEST:
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x1 - 1, m_y0 - 1, color);
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x0 - 1, m_y1 - 1, color);
      break;
    case OVEST_NORTH_EAST:
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x0 - 1, m_y1 - 1, color);
      LCD_DrawLine(m_x0 - 1, m_y0 - 1, m_x1 + 1, m_y0 - 1, color);
      LCD_DrawLine(m_x1 + 1, m_y0 - 1, m_x1 + 1, m_y1 - 1, color);
      break;
  }
}

void LCD_DrawRectWithShadow(uint16_t m_x0, uint16_t m_y0, uint16_t width, uint16_t heigth, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color) {
  LCD_DrawRect(m_x0, m_y0, m_x0 + width, m_y0 + heigth, rect_color);
  LCD_DrawShadow(m_x0, m_y0, m_x0 + width, m_y0 + heigth, dir, shadow_color);
}

void LCD_DrawSquare(uint16_t m_x0, uint16_t m_y0, uint16_t len, uint16_t color) {
  LCD_DrawRect(m_x0, m_y0, m_x0 + len, m_y0 + len, color);  // square is just a special Rect
}

void LCD_FillRect(uint16_t m_x0, uint16_t m_y0, uint16_t m_x1, uint16_t m_y1, uint16_t color) {
  uint16_t i;

  for (i = m_y0; i <= m_y1; i++) {
    LCD_DrawLine(m_x0, i, m_x1, i, color);
  }
}

void LCD_FillSquare(uint16_t m_x0, uint16_t m_y0, uint16_t len, uint16_t color) {
  LCD_FillRect(m_x0, m_y0, m_x0 + len, m_y0 + len, color);
}

void LCD_ClearRect(struct Rect m_rect) {
  LCD_FillRect(m_rect.x0, m_rect.y0, m_rect.x1, m_rect.y1, GameBG);
}

void LCD_ClearSquare(struct Vector2D m_start_pos, uint16_t m_len) {
  LCD_FillSquare(m_start_pos.x, m_start_pos.y, m_len, GameBG);
}

struct Vector2D Get_Vec_From_Dir(DIRECTION dir) {
  struct Vector2D m_vec2d;

  switch (dir) {
    case UP:
      m_vec2d.x = 0;
      m_vec2d.y = -1;
      break;
    case RIGHT:
      m_vec2d.x = +1;
      m_vec2d.y = 0;
      break;
    case DOWN:
      m_vec2d.x = 0;
      m_vec2d.y = +1;
      break;
    case LEFT:
      m_vec2d.x = -1;
      m_vec2d.y = 0;
      break;
  }
  return m_vec2d;
}

DIRECTION Get_Dir_From_Vec(struct Vector2D vec2d) {
  // cannot move diagonally
  if (vec2d.x > 0)
    return RIGHT;
  if (vec2d.x < 0)
    return LEFT;
  if (vec2d.y > 0)
    return UP;

  // if(vec2d.y < 0)
  return DOWN;
}
