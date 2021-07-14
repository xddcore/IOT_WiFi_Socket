#ifndef _HAL_RELAY_H
#define _HAL_RELAY_H
#include <stm32f10x.h>
#define relay_on  1
#define relay_off 0
void relay_init(void);
void relay_control(unsigned char onoff);
#endif
