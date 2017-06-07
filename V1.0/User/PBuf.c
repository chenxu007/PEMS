/* PBuf.c - PBuf module API*/

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
#include <PBuf.h>
#include <stm32f10x.h>

#define PBUF_ITEM_MAX   4

static PBuf sysPBuf[PBUF_ITEM_MAX];

PBuf * PBufAlloc (void)
{
    u32 idx;
    
    for(idx = 0; idx < PBUF_ITEM_MAX; idx++)
    {
        if(0 == sysPBuf[idx].isUsed)
        {
            sysPBuf[idx].isUsed = 1;
            return (&sysPBuf[idx]);
        }
            
    }
    
    return NULL;
}

void PBufFree(PBuf * pbuf)
{
    pbuf->isUsed = 0;
}
