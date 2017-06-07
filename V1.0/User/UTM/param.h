#ifndef __PARAM_H__
#define __PARAM_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"


#define CAN_REPORT_MODE_REAL    0
#define CAN_REPORT_MODE_PERIOD  1

typedef enum
{
    PARAM_OPT_READ = 0,
    PARAM_OPT_WRITE = 1
}ParamOption;


typedef struct
{
    u8 transmitDelay;       //单位为秒
    u8 retryTimes;          //重发次数
    u8 heartBeatInterval;   //心跳周期，单位分钟
}Param_F1;

typedef struct
{
    u8 mIP[4];              //主IP
    u16 mPort;              //主端口
    u8 bIP[4];              //备IP
    u16 bPort;              //备端口
    u8 apn[16];             //apn
}Param_F2;

typedef struct
{
    u8 terminalTypeAddr;    //控制器类型地址
    u8 terminalAddr[5];     //控制器地址
}Param_F3;

typedef struct
{
    u16 gpsReportInterval;  //GPS位置信息上报周期，单位秒
}Param_F4;

typedef struct
{
    u8 canReportMode;       //CAN主动上报方式
    u32 canReportInterval;  //CAN主动上报周期，单位秒
}Param_F5;

typedef struct
{
    Param_F1 f1;
    Param_F2 f2;
    Param_F3 f3;
    Param_F4 f4;
    Param_F5 f5;
}Param;
 
extern void ParamRequest (u8 fn, void *data, ParamOption opt);

#ifdef __cplusplus
}
#endif 

#endif
