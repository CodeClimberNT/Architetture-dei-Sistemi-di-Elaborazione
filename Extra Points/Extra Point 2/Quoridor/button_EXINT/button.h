#ifndef __STDINT_H
#include "stdint.h"
#endif

void BUTTON_init(void);

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);

void disable_button( uint8_t button_num);
void enable_button( uint8_t button_num );
