#ifndef __WLM_H__
#define __WLM_H__

#include "stm32f10x.h"
#include <AppBuf.h>

typedef struct
{
    u8 transmitDelay;       //单位为秒
    u8 retryTimes;          //重发次数
    u8 heartBeatInterval;   //心跳周期，单位分钟
    
    u8 mIP[4];              //主IP
    u16 mPort;              //主端口
    u8 bIP[4];              //备IP
    u16 bPort;              //备端口
    u8 apn[16];             //apn
}WlmParam;

typedef struct
{
    bool   moduleisinsert;   /**< 模块是否插入*/
    bool   flagoflogined;    /**< 是否登录到主站 */
    bool   flagofconnect;    /**< 是否建立连接*/
//    oper_type_e  oper_type;  /**< 运营商类型*/
//    modulever_t  mver;       /**< 模块版本信息*/
    s32  ncsq;             /**< 信号强度*/
    u32 flow;             /**< 总的流量,提供给统计进行*/
    u8  imsicode[16];     /**< IMSI码*/
//    wlm_states_e  nlinkstate;       /**< 连接状态*/
//    time_t nlasttime;        /**< 最后一次接收到数据的时间*/
//   ip_addr_t lastconnip;    /**< 最后一次连接成功的主站IP,区分主IP还是备IP */
//    uint16_t lastconnport;   /**< 最后一次连接成功的主站端口*/
}WlmInfo;

extern void WlmInit(void);
extern void WlmSend(AppBuf *appBuf);
#endif

