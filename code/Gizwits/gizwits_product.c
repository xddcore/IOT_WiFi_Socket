/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits 控制协议处理,及平台相关的硬件初始化 
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

#include <stdio.h>
#include <string.h>
#include "Hal_Usart/hal_uart.h"
#include "gizwits_protocol.h"
#include "hal_relay.h"
#include "hal_led.h"
#include "24cxx.h" 
	u8 sys_h;
	u8 sys_min;
	extern	u8 sys_week;//系统week
	u8 const table_week[12]={0,3,3,6,1,4,6,2,5,0,3,5}; //月修正数据表	  
//平年的月份日期表
const u8 mon_table[12]={31,28,31,30,31,30,31,31,30,31,30,31};
	u8 RTC_Get_Week(u16 year,u8 month,u8 day)
{	
	u16 temp2;
	u8 yearH,yearL;
	
	yearH=year/100;	yearL=year%100; 
	// 如果为21世纪,年份数加100  
	if (yearH>19)yearL+=100;
	// 所过闰年数只算1900年之后的  
	temp2=yearL+yearL/4;
	temp2=temp2%7; 
	temp2=temp2+day+table_week[month-1];
	if (yearL%4==0&&month<3)temp2--;
	return(temp2%7);
}
/**@} */
/**@name Gizwits 用户API接口
* @{
*/
extern dataPoint_t currentDataPoint;
/**
* @brief 事件处理接口

* 说明：

* 1.用户可以对WiFi模组状态的变化进行自定义的处理

* 2.用户可以在该函数内添加数据点事件处理逻辑，如调用相关硬件外设的操作接口

* @param[in] info : 事件队列
* @param[in] data : 协议数据
* @param[in] len : 协议数据长度
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *data, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)data;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)data;
  protocolTime_t *ptime = (protocolTime_t *)data;

  if((NULL == info) || (NULL == data))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_ONOFF:
        currentDataPoint.valueOnOff = dataPointPtr->valueOnOff;
        GIZWITS_LOG("Evt: EVENT_ONOFF %d \n", currentDataPoint.valueOnOff);
        if(0x01 == currentDataPoint.valueOnOff)
        {
        relay_control(relay_on);  //user handle
        }
        else
        {
         relay_control(relay_off); //user handle    
        }
        break;
      case EVENT_TIME_ONOFF:
        currentDataPoint.valueTime_OnOff = dataPointPtr->valueTime_OnOff;
        GIZWITS_LOG("Evt: EVENT_TIME_ONOFF %d \n", currentDataPoint.valueTime_OnOff);
        if(0x01 == currentDataPoint.valueTime_OnOff)
        {
          AT24CXX_WriteOneByte(address_valueTime_OnOff,currentDataPoint.valueTime_OnOff);//写入数据 //user handle
        }
        else
        {
          AT24CXX_WriteOneByte(address_valueTime_OnOff,currentDataPoint.valueTime_OnOff);//写入数据 //user handle    
        }
        break;
      case EVENT_COUNTDOWN_ONOFF:
        currentDataPoint.valueCountDown_OnOff = dataPointPtr->valueCountDown_OnOff;
       GIZWITS_LOG("Evt: EVENT_COUNTDOWN_ONOFF %d \n", currentDataPoint.valueCountDown_OnOff);
        if(0x01 == currentDataPoint.valueCountDown_OnOff)
        {
          AT24CXX_WriteOneByte(address_valueCountDown_OnOff,currentDataPoint.valueCountDown_OnOff);//写入数据//user handle
        }
        else
        {
          AT24CXX_WriteOneByte(address_valueCountDown_OnOff,currentDataPoint.valueCountDown_OnOff);//写入数据//user handle    
        }
        break;


      case EVENT_WEEK_REPEAT:
        currentDataPoint.valueWeek_Repeat = dataPointPtr->valueWeek_Repeat;
       GIZWITS_LOG("Evt:EVENT_WEEK_REPEAT %d\n",currentDataPoint.valueWeek_Repeat);
       AT24CXX_WriteLenByte(address_valueWeek_Repeat,currentDataPoint.valueWeek_Repeat,4);//写入数据  //user handle
        break;
      case EVENT_TIME_ON_MINUTE:
        currentDataPoint.valueTime_On_Minute = dataPointPtr->valueTime_On_Minute;
        GIZWITS_LOG("Evt:EVENT_TIME_ON_MINUTE %d\n",currentDataPoint.valueTime_On_Minute);
        AT24CXX_WriteLenByte(address_valueTime_On_Minute,currentDataPoint.valueTime_On_Minute,4);//写入数据 //user handle
        break;
      case EVENT_TIME_OFF_MINUTE:
        currentDataPoint.valueTime_Off_Minute = dataPointPtr->valueTime_Off_Minute;
       GIZWITS_LOG("Evt:EVENT_TIME_OFF_MINUTE %d\n",currentDataPoint.valueTime_Off_Minute);
       AT24CXX_WriteLenByte(address_valueTime_Off_Minute,currentDataPoint.valueTime_Off_Minute,4);//写入数据  //user handle
        break;
      case EVENT_COUNTDOWN_MINUTE:
        currentDataPoint.valueCountDown_Minute = dataPointPtr->valueCountDown_Minute;
        GIZWITS_LOG("Evt:EVENT_COUNTDOWN_MINUTE %d\n",currentDataPoint.valueCountDown_Minute);
        AT24CXX_WriteLenByte(address_valueCountDown_Minute,currentDataPoint.valueCountDown_Minute,4);//写入数据 //user handle
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
        break;
      case WIFI_DISCON_ROUTER:
        break;
      case WIFI_CON_M2M:
				switch_led(led_normal,off);//成功连接服务器，配置成功，关闭绿灯
        break;
      case WIFI_DISCON_M2M:
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
         	  sys_week=RTC_Get_Week(ptime->year,ptime->month,ptime->day);//系统week
			      sys_h=ptime->hour;
	          sys_min=ptime->minute;
			break;
      default:
        break;
    }
  }

  return 0;
}



/**
* @brief 系统毫秒计时维护函数,毫秒自增,溢出归零

* @param none
* @return none
*/
static uint32_t timerMsCount;
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief 读取系统时间毫秒计时数

* @param none
* @return 系统时间毫秒数
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}


/**
* @brief MCU复位函数

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

/**@} */

/**
* @brief 定时器TIM3中断处理函数

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
    if (TIM_GetITStatus(TIMER, TIM_IT_Update) != RESET)  
    {
        TIM_ClearITPendingBit(TIMER, TIM_IT_Update  );
        gizTimerMs();
    }
}

/**
* @brief 定时器TIM3定时器初始化

* @param none
* @return none
*/
void timerInit(void)
{
    u16 arr = 7199;
    u16 psc = 9;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(TIMER_RCC, ENABLE); //时钟使能

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位0

    TIM_ITConfig(TIMER, TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIMER_IRQ;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //优占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
    TIM_Cmd(TIMER, ENABLE);  //使能TIMx
}

/**
* @brief USART2串口中断函数

* 接收功能，用于接收与WiFi模组间的串口协议数据
* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
    uint8_t value = 0;
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2,USART_IT_RXNE);
        value = USART_ReceiveData(USART2);

        gizPutData(&value, 1);
    }
}


/**
* @brief 串口写操作，发送数据到WiFi模组
*
* @param buf      : 数据地址
* @param len       : 数据长度
*
* @return : 正确返回有效数据长度;-1，错误返回
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
#endif
    
    for(i=0; i<len; i++)
    {
        USART_SendData(UART, buf[i]);
        while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);//Loop until the end of transmission
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", buf[i]);
#endif
        if(i >=2 && buf[i] == 0xFF)
        {
          USART_SendData(UART,0x55);
          while (USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);//Loop until the end of transmission
#ifdef PROTOCOL_DEBUG
        GIZWITS_LOG("%02x ", 0x55);
#endif
        }
    }
    
#ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("\n");
#endif
    
    return len;
}

/**
* @brief USART串口初始化

* 用于与WiFi模组间的串口通信
* @param none
* @return none
*/
void uartInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    UART_GPIO_Cmd(UART_GPIO_CLK, ENABLE);
    UART_CLK_Cmd(UART_CLK, ENABLE);
    UART_AFIO_Cmd(UART_AFIO_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = UART_TxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = UART_RxPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_GPIO_PORT, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = UART_BAUDRATE;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART, &USART_InitStructure);

    USART_ITConfig(UART,USART_IT_RXNE,ENABLE);
    USART_Cmd(UART, ENABLE);
    USART_ClearFlag(UART, USART_FLAG_TC); /*清空发送完成标志,Transmission Complete flag */

    /*使能串口中断,并设置优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

