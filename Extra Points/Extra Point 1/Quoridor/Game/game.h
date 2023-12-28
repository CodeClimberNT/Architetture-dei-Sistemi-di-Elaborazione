#ifndef __GAME_H  // Include guard
#define __GAME_H

#ifndef __STDINT_H
#include "stdint.h"
#endif

#ifndef __MY_STDIO_H
#define __MY_STDIO_H
#include "stdio.h"
#endif

/*******************PERIPHERAL LINKAGE**********************/
#ifndef __MY_PERIPHERAL_H
#define __MY_PERIPHERAL_H
#include "../GLCD/GLCD.h"
#include "../RIT/RIT.h"
#include "../button_EXINT/button.h"
#include "../joystick/joystick.h"
#include "../timer/timer.h"
#endif

/*******************CONSTANT DEFINITION**********************/
// COLOR DEFINITION
#define GameBG White
#define DarkGray 0x7BEF
#define ChristmasRed 0xF800
#define ChristmasGreen 0xE60
#define QuoridorRed 0xA9A9

#define P0_Color Blue2
#define P1_Color Red

#define WallColor 0xF731
#define PhantomWallColor DarkGray
#define WALL_DISCOUNT 1

// PLAYER OPTIONS
#define MAX_WALLS 8
#define MAX_TIME 20  // seconds
#define COOLDOWN 2   // wait #COOLDOWN before allowed to next move

// BOARD OPTIONS
#define NUM_ROWS 7
#define NUM_COLUMNS 7
#define NUM_ROWS_WALL NUM_ROWS - 1
#define NUM_COLUMNS_WALL NUM_COLUMNS - 1
#define WALL_WIDTH 6
#define HALF_WALL_WIDTH WALL_WIDTH / 2
#define SQUARE_SIZE (MAX_X - (NUM_ROWS - 1) * WALL_WIDTH) / NUM_ROWS
#define PLAYER_SIZE SQUARE_SIZE - 2
#define WALL_LENGTH (SQUARE_SIZE * 2 + WALL_WIDTH)  // wall takes two square plus the wall width in the interception

// UI OPTIONS

// READY
#define READY_UI_WIDTH MAX_X * 0.8
#define READY_UI_HEIGHT 80

#define READY_UI_XPOS (MAX_X - READY_UI_WIDTH) / 2   // CENTER HORIZONTALLY
#define READY_UI_YPOS (MAX_Y - READY_UI_HEIGHT) / 2  // CENTER VERTICALLY

// HEIGHT DURING GAME
#define UI_HEIGHT 2 * SQUARE_SIZE

#define UI_YPOS (SQUARE_SIZE + WALL_WIDTH) * NUM_ROWS + SQUARE_SIZE / 3

// PLAYER UI
#define P_UI_WIDTH 64

#define P0_UI_XPOS 10
#define P0_UI_YPOS UI_YPOS
#define P1_UI_XPOS MAX_X - 10 - P_UI_WIDTH
#define P1_UI_YPOS UI_YPOS

// Pos for timer ui
#define T_UI_XPOS (P0_UI_XPOS + P_UI_WIDTH + 5)
#define T_UI_YPOS UI_YPOS

#define T_UI_HEIGHT 70
#define T_UI_WIDTH 82


//Color Buffer
#define COLOR_BUFFER_LENGTH	MAX_X
#define COLOR_BUFFER_HEIGHT (SQUARE_SIZE+WALL_WIDTH)*NUM_ROWS

/*******************ENUM DEFINITION**********************/
typedef enum { TRANSITION,
               IDLE } GAME_STATE;
typedef enum { PLAYER,
               WALL } MOVING_ENTITY;
typedef enum { Vertical,
               Horizontal } WALL_DIRECTION;
typedef enum { UP,
               RIGHT,
               DOWN,
               LEFT } DIRECTION;
typedef enum { NORTH,
               NORTH_EAST,
               EAST,
               SUD_EAST,
               SUD,
               SUD_OVEST,
               OVEST,
               NORTH_OVEST,
               OVEST_NORTH_EAST } SHADOW_DIRECTION;

/*******************STRUCT DEFINITION**********************/
struct Vector2D {
  int16_t x;
  int16_t y;
};

struct Rect {
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
};

struct Player {
  uint8_t id;
  struct Vector2D Position;
  uint8_t wallsRemaining;
  uint16_t color;
};

struct Wall {
  struct Vector2D position;
  WALL_DIRECTION direction;
  uint16_t color;
	uint8_t discount;
};

struct UI {
  uint8_t id;
  struct Vector2D ui_Position;
  struct Vector2D title_position;
  char *title_text;
  struct Vector2D value_position;
  char *value_text;
  uint16_t height;
  uint16_t width;
};

/*******************UTILS FUNCTIONS**********************/
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MOD(x, y) ((x % y + y) % y)

/*******************GAME FUNCTION**********************/
void Setup(void);

// INIT FUNCTION
void Peripheral_Init(void);
void Peripheral_Enable(void);
void Start_Game(void);
void Board_Init(void);
void UI_Init(void);

void UI_Counter_Init(void);

// GAME MODE FUNCTION
void Waiting_Player(void);
void End_Turn(void);

void Switch_Player_Wall(void);

// PLAYER FUNCTION
void Player_Init(void);
struct Player Create_Player(uint8_t id, uint16_t x, uint16_t y, uint16_t color);

void UI_Init(void);
struct UI Create_UI(uint8_t id, uint16_t ui_x, uint16_t ui_y, uint16_t height, uint16_t width, uint16_t tit_x, uint16_t tit_y, char title_text[2], uint16_t val_x, uint16_t val_y, char value_text[2]);

void Update_UI(struct UI ui);

void Update_Timer(uint8_t time_value);

void Decrease_Timer(void);

void LCD_DrawRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_DrawSquare(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color);
void LCD_FillRect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void LCD_FillSquare(uint16_t x0, uint16_t y0, uint16_t len, uint16_t color);
void LCD_DrawRectWithShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t rect_color, SHADOW_DIRECTION dir, uint16_t shadow_color);
void LCD_DrawShadow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SHADOW_DIRECTION dir, uint16_t color);

void Draw_Checkers(void);

void Move(DIRECTION dir);

void Position_Player(struct Player player);
struct Player Move_Player(struct Player player, DIRECTION dir);
void Remove_Player(struct Player player);

struct Wall Create_Wall(struct Wall wall);
void Place_Wall(struct Wall wall);
struct Wall Preview_Wall(struct Wall wall);
struct Wall Move_Wall(struct Wall wall, DIRECTION dir);
struct Wall Rotate_Wall(struct Wall m_wall);
void Remove_Wall(struct Wall wall);
uint8_t Can_Place_Wall(struct Wall m_wall);

struct Rect Get_Position_Of(struct Wall m_wall);

struct Vector2D Get_Relative_Pos(DIRECTION dir);
struct Vector2D Find_Free_Spot(struct Vector2D vec2d);

void wait_delay(int count);

void Create_Color_Buffer(void);
void Draw_Color_Buffer(void);


#endif  // __GAME_H
