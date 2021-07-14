#include "hal_led.h"
void led_init()
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //状态正常指示
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOA,GPIO_Pin_0);						 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;	    		 //PB13状态异常 PB15开关指示
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
GPIO_Init(GPIOB, &GPIO_InitStructure);	  		
 GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_15); 						
}
void switch_led(unsigned char sn,unsigned char onoff)
{
	switch(sn)
	{
		case led_normal:if(onoff==on)GPIO_ResetBits(GPIOA,GPIO_Pin_0);else GPIO_SetBits(GPIOA,GPIO_Pin_0);break;
		case led_error :if(onoff==on)GPIO_ResetBits(GPIOB,GPIO_Pin_13);else GPIO_SetBits(GPIOB,GPIO_Pin_13);break;
		case led_onoff :if(onoff==on)GPIO_ResetBits(GPIOB,GPIO_Pin_15);else GPIO_SetBits(GPIOB,GPIO_Pin_15);break;
		default: break;
		}
}

