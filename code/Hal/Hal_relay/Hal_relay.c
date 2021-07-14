#include "hal_relay.h"
void relay_init()
{
		 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //״̬����ָʾ
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);				 
}
void relay_control(unsigned char onoff)
{
	if(onoff==relay_on){
//		relay_flag=1;
		GPIO_SetBits(GPIOB,GPIO_Pin_8);
	}
	else{
	//	relay_flag=0;
		GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	}
}

