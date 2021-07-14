/**
************************************************************
* @file         main.c
* @brief        MCU版 入口文件
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

#include <string.h>
#include "Hal_key/hal_key.h"
#include "Hal_Usart/hal_uart.h"
#include "Hal_Watchdog/hal_watchdog.h"
#include "gizwits_protocol.h"
#include "hal_relay.h"
#include "hal_led.h"
#include "adc.h"
#include "acs712.h"
#include "delay.h"
#include "24cxx.h" 
u8 w=0;//倒计时开启标志
	u8 count_h;
  u8 count_min;
/** 用户区当前设备状态结构体*/
	u8 sys_week;//系统week
	unsigned char start_flag=0;
dataPoint_t currentDataPoint;
/**@name 按键相关定义 
* @{
*/
#define GPIO_KEY_NUM 2                                  ///< 定义按键成员总数
keyTypedef_t singleKey[GPIO_KEY_NUM];                   ///< 定义单个按键成员数组指针
keysTypedef_t keys;                                     ///< 定义总的按键模块结构体指针
/**@} */

/**
* 用户初始化函数

* 在该函数中完成了外设驱动初始化以及用户相关数据的初始
* @param none
* @return none
* @note 开发者可在此函数内添加新的驱动初始及状态初始化
*/
void userInit(void)
{
    uartxInit();        //printf打印串口初始化
  //  watchdogInit(4);    //看门狗复位时间2s
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
}

/**
* 用户数据获取

* 此处需要用户实现除可写数据点之外所有传感器数据的采集,可自行定义采集频率和设计数据过滤算法
* @param none
* @return none
*/

void userHandle(void)
{
u8 i;
	typedef struct 
{
	u8 MON;
	u8 TUE;
	u8 WED;			
	u8 THU;
	u8  FIR;
	u8  SAT;
	char  SUN;		 
}_week_Repeat;	

_week_Repeat week_Repeat;
	u8 h_On;
	u8 min_On;
	
	u8 h_Off;
	u8 min_Off;
	
	u8 h_CountDown;
	u8 min_CountDown;
	
	u8 Repeat_week_app;
	
extern	u8 sys_h;
extern	u8 sys_min;
	/*if(start_flag==0)//首次启动读出数据
	{
	currentDataPoint.valueTime_OnOff=AT24CXX_ReadOneByte(address_valueTime_OnOff );
	//currentDataPoint.valueCountDown_OnOff=AT24CXX_ReadOneByte(address_valueCountDown_OnOff);
	
	//currentDataPoint.valueWeek_Repeat=AT24CXX_ReadLenByte(address_valueWeek_Repeat,4);
	//currentDataPoint.valueTime_On_Minute=AT24CXX_ReadLenByte(address_valueTime_On_Minute,4);
	//currentDataPoint.valueTime_Off_Minute=AT24CXX_ReadLenByte(address_valueTime_Off_Minute,4);
	//currentDataPoint.valueCountDown_Minute=AT24CXX_ReadLenByte(address_valueCountDown_Minute,4);
		start_flag=1;
	}*/
	if(currentDataPoint.valueTime_OnOff==1) //是否开启定时预约
	{
		currentDataPoint.valueCountDown_OnOff=0;
	delay_ms(200);
	gizwitsGetNTP();//NTP 授时
		Repeat_week_app=currentDataPoint.valueWeek_Repeat;//周重复结构体
		for(i=1;i<=7;i++)//周数据解算
		{
			if(i==1)week_Repeat.SUN=((Repeat_week_app<<1)&0x80)>>7;
			if(i==2)week_Repeat.SAT=((Repeat_week_app<<2)&0x80)>>7;
			if(i==3)week_Repeat.FIR=((Repeat_week_app<<3)&0x80)>>7;
			if(i==4)week_Repeat.THU=((Repeat_week_app<<4)&0x80)>>7;
			if(i==5)week_Repeat.WED=((Repeat_week_app<<5)&0x80)>>7;
			if(i==6)week_Repeat.TUE=((Repeat_week_app<<6)&0x80)>>7;
			if(i==7)week_Repeat.MON=((Repeat_week_app<<7)&0x80)>>7;
		}
		if(week_Repeat.TUE==1)week_Repeat.TUE=week_Repeat.TUE+1;
		if(week_Repeat.WED==1)week_Repeat.WED=week_Repeat.WED+2;
		if(week_Repeat.THU==1)week_Repeat.THU=week_Repeat.THU+3;
		if(week_Repeat.FIR==1)week_Repeat.FIR=week_Repeat.FIR+4;
		if(week_Repeat.SAT==1)week_Repeat.SAT=week_Repeat.SAT+5;
		if(week_Repeat.SUN==1)week_Repeat.SUN=week_Repeat.SUN+6;
		if(sys_week==week_Repeat.MON||sys_week==week_Repeat.TUE
			||sys_week==week_Repeat.WED||sys_week==week_Repeat.THU
		||sys_week==week_Repeat.FIR||sys_week==week_Repeat.SAT||sys_week==week_Repeat.SUN)//判断用户是否设置了周重复
		{
			
		h_On=currentDataPoint.valueTime_On_Minute/60;
		min_On=currentDataPoint.valueTime_On_Minute%60;//获取定时开时间
		
		h_Off=currentDataPoint.valueTime_Off_Minute/60;//获取定时关时间
		min_Off=currentDataPoint.valueTime_Off_Minute%60;
		
			switch(sys_week)//判断下今天是第几周
			{
				case 1:		
		if(sys_week==week_Repeat.MON)//判断周一是否被选定
					{
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
		//	currentDataPoint.valueTime_OnOff=0;
		}
	}
		break;
		
		
				case 2:	
		if(sys_week==week_Repeat.TUE)//判断周2是否被选定
					{					
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
	}
					break;
		
		
		
				case 3:		
							if(sys_week==week_Repeat.WED)//判断周3是否被选定
					{
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
			//currentDataPoint.valueTime_OnOff=0;
		}
		
		
	}
					break;
		
		
				case 4:		
							if(sys_week==week_Repeat.THU)//判断周4是否被选定
					{
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
	}
					break;
		
		
				case 5:	
		if(sys_week==week_Repeat.FIR)//判断周5是否被选定
					{					
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
	}
					break;
		
		
				case 6:		
							if(sys_week==week_Repeat.SAT)//判断周6是否被选定
					{
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
		//	currentDataPoint.valueTime_OnOff=0;
		}
	}
					break;
		
		
				case 0:	
		if(sys_week==(week_Repeat.SUN-7))//判断周7是否被选定
					{					
					if(sys_h==h_On)
		{
			if(sys_min==min_On)
			relay_control(relay_on);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
				if(sys_h==h_Off)
		{
			if(sys_min==min_Off)
			relay_control(relay_off);
		//	currentDataPoint.valueTime_OnOff=0;
		}
		
	}
					break;
		
				default:break;
					
			}
	}
	}
	
	
		if(currentDataPoint.valueCountDown_OnOff==1)//判断倒计时功能是否开启
	{
		currentDataPoint.valueTime_OnOff=0;
 delay_ms(200);
	gizwitsGetNTP();//NTP 授时
		h_CountDown=currentDataPoint.valueCountDown_Minute/60;
		min_CountDown=currentDataPoint.valueCountDown_Minute%60;
		if(w==0)
		{
			w=1;
		count_h=sys_h+h_CountDown;
		count_min=sys_min+min_CountDown;
		if(count_min>=60){count_min-=60;count_h+=1;}
	  if(count_h>=24){count_h-=24;}
		}
		//count_h=sys_h+h_CountDown;
		//count_min=sys_min+min_CountDown;
		if(sys_h==count_h)
		{
			if(sys_min==count_min)
			{
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1)//翻转状态
		{
			currentDataPoint.valueCountDown_OnOff=0;
		relay_control(relay_off);
	//currentDataPoint.valueOnOff=0;
		}
	else
	{
		currentDataPoint.valueCountDown_OnOff=0;
	relay_control(relay_on);
//	currentDataPoint.valueOnOff=1;
	}
		}
	
	}
}
	else
	{
		w=0;
	}
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1)//继电器控制手动控制状态上报
{
	currentDataPoint.valueOnOff=1;
 switch_led(led_onoff,on);//开启蓝灯
}
	else
	{
	currentDataPoint.valueOnOff=0;
		switch_led(led_onoff,off);//关闭蓝灯
	}
	currentDataPoint.valueelectric = get_Electric();
	//currentDataPoint.valuePower_Consumption =sys_week;
	    /*
    currentDataPoint.valuePower_Consumption = ;//Add Sensor Data Collection
    currentDataPoint.valueelectric = ;//Add Sensor Data Collection
    currentDataPoint.valueFuse_failure = ;//Add Sensor Data Collection

    */
}

/**
* key1按键短按处理
* @param none
* @return none
*/
void key1ShortPress(void)
{
    printf("KEY1 PRESS ,Production Mode\n");
    gizwitsSetMode(WIFI_PRODUCTION_TEST);
}

/**
* key1按键长按处理
* @param none
* @return none
*/
void key1LongPress(void)
{
    printf("KEY1 PRESS LONG ,Wifi Reset\n");
    gizwitsSetMode(WIFI_RESET_MODE);

}

/**
* key2按键短按处理
* @param none
* @return none
*/
void key2ShortPress(void)
{
   // printf("KEY2 PRESS ,Soft AP mode\n");
    //gizwitsSetMode(WIFI_SOFTAP_MODE);
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==1)
	{
	 relay_control(relay_off);
	switch_led(led_onoff,off);//关闭蓝灯
	}
	else
	{
		relay_control(relay_on);
		switch_led(led_onoff,on);//开启蓝灯
	}
}

/**
* key2按键长按处理
* @param none
* @return none
*/
void key2LongPress(void)
{
    //AirLink mode
    printf("KEY2 PRESS LONG ,AirLink mode\n");
    gizwitsSetMode(WIFI_AIRLINK_MODE);
	  switch_led(led_normal,on);//绿灯指示
}

/**
* 按键初始化
* @param none
* @return none
*/
void keyInit(void)
{
    singleKey[0] = keyInitOne(RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_10, key1ShortPress, key1LongPress);
    singleKey[1] = keyInitOne(RCC_APB2Periph_GPIOA, GPIOB, GPIO_Pin_0, key2ShortPress, key2LongPress);
    keys.singleKey = (keyTypedef_t *)&singleKey;
    keyParaInit(&keys); 
}

/**
* @brief程序入口函数

* 在该函数中完成用户相关的初始化,及主逻辑循环
* @param none
* @return none
*/
int main(void)
{
    SystemInit();
    
    userInit();
    keyInit();
   relay_init();
	 led_init();
	 Adc_Init();
	delay_init();
	AT24CXX_Init();//AT24C02初始化
    timerInit();
    uartInit();

    gizwitsInit();
 // currentDataPoint.valueTime_OnOff=AT24CXX_ReadOneByte(address_valueTime_OnOff );
	//currentDataPoint.valueCountDown_OnOff=AT24CXX_ReadOneByte(address_valueCountDown_OnOff);
	currentDataPoint.valueWeek_Repeat=AT24CXX_ReadLenByte(address_valueWeek_Repeat,4);
  currentDataPoint.valueTime_On_Minute=AT24CXX_ReadLenByte(address_valueTime_On_Minute,4);
	currentDataPoint.valueTime_Off_Minute=AT24CXX_ReadLenByte(address_valueTime_Off_Minute,4);
	currentDataPoint.valueCountDown_Minute=AT24CXX_ReadLenByte(address_valueCountDown_Minute,4);
    printf("MCU Init Success \n");
    while(1)
    {
       // watchdogFeed();
        
        userHandle();
        
        gizwitsHandle((dataPoint_t *)&currentDataPoint);
    }
}
