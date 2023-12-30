#ifndef __COLOR_H
#define __COLOR_H

//LCD COLOR
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define GameBG White
#define DarkGray 0x7BEF
#define ChristmasRed 0xF800
#define ChristmasGreen 0xE60
#define QuoridorRed 0xA9A9

#define P0_Color Blue2
#define P1_Color Red

#define WallColor 0xF731
#define PhantomWallColor DarkGray

/*
void Create_Color_Buffer(uint16_t *p_buffer_vec, struct Vector2D m_start_pos ,uint16_t m_buffer_lenght, uint16_t m_buffer_height);
void Draw_Color_Buffer(uint16_t *p_buffer_vec, struct Vector2D m_start_pos ,uint16_t m_buffer_lenght, uint16_t m_buffer_height);

void Create_Wall_Color_Buffer(struct Wall m_wall);
void Draw_Wall_Color_Buffer(struct Wall m_wall);
*/

#endif
