#include "color.h"

/**************Have some problem with setting point color being additive instead of substitutive*************************/
/*
void Create_Color_Buffer(uint16_t *p_buffer_vec, struct Vector2D m_start_pos, uint16_t m_buffer_lenght, uint16_t m_buffer_height){
	uint16_t i, j, x, y;
	for(j=0; j<=m_buffer_height; j++){
		for(i=0; i<=m_buffer_lenght; i++){
			x = i + m_start_pos.x;
			y = j + m_start_pos.y;
			p_buffer_vec[x * m_buffer_lenght + y] = LCD_GetPoint(x, y);
		}
	}
}

void Draw_Color_Buffer(uint16_t *p_buffer_vec, struct Vector2D m_start_pos, uint16_t m_buffer_lenght, uint16_t m_buffer_height){
	uint16_t i,j, x, y;
	for(j=0; j<=m_buffer_height; j++){
		for(i=0; i<=m_buffer_lenght; i++){
			x = i + m_start_pos.x;
			y = j + m_start_pos.y;
			LCD_SetPoint(x, y, p_buffer_vec[x * m_buffer_lenght + y]);
		}
	}
}

void Create_Wall_Color_Buffer(struct Wall m_wall){
	struct Vector2D m_start_pos;
	m_start_pos.x = m_wall.position.x * (SQUARE_SIZE + WALL_WIDTH);
	m_start_pos.y = m_wall.position.y * (SQUARE_SIZE + WALL_WIDTH);
	
	Create_Color_Buffer(&wall_color_buffer[0][0], m_start_pos, WALL_COLOR_BUFFER_LENGTH, WALL_COLOR_BUFFER_HEIGHT);
}

void Draw_Wall_Color_Buffer(struct Wall m_wall){
	struct Vector2D m_start_pos;
	m_start_pos.x = m_wall.position.x * (SQUARE_SIZE + WALL_WIDTH);
	m_start_pos.y = m_wall.position.y * (SQUARE_SIZE + WALL_WIDTH);
	
	Draw_Color_Buffer(&wall_color_buffer[0][0], m_start_pos, WALL_COLOR_BUFFER_LENGTH, WALL_COLOR_BUFFER_HEIGHT);
}
*/
