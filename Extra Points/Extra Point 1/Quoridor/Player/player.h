#ifndef __PLAYER_H
#define __PLAYER_H

#include "../Game/includes.h"

struct Player {
  uint8_t id;
  struct Vector2D board_pos;
	struct Vector2D screen_pos;
	uint8_t size;
  uint8_t wallsRemaining;
  uint16_t color;
};

void Player_Init(void);
struct Player Create_Player(uint8_t m_id, uint16_t m_x, uint16_t m_y, uint16_t m_color);

void Position_Player(struct Player player);
struct Player Move_Player(struct Player player, DIRECTION dir);
void Remove_Player(struct Player player);

#endif
