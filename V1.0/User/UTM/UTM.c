/* utm.c - utm module API*/

/*
 * Copyright (c) 2017 Bin Hong Tech, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable BinHong-tech license agreement.
 */

/*
modification history
--------------------
01a,17jun01,cx_  add file
*/
#include <UTM.h>
#include <Param.h>
#include <string.h>
#include <Chan.h>

typedef struct
{
    u8 Flag1;               /* 0x68 */
    u16 Identify1 : 2;      /* protocol identify */
    u16 Len1 : 14;          /* len1 */
    u16 Identify2 : 2;
    u16 Len2 : 14;
    u8 Flag2;               /* 0x68 */
    u8 Resv1 : 6;           /* reserve bits */
    u8 Prm : 1;             /* prm */
    u8 Dir : 1;             /* transfer direction bit */
    u8 A1;                  /* controller type address code */
    u8 A2[5];               /* controller address ,BCD encode */
    u8 A3;                  /* master station address */
    u8 AFN;                 /* application function code */
    u8 PSEQ : 4;
    u8 CON : 1;             /* need confirm flag */
    u8 FIR_FIN : 2;         /* the first frame flag FIR, the last frame flag FIN 
                                    00 - multi frame, the middle frame 
                                    01 - multi frame, the last frame
                                    10 - multi frame, the first frame
                                    11 - single frame */
    u8 Resv2 : 1;
}Header;

typedef struct
{
    Header header;

    u8 pfc;
    
    void (*plogin_callback)(void);            /**< 登录应答回调函数                                                   */
    void (*phb_callback)(void);               /**< 心跳应答回调函数                                                   */
    
}UTMCB;

extern int CMD_Reset(void);
extern int CMD_Ctrl(void);
extern int CMD_SetParam(void);
extern int CMD_QueryParam(void);
extern int CMD_QueryRealData(void);
extern int CMD_FileTrans(void);
extern int CMD_DataTrans(void);

static UTMCB UTM;
    
static FUNCPTR AfnFunc[] = 
{
    NULL,
    (FUNCPTR)CMD_Reset,              /* 0x01 */
    NULL,
    (FUNCPTR)CMD_Ctrl,               /* 0x03 */
    (FUNCPTR)CMD_SetParam,           /* 0x04 */
    (FUNCPTR)CMD_QueryParam,           /* 0x05 */
    NULL,
    (FUNCPTR)CMD_QueryRealData,        /* 0x07 */
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (FUNCPTR)CMD_FileTrans,          /* 0x0F */
    (FUNCPTR)CMD_DataTrans           /* 0x10 */
};

static void UTM_OUTPUT(AppBuf * appBuf)
{
	  u32 i;
    u8 cs = 0u;
    Param_F3 f3;
    Header *header = (Header *)appBuf->pbuf->payLoad;   
    
    header->Flag1 = 0x68;
    header->Flag2 = 0x68;
    header->Identify1 = PROTOCOL_V0;
    header->Identify2 = PROTOCOL_V0;
    header->Dir = 0x01;
    header->Resv1 = 0;
    header->Resv2 = 0;
    header->AFN = appBuf->poa.out.afn;
    header->Prm = appBuf->poa.out.prm;
    header->FIR_FIN = appBuf->poa.out.fts;
    header->CON = appBuf->poa.out.con;

    ParamRequest(3, (void*)&f3, PARAM_OPT_READ);
    header->A1 = f3.terminalTypeAddr;
    memcpy(header->A2, f3.terminalAddr, sizeof(header->A2));

    //启动站
    if(PRM_REQU == header->Prm)
    {
        header->A3 = 0;
        header->PSEQ = (u8)(UTM.pfc & 0x0F);
        UTM.pfc++;
    }
    //从动站
    else if(PRM_RESP == header->Prm)
    {
        //从动站返回原主站地址
        header->A3 = UTM.header.A3;
        //返回原帧序号
        header->PSEQ = UTM.header.PSEQ;
    }
    else
    {
    }

    /* 计算报文长度len字段的值 */
    header->Len1 = appBuf->pbuf->len - 6;
    header->Len2 = header->Len1;
    /* 计算校验和CS,并在报文末尾附加CS和结束符 */
    
    for (i = 0; i < header->Len1; i++)
    {
        cs += *((u8 *) appBuf->pbuf->payLoad + 6 + i);
    }
    appBuf->pbuf->len += 2;

    *(((u8 *) appBuf->pbuf->payLoad + appBuf->pbuf->len) - 2) = cs;
    *(((u8 *) appBuf->pbuf->payLoad + appBuf->pbuf->len) - 1) = 0x16;

    //确认callback 注册
    if(header->CON)
    {
        u8 fn = *((u8*)appBuf->pbuf->payLoad + UTM_BSN_DATA_OFF);

        if(AFN_LINKCHECK == header->AFN)
        {
            if(1 == fn)
            {
                UTM.plogin_callback = appBuf->poa.out.callback;
            }
            else if(3 == fn)
            {
                UTM.phb_callback = appBuf->poa.out.callback;
            }    
        }
    }

    ChanSend(appBuf);
}

static void UTM_BSN_DO(AppBuf * appBuf)
{
}

void UTM_DO (AppBuf * appBuf)
{
    
    if(APPBUF_POAOUT == appBuf->bufTyp)
    {
        UTM_OUTPUT(appBuf);
    }

    if(APPBUF_POAIN == appBuf->bufTyp)
    {
        UTM_BSN_DO(appBuf);
    }

    memcpy(&UTM.header, appBuf->pbuf->payLoad, sizeof(UTM.header));

    //地址校验

    //协议校验

    //传输方向校验

    
}

void CanActiveReport(u16 canId, u8 *dataArry)
{
    AppBuf appBuf;
    PBuf * ppbuffer;

    PBUF_ALLOC(ppbuffer);
    
    APPBUF_POAOUT_INIT((&appBuf), CHAN_TYPE_GPRS, ppbuffer,
            AFN_ACTIVEREPORT, PRM_REQU, FRAME_SINGLE, 0, NULL);

    //can实时上报
    appBuf.pbuf->payLoad[appBuf.pbuf->len++] = 1;
    //canId
    appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(canId & 0x00FF);
    appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((canId >> 8) & 0x00FF);

    memcpy(&appBuf.pbuf->payLoad[appBuf.pbuf->len], dataArry, 8);
    appBuf.pbuf->len += 8;

    UTM_DO(&appBuf);

    PBUF_FREE(ppbuffer);
    appBuf.pbuf = NULL;
}

