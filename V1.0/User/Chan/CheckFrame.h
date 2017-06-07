
#ifndef CHKFRAME_H_
#define CHKFRAME_H_

#include <Chan.h>
#include <AppBuf.h>
//#inlcude <time.h>
#include "stm32f10x.h"


typedef struct CheckFrameCB_
{
    void (* pchkfrm)(struct CheckFrameCB_* chk, ChanType type, u8 *recvBuf, u32 recvLen);

    u8 frameState;                       /* 报文状态                                  */
//    time_t updateTime;                 /* 更新时间                                  */
    u8 overTime;                         /* 报文等待超时时间                          */
    u32 dLen;                            /* 用户数据长度,解析自报文长度域             */
    u32 cfmLen;                          /* 用户数据确认长度，用于解析数据长度临时用  */

    ChanType chanType;                   /* 报文信道来源类型                          */
    PBuf *ppbuf;                         /* 报文池                                    */
    u16 pbufPos;                         /* 报文接收字节偏移位置                      */
    u8 cs;                               /* 校验和                                    */
} CheckFrameCB;


extern CheckFrameCB *CheckFrameInit(void);


#endif
