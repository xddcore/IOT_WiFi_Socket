/**
************************************************************
* @file         hal_watchdog.h
* @brief        hal_watchdog.c对应头文件
* @author       Gizwits
* @date         2016-09-05
* @version      V03010101
* @copyright    Gizwits
* 
* @note         机智云.只为智能硬件而生
*               Gizwits Smart Cloud  for Smart Products
*               链接|增值ֵ|开放|中立|安全|自有|自由|生态
*               www.gizwits.com
*
***********************************************************/
#ifndef WATCHDOG_H
#define WATCHDOG_H

#include "stm32f10x.h"
/**
* @name 看门狗宏开关
* @{
*/
#define WATCHDOG
/**@} */

void watchdogInit(uint8_t timeoutS);
void watchdogFeed(void);

#endif

