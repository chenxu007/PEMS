/* Wlm.c*/

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

#include <Wlm.h>
#include <CheckFrame.h>
#include <param.h>
#include <assert.h>
#include <string.h>
#include <Bsp.h>
#include <M35.h>

typedef struct
{
    WlmParam Param;
    WlmInfo Info;
    CheckFrameCB *CheckFrame;
    
    //pravite
}WlmCB;

static WlmCB WirelessModuleCB;

void WlmInit(void)
{
    Param_F1 f1;
    Param_F2 f2;
        
    ParamRequest(1, (void *)& f1, PARAM_OPT_READ);        
    WirelessModuleCB.Param.transmitDelay = f1.transmitDelay;
    WirelessModuleCB.Param.retryTimes = f1.retryTimes;
    WirelessModuleCB.Param.heartBeatInterval = f1.heartBeatInterval;

    ParamRequest(2, (void *)& f2, PARAM_OPT_READ);
    memcpy(WirelessModuleCB.Param.mIP, f2.mIP, sizeof(WirelessModuleCB.Param.mIP));    
    WirelessModuleCB.Param.mPort = f2.mPort;
    memcpy(WirelessModuleCB.Param.bIP, f2.bIP, sizeof(WirelessModuleCB.Param.bIP));
    WirelessModuleCB.Param.bPort = f2.bPort;
    memcpy(WirelessModuleCB.Param.apn, f2.apn, sizeof(WirelessModuleCB.Param.apn));
    WirelessModuleCB.CheckFrame = CheckFrameInit();
    
//     M35_GPIOInit();  //手机模块控制引脚配置
//     NVIC_Config();
//     M35_USARTx_Config(115200);

//     LED_init();
//     do
//     {
//         M35_Start();                                  //启动M35模块
//     }while(M35_TCPConnect() == ERROR);
}

void WlmSend(AppBuf *appBuf)
{ 
    u8 cmd[20];

    strcpy(cmd, "AT+QISEND=");
    U16AddToAsciiString(cmd, appBuf->pbuf->len);
    strcat(cmd,"\r\n");
    
    if (SIM_SendCmd(cmd,"\r\n>","NULL","NULL",2000,1) != 1)
        assert(0);
		
    if(SIM_SendBuf(appBuf->pbuf->payLoad, appBuf->pbuf->len, "\r\nSEND OK","NULL","NULL",5000,1) != 1)
        assert(0);
		
//		if (SIM_SendCmd("\x1a","\r\nSEND OK","NULL","NULL",2000,1) != 1)
//			  assert(0);
}

