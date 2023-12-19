void BUTTON_init(void);

void disable_button( unsigned char button_num );
void enable_button( unsigned char button_num );

void EINT1_IRQHandler(void);
void EINT2_IRQHandler(void);
void EINT3_IRQHandler(void);
