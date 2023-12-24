#ifndef __PLAYER_H  // Include guard
#define __PLAYER_H

#ifndef __STDINT_H
#include <stdint.h>
#endif

#define MAX_WALLS	8
#define MAX_TIME	20 //seconds


struct Point {
    uint16_t x;
    uint16_t y;
};

struct Player {
    uint8_t Id;
    struct Point Position;
    uint8_t wallsRemaining; 
};

void Player_Init(void);
struct Player Create_Player(uint8_t Id);
void Update_Wall_Counter(void);
void Update_Timer_Left(void);

#endif  // __PLAYER_H
