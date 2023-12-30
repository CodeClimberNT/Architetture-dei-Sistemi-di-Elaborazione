#include "peripheral.h"

void Peripheral_Init() {
  LCD_Initialization();
  // 0x17D7840
  init_timer(0, 0x26259F); /* 1s    * 25MHz = 25*10^6   = 0x17D7840 */
  // init_timer(0, 0x1312D0 ); 						/* 50ms  * 25MHz = 1.25*10^6 = 0x1312D0  */
  // init_timer(0, 0x6108 ); 						  /* 1ms   * 25MHz = 25*10^3   = 0x6108    */
  // init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2     */

  init_RIT(0x7A120); /* 50ms  * 100MHz = 5*10^6 = 0x4C4B40  */
  BUTTON_init();
  disable_button(1);
  disable_button(2);
  joystick_init();
}

void Peripheral_Enable() {
  enable_timer(0);
  enable_RIT();
  enable_button(1);
  enable_button(2);
}
