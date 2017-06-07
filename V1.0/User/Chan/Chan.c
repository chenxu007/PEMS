/* Chan.c*/

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
#include <chan.h>
#include <assert.h>
#include <Wlm.h>
#include <AppBuf.h>
#include <UTM.h>

void ChanInit(void)
{
    WlmInit(); 
};

void ChanSend(AppBuf *appBuf)
{
    switch(appBuf->chTyp)
    {
        case CHAN_TYPE_GPRS:
            WlmSend(appBuf);
            break;
        default:
            assert(0);
    }
}

void ChanSendLinkFrame(ChanType chType, LinkType linkType, void (*onConfirmFun)(void))
{
    PBuf * ppbuf;
    AppBuf appBuf;
    
    PBUF_ALLOC(ppbuf);
    APPBUF_POAOUT_INIT((&appBuf), chType, ppbuf, AFN_LINKCHECK, PRM_REQU, FRAME_SINGLE, 1, onConfirmFun);
    
    ppbuf->payLoad[ppbuf->len++] = (u8)linkType;
    
    UTM_DO(&appBuf);
    
    PBUF_FREE(ppbuf);
    appBuf.pbuf = NULL;
}
