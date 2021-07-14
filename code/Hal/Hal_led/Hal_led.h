#ifndef _HAL_LED_H
#define _HAL_LED_H
#include <stm32f10x.h>
#define led_normal  1
#define led_error   2
#define led_onoff   3
#define on          1
#define off         0
void led_init(void);
void switch_led(unsigned char sn,unsigned char onoff);
#endif
