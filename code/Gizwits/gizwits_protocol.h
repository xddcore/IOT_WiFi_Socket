/**
************************************************************
* @file         gizwits_protocol.h
* @brief        对应gizwits_protocol.c的头文件 (包含产品相关定义)
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

#ifndef _GIZWITS_PROTOCOL_H
#define _GIZWITS_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gizwits_product.h"


/** WiFi模组配置参数*/
typedef enum
{
  WIFI_RESET_MODE = 0x00,                           ///< WIFI模组复位
  WIFI_SOFTAP_MODE,                                 ///< WIFI模组softAP模式
  WIFI_AIRLINK_MODE,                                ///< WIFI模组AirLink模式
  WIFI_PRODUCTION_TEST,                             ///< MCU请求WiFi模组进入产测模式
  WIFI_NINABLE_MODE                                 ///< MCU请求模组进入可绑定模式
}WIFI_MODE_TYPE_T;                                  
                                          
#define min(a, b) (a)<(b)?(a):(b)                   ///< 获取最小值
                                                    
#define SEND_MAX_TIME       2000                    ///< 2000ms重发
#define SEND_MAX_NUM        2                       ///< 重发次数
                                                    
#define protocol_VERSION    "00000004"              ///< protocol版本号
#define P0_VERSION          "00000002"              ///< P0协议版本号
#define DEV_IS_GATEWAY   0                          ///< 设备是否为中控类,0表示否,1表示是
#define NINABLETIME  0                              ///< 绑定时间
                            

/** 协议生成事件类型*/
typedef enum
{
  STATELESS_TYPE = 0x00,                            ///< 无状态类型
  ACTION_CONTROL_TYPE,                              ///< 协议4.10 WiFi模组控制设备事件
  WIFI_STATUS_TYPE,                                 ///< 协议4.5 WiFi模组向设备MCU通知WiFi模组工作状态的变化事件
  ACTION_W2D_TRANSPARENT_TYPE,                      ///< 协议WiFi到设备MCU透传事件
  GET_NTP_TYPE,                                     ///< 协议4.13 MCU请求获取网络时间事件
  PROTOCOL_EVENT_TYPE_MAX                           ///< 枚举成员数量计算 (用户误删)
} PROTOCOL_EVENT_TYPE_T;
                                                                                       
                                                    
/** wifi信号强度数值结构体 */                       
typedef_t struct {                           
    uint8_t rssi;                                   ///< WIFI信号强度
}moduleStatusInfo_t;                                

/** 协议网络时间结构体 */
typedef_t struct
{
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint32_t ntp;
}protocolTime_t;

/** 协议命令码 */                                   
typedef_t enum                               
{                                                   
    CMD_GET_DEVICE_INTO             = 0x01,         ///< 命令字，对应协议：4.1 WiFi模组请求设备信息 中 WiFi模组发送
    ACK_GET_DEVICE_INFO             = 0x02,         ///< 命令字，对应协议：4.1 WiFi模组请求设备信息 中 设备MCU回复
            
    CMD_ISSUED_P0                   = 0x03,         ///< 命令字，对应协议：4.8 WiFi模组读取设备的当前状态、4.10 WiFi模组控制设备 中 WiFi模组发送
    ACK_ISSUED_P0                   = 0x04,         ///< 命令字，对应协议：4.8 WiFi模组读取设备的当前状态、4.10 WiFi模组控制设备 中 设备MCU回复
            
    CMD_REPORT_P0                   = 0x05,         ///< 命令字，对应协议：4.9 设备MCU向WiFi模组主动上报当前状态 中 设备MCU发送
    ACK_REPORT_P0                   = 0x06,         ///< 命令字，对应协议：4.9 设备MCU向WiFi模组主动上报当前状态 中 WiFi模组回复
            
    CMD_HEARTBEAT                   = 0x07,         ///< 命令字，对应协议：4.2 WiFi模组与设备MCU的心跳 中 WiFi模组发送
    ACK_HEARTBEAT                   = 0x08,         ///< 命令字，对应协议：4.2 WiFi模组与设备MCU的心跳 中 设备MCU回复
            
    CMD_WIFI_CONFIG                 = 0x09,         ///< 命令字，对应协议：4.3 设备MCU通知WiFi模组进入配置模式 中 设备MCU发送
    ACK_WIFI_CONFIG                 = 0x0A,         ///< 命令字，对应协议：4.3 设备MCU通知WiFi模组进入配置模式 中 WiFi模组回复
            
    CMD_SET_DEFAULT                 = 0x0B,         ///< 命令字，对应协议：4.4 设备MCU重置WiFi模组 中 设备MCU发送
    ACK_SET_DEFAULT                 = 0x0C,         ///< 命令字，对应协议：4.4 设备MCU重置WiFi模组 中 WiFi模组回复
            
    CMD_WIFISTATUS                  = 0x0D,         ///< 命令字，对应协议：4.5 WiFi模组向设备MCU通知WiFi模组工作状态的变化 中 WiFi模组发送
    ACK_WIFISTATUS                  = 0x0E,         ///< 命令字，对应协议：4.5 WiFi模组向设备MCU通知WiFi模组工作状态的变化 中 设备MCU回复
        
    CMD_MCU_REBOOT                  = 0x0F,         ///< 命令字，对应协议：4.6 WiFi模组请求重启MCU 中 WiFi模组发送
    ACK_MCU_REBOOT                  = 0x10,         ///< 命令字，对应协议：4.6 WiFi模组请求重启MCU 中 设备MCU回复
            
    CMD_ERROR_PACKAGE               = 0x11,         ///< 命令字，对应协议：4.7 非法消息通知 中 WiFi模组回应MCU对应包序号的数据包非法
    ACK_ERROR_PACKAGE               = 0x12,         ///< 命令字，对应协议：4.7 非法消息通知 中 MCU回应WiFi模组对应包序号的数据包非法

    CMD_PRODUCTION_TEST             = 0x13,         ///< 命令字，对应协议：4.11 MCU请求WiFi模组进入产测模式 中 设备MCU发送
    ACK_PRODUCTION_TEST             = 0x14,         ///< 命令字，对应协议：4.11 MCU请求WiFi模组进入产测模式 中 WiFi模组回复

    CMD_NINABLE_MODE                = 0x15,         ///< 命令字，对应协议：4.12 MCU通知WiFi模组进入可绑定模式 中 设备MCU发送
    ACK_NINABLE_MODE                = 0x16,         ///< 命令字，对应协议：4.12 MCU通知WiFi模组进入可绑定模式 中 WiFi模组回复

    CMD_GET_NTP                     = 0x17,         ///< 命令字，对应协议：4.13 MCU请求获取网络时间 中 设备MCU发送
    ACK_GET_NTP                     = 0x18,         ///< 命令字，对应协议：4.13 MCU请求获取网络时间 中 WiFi模组回复
} PROTOCOL_CMDTYPE;

/** 协议标准头结构体 */
typedef_t struct
{
    uint8_t                 head[2];                ///< 包头(header)固定为0xFFFF
    uint16_t                len;                    ///< 长度(len)是指从cmd 开始到整个数据包结束所占用的字节数
    uint8_t                 cmd;                    ///< 命令字（cmd）表示具体的命令含义，详见协议举例
    uint8_t                 sn;                     ///< 消息序号(sn)由发送方给出,接收方响应命令时需把消息序号返回给发送方
    uint8_t                 flags[2];               ///< 标志位（flag），产品填写默认0
} protocolHead_t;

/** 4.1 WiFi模组请求设备信息 协议结构体 */
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 protocolVer[8];         ///< 协议版本号
    uint8_t                 p0Ver[8];               ///< p0协议版本号
    uint8_t                 hardVer[8];             ///< 硬件版本号
    uint8_t                 softVer[8];             ///< 软件版本号
    uint8_t                 productKey[32];         ///< 产品标识码
    uint16_t                ninableTime;            ///< 绑定超时(秒)
    uint8_t                 devAttr[8];             ///< 设备属性
    uint8_t                 productSecret[32];      ///< 产品密钥
    uint8_t                 sum;                    ///< 检验和
} protocolDeviceInfo_t;

/** 协议通用数据帧(4.2、4.4、4.6、4.9、4.10) 协议结构体 */
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 sum;                    ///< 检验和
} protocolCommon_t;

/** 4.3 设备MCU通知WiFi模组进入配置模式 协议结构体 */
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 cfgMode;                ///< 配置参数
    uint8_t                 sum;                    ///< 检验和
} protocolCfgMode_t;

/** 4.13 MCU请求获取网络时间 协议结构体 */
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 time[7];                ///< 硬件版本号
    uint8_t                 ntp_time[4];            ///< 软件版本号
    uint8_t                 sum;                    ///< 检验和
} protocolUTT_t;

/** WiFi模组工作状态*/
typedef union
{
    uint16_t                value;
    struct
    {
        uint16_t            softap:1;               ///< 表示WiFi模组所处的SOFTAP模式状态，类型为bool
        uint16_t            station:1;              ///< 表示WiFi模组所处的STATION模式状态，类型为bool
        uint16_t            onboarding:1;           ///< 表示WiFi模组所处的配置状态，类型为bool
        uint16_t            binding:1;              ///< 表示WiFi模组所处的绑定状态，类型为bool
        uint16_t            con_route:1;            ///< 表示WiFi模组与路由器的连接状态，类型为bool
        uint16_t            con_m2m:1;              ///< 表示WiFi模组与云端m2m的状态，类型为bool
        uint16_t            reserve1:2;             ///< 数据位补齐
        uint16_t            rssi:3;                 ///< 表示路由的信号强度，类型为数值
        uint16_t            app:1;                  ///< 表示WiFi模组与APP端的连接状态，类型为bool
        uint16_t            test:1;                 ///< 表示WiFi模组所处的场测模式状态，类型为bool
        uint16_t            reserve2:3;             ///< 数据位补齐
    }types; 
    
} wifiStatus_t;

/** WiFi状态类型 协议结构体 */
typedef struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    wifiStatus_t            ststus;                 ///< WIFI状态
    uint8_t                 sum;                    ///< 检验和
} protocolWifiStatus_t;

/** 非法报文类型*/  
typedef_t enum
{
    ERROR_ACK_SUM = 0x01,                           ///< 校验错误
    ERROR_CMD     = 0x02,                           ///< 命令码错误
    ERROR_OTHER   = 0x03,                           ///< 其他
} errorPacketsType_t;

/** 4.7 非法消息通知 协议结构体*/
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 error;                  ///< 错误值
    uint8_t                 sum;                    ///< 检验和
} protocolErrorType_t;


/** P0报文标准头 */
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 action;                 ///< p0 命令位
} protocolP0Head_t;

/** 环形缓冲区数据结构 */
typedef_t struct {
    size_t rbCapacity;
    uint8_t  *rbHead;
    uint8_t  *rbTail;
    uint8_t  *rbBuff;
}rb_t;


/**@name Product Key (产品标识码)
* @{
*/
#define PRODUCT_KEY "26cabbe05a3740ec866cdc1bd65dc3ce"
/**@} */
/**@name Product Secret (产品密钥)
* @{
*/
#define PRODUCT_SECRET "069dcd79f7344960997836c61d81cb8c"
/**@} */
#define MAX_PACKAGE_LEN    (sizeof(devStatus_t)+sizeof(attrFlags_t)+10)                ///< 数据缓冲区最大长度
#define RB_MAX_LEN          (MAX_PACKAGE_LEN*2)     ///< 环形缓冲区最大长度
/**@name 数据点相关定义
* @{
*/
#define ONOFF_BYTEOFFSET                    0
#define ONOFF_BITOFFSET                     0
#define ONOFF_LEN                           1

#define TIME_ONOFF_BYTEOFFSET                    0
#define TIME_ONOFF_BITOFFSET                     1
#define TIME_ONOFF_LEN                           1

#define COUNTDOWN_ONOFF_BYTEOFFSET                    0
#define COUNTDOWN_ONOFF_BITOFFSET                     2
#define COUNTDOWN_ONOFF_LEN                           1


#define WEEK_REPEAT_RATIO                         1
#define WEEK_REPEAT_ADDITION                      0
#define WEEK_REPEAT_MIN                           0
#define WEEK_REPEAT_MAX                           255

#define TIME_ON_MINUTE_RATIO                         1
#define TIME_ON_MINUTE_ADDITION                      0
#define TIME_ON_MINUTE_MIN                           0
#define TIME_ON_MINUTE_MAX                           1440

#define TIME_OFF_MINUTE_RATIO                         1
#define TIME_OFF_MINUTE_ADDITION                      0
#define TIME_OFF_MINUTE_MIN                           0
#define TIME_OFF_MINUTE_MAX                           1440

#define COUNTDOWN_MINUTE_RATIO                         1
#define COUNTDOWN_MINUTE_ADDITION                      0
#define COUNTDOWN_MINUTE_MIN                           0
#define COUNTDOWN_MINUTE_MAX                           1440

#define POWER_CONSUMPTION_RATIO                         1
#define POWER_CONSUMPTION_ADDITION                      0
#define POWER_CONSUMPTION_MIN                           0
#define POWER_CONSUMPTION_MAX                           65535

#define ELECTRIC_RATIO                         0.01
#define ELECTRIC_ADDITION                      0
#define ELECTRIC_MIN                           0
#define ELECTRIC_MAX                           1200

#define FUSE_FAILURE_RATIO                         0.001
#define FUSE_FAILURE_ADDITION                      0
#define FUSE_FAILURE_MIN                           0
#define FUSE_FAILURE_MAX                           1000

/**@} */


/** 可写型数据点 布尔和枚举变量 所占字节大小*/
#define COUNT_W_BIT 1








/** 事件枚举*/
typedef enum
{
  WIFI_SOFTAP = 0x00,                               ///< WiFi SOFTAP 配置事件
  WIFI_AIRLINK,                                     ///< WiFi模块 AIRLINK 配置事件
  WIFI_STATION,                                     ///< WiFi模块 STATION 配置事件
  WIFI_OPEN_BINDING,                                ///< WiFi模块开启绑定事件
  WIFI_CLOSE_BINDING,                               ///< WiFi模块关闭绑定事件
  WIFI_CON_ROUTER,                                  ///< WiFi模块已连接路由事件
  WIFI_DISCON_ROUTER,                               ///< WiFi模块已断开连接路由事件
  WIFI_CON_M2M,                                     ///< WiFi模块已连服务器M2M事件
  WIFI_DISCON_M2M,                                  ///< WiFi模块已断开连服务器M2M事件
  WIFI_OPEN_TESTMODE,                               ///< WiFi模块开启测试模式事件
  WIFI_CLOSE_TESTMODE,                              ///< WiFi模块关闭测试模式事件
  WIFI_CON_APP,                                     ///< WiFi模块连接APP事件
  WIFI_DISCON_APP,                                  ///< WiFi模块断开APP事件
  WIFI_RSSI,                                        ///< WiFi模块信号事件
  WIFI_NTP,                                         ///< 网络时间事件
  TRANSPARENT_DATA,                                 ///< 透传事件
  EVENT_ONOFF,
  EVENT_TIME_ONOFF,
  EVENT_COUNTDOWN_ONOFF,
  EVENT_WEEK_REPEAT,
  EVENT_TIME_ON_MINUTE,
  EVENT_TIME_OFF_MINUTE,
  EVENT_COUNTDOWN_MINUTE,
  EVENT_TYPE_MAX                                    ///< 枚举成员数量计算 (用户误删)
} EVENT_TYPE_T;


/** 用户区设备状态结构体*/
typedef_t struct {
  bool valueOnOff;
  bool valueTime_OnOff;
  bool valueCountDown_OnOff;
  uint32_t valueWeek_Repeat;
  uint32_t valueTime_On_Minute;
  uint32_t valueTime_Off_Minute;
  uint32_t valueCountDown_Minute;
  uint32_t valuePower_Consumption;
  float valueelectric;
  float valueFuse_failure;
} dataPoint_t;

/** 对应协议“4.10 WiFi模组控制设备”中的标志位"attr_flags" */ 
typedef_t struct {
  uint8_t flagOnOff:1;
  uint8_t flagTime_OnOff:1;
  uint8_t flagCountDown_OnOff:1;
  uint8_t flagWeek_Repeat:1;
  uint8_t flagTime_On_Minute:1;
  uint8_t flagTime_Off_Minute:1;
  uint8_t flagCountDown_Minute:1;
} attrFlags_t;

/** 对应协议“4.10 WiFi模组控制设备”中的数据值"attr_vals" */
typedef_t struct {
  uint8_t wBitBuf[COUNT_W_BIT];
  uint8_t valueWeek_Repeat;
  uint16_t valueTime_On_Minute;
  uint16_t valueTime_Off_Minute;
  uint16_t valueCountDown_Minute;
} attrVals_t;

/** 对应协议“4.10 WiFi模组控制设备”中“P0协议区”的标志位"attr_flags(1B)" + 数据值"attr_vals(6B)" */ 
typedef_t struct {
    attrFlags_t attrFlags;
    attrVals_t  attrVals;
}gizwitsIssued_t;

/** 对应协议“4.9 设备MCU向WiFi模组主动上报当前状态”中的设备状态"dev_status(11B)" */ 
typedef_t struct {
  uint8_t wBitBuf[COUNT_W_BIT];

  uint8_t valueWeek_Repeat;
  uint16_t valueTime_On_Minute;
  uint16_t valueTime_Off_Minute;
  uint16_t valueCountDown_Minute;


  uint16_t valuePower_Consumption;
  uint16_t valueelectric;
  uint16_t valueFuse_failure;


} devStatus_t; 

/** P0 command 命令码*/
typedef enum
{
    ACTION_CONTROL_DEVICE       = 0x01,             ///< 协议4.10 WiFi模组控制设备 WiFi模组发送
    ACTION_READ_DEV_STATUS      = 0x02,             ///< 协议4.8 WiFi模组读取设备的当前状态 WiFi模组发送
    ACTION_READ_DEV_STATUS_ACK  = 0x03,             ///< 协议4.8 WiFi模组读取设备的当前状态 设备MCU回复
    ACTION_REPORT_DEV_STATUS    = 0X04,             ///< 协议4.9 设备MCU向WiFi模组主动上报当前状态 设备MCU发送
    ACTION_W2D_TRANSPARENT_DATA = 0x05,             ///< WiFi到设备MCU透传
    ACTION_D2W_TRANSPARENT_DATA = 0x06,             ///< 设备MCU到WiFi透传
} actionType_t;

                                                                                       
/** 事件队列结构体 */                               
typedef_t struct {                           
    uint8_t num;                                    ///< 队列成员个数
    uint8_t event[EVENT_TYPE_MAX];                  ///< 队列成员事件内容
}eventInfo_t;                                
 

/** 对应协议“4.9 设备MCU向WiFi模组主动上报当前状态” 中的 设备状态位"dev_status(11B)"  */ 
typedef_t struct {                           
    devStatus_t devStatus;                          ///< 作用：用来存放待上报的设备状态数据
}gizwitsReport_t;
/** 重发机制结构体 */
typedef_t struct {
    uint8_t                 num;                    ///< 重发次数
    uint8_t                 flag;                   ///< 1,表示有需要等待的ACK;0,表示无需要等待的ACK
    uint8_t                 buf[MAX_PACKAGE_LEN];   ///< 重发数据缓冲区
    uint16_t                dataLen;                ///< 重发数据长度
    uint32_t                sendTime;               ///< 重发的系统时间
} protocolWaitAck_t;

/** 4.8 WiFi模组读取设备的当前状态 设备MCU回复结构体 */
typedef_t struct
{
    protocolHead_t          head;                   ///< 协议标准头结构体
    uint8_t                 action;                 ///< p0 命令位
    gizwitsReport_t         reportData;             ///< 上报数据区
    uint8_t                 sum;                    ///< 检验和
} protocolReport_t;

/** 协议处理总结构体 */
typedef_t struct
{
    uint8_t issuedFlag;                             ///< 保存对应的 p0命令类型
    uint8_t protocolBuf[MAX_PACKAGE_LEN];           ///< 协议报文存储区
    uint8_t transparentBuff[MAX_PACKAGE_LEN];       ///< 透传数据存储区
    uint32_t transparentLen;                        ///< 透传数据长度
    
    uint32_t sn;                                    ///< 消息序号
    protocolWaitAck_t waitAck;                      ///< 协议ACK数据结构
    
    eventInfo_t issuedProcessEvent;                 ///< 控制事件
    eventInfo_t wifiStatusEvent;                    ///< WIFI状态 事件
    eventInfo_t NTPEvent;                           ///< NTP 事件

    dataPoint_t gizCurrentDataPoint;                ///< 当前设备状态数据
    dataPoint_t gizLastDataPoint;                   ///< 上次上报的设备状态数据
    gizwitsReport_t reportData;                     ///< 协议上报实际数据
    moduleStatusInfo_t wifiStatusData;              ///< WIFI 状态信息(信号强度)
    protocolTime_t TimeNTP;                         ///< 网络时间信息
}gizwitsProtocol_t; 

/**@name Gizwits 用户API接口
* @{
*/
void gizwitsInit(void);
int32_t gizwitsSetMode(uint8_t mode);
void gizwitsGetNTP(void);
int32_t gizwitsHandle(dataPoint_t *currentData);
int32_t gizwitsPassthroughData(uint8_t * data, uint32_t len);
/**@} */

int32_t gizPutData(uint8_t *buf, uint32_t len);

#endif
