/* CheckFrame.c*/

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

#include <AppBuf.h>
#include <Chan.h>
#include <assert.h>
#include <CheckFrame.h>
#include <string.h>
#include <UTM.h>

/*  structure of normal frame from the Data Link Layer

 Flag            ( 1 Byte 68H)
 Length          ( 2 Bytes)
 Length          ( 2 Bytes)
 Flag            ( 1 Byte 68H)
 Control         ( 1 Byte )
 Addr            ( 5 Byte rtu logical address )
 DATA            ( n Bytes, depend only from the length)
 CS              ( 1 Byte )
 Flag            ( 1 Byte 16H)
 */

/*  structure of encrypt frame from the Data Link Layer
 APN             ( 1 Byte application layer function code )
 SEQ             ( 1 Byte frame sequence code )
 DA              ( 2 Bytes information point )
 DT              ( 2 Bytes information type )
 Data unit       ( n Bytes data unit)
 ...             ( DAn, DTn, Data units... )
 DA              ( 2 Bytes information point )
 DT              ( 2 Bytes information type )
 Data unit       ( n Bytes data unit)
 AUX             ( 8/22 Bytes  )

 */

#define FRAME_NO_DATA_LEN               14   /**< 无数据内容有数据标示的报文头长度 */

/* states for scanning incomming bytes from a bytestream */
#define FRAME_STATES_NULL               0    /**< no synchronisation */
#define FRAME_STATES_FLAG_FIR           1    /**< have the first Flag Byte received */
#define FRAME_STATES_LEN1_1             2    /**< have the length byte */
#define FRAME_STATES_LEN1_2             3    /**< have the length byte */
#define FRAME_STATES_LEN2_1             4    /**< have the confirm length byte */
#define FRAME_STATES_LEN2_2             5    /**< have the confirm length byte */
#define FRAME_STATES_FLAG_SEC           6    /**< have the second Flag Byte received */
#define FRAME_STATES_CONTROL            7    /**< have the control byte */
#define FRAME_STATES_A3                 8    /**< have the A3 byte */
#define FRAME_STATES_LINK_USER_DATA     9    /**< have the link user data bytes */
#define FRAME_STATES_CS                 10   /**< wait for the CS */
#define FRAME_STATES_END                11   /**< wait for the 16H */
#define FRAME_STATES_COMPLETE           12   /**< complete frame */


#define CHKFRAME_TIMEOUT                10   /**< 报文超时时间默认10秒  */


static CheckFrameCB thechk[CHAN_TYPE_SIZE];
static u8 thechk_id = 0u;


static void
chk_frm(CheckFrameCB *chk,
        ChanType type,
        u8 *rxBuf,
        u32 rxLen);


CheckFrameCB *
CheckFrameInit(void)
{
    if (thechk_id >= CHAN_TYPE_SIZE)
    {
        return NULL;
    }
    thechk[thechk_id].frameState = FRAME_STATES_NULL;
//    thechk[thechk_id].updateTime = time(NULL);
    thechk[thechk_id].overTime = CHKFRAME_TIMEOUT;
    thechk[thechk_id].dLen = 0;
    thechk[thechk_id].cfmLen = 0;
    thechk[thechk_id].ppbuf = NULL;    

    thechk[thechk_id].pchkfrm = chk_frm;
    thechk_id++;
    return &thechk[thechk_id - 1];
}


static void
chk_frm(CheckFrameCB *chk,
        ChanType chanType,
        u8 *rxBuf,
        u32 rxLen)
{
    /* 如果已经完成的桢则重新开始 */
    if (chk->frameState == FRAME_STATES_COMPLETE)
    {
        chk->frameState = FRAME_STATES_NULL;
    }
    /* 如果信道变化则重新开始 */
    if (chanType != chk->chanType)
    {
        chk->chanType = chanType;
        chk->frameState = FRAME_STATES_NULL;
    }

    /* 如果超时则重新开始 */
//     if (abs(time(NULL) - chk->updateTime) > chk->overTime)
//     {
//         chk->frameState = FRAME_STATES_NULL;
//     }

    while (rxLen > 0)
    {

        switch (chk->frameState)
        {
            case FRAME_STATES_NULL:

                if (*rxBuf == 0x68)
                {
                    /* 如果之前申请过但未检测为完整则先free */
                    if (chk->ppbuf != NULL)
                    {
                        PBUF_FREE(chk->ppbuf);
                    }

                    /* 接收帧开始，如果pbuf为空则申请 */
                    PBUF_ALLOC(chk->ppbuf);
                    
                    chk->pbufPos = 0;

                    chk->frameState = FRAME_STATES_LEN1_1;
//                    chk->updateTime = time(NULL);
                    chk->dLen = 0;
                }
                break;

            case FRAME_STATES_LEN1_1: /* 检测L1的低字节 */
                chk->frameState = FRAME_STATES_LEN1_2;/* 为兼容主站不检测规约类型 */
                chk->dLen = ((*rxBuf & 0xFCu) >> 2u);
                break;

            case FRAME_STATES_LEN1_2: /* 检测L1的高字节 */
                chk->dLen += ((u32)*rxBuf << 6u);
                if (chk->dLen > (PBUF_BYTE_MAX - 8))
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    assert(0);
                }
                else
                {
                    chk->frameState = FRAME_STATES_LEN2_1;
                }
                break;

            case FRAME_STATES_LEN2_1: /*检测L2的低字节*/
                chk->frameState = FRAME_STATES_LEN2_2;
                chk->cfmLen = ((*rxBuf & 0xFCu) >> 2u);
                break;

            case FRAME_STATES_LEN2_2: /*检测L2的高字节*/
                chk->cfmLen += ((u32)*rxBuf << 6u);
                if (chk->cfmLen == chk->dLen)
                {
                    chk->frameState = FRAME_STATES_FLAG_SEC;
                }
                else
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    assert(0);
                }
                break;

            case FRAME_STATES_FLAG_SEC:
                if (*rxBuf == 0x68)
                {
                    chk->frameState = FRAME_STATES_CONTROL;
                }
                else
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    assert(0);
                }
                break;

            case FRAME_STATES_CONTROL:
                chk->cs = *rxBuf;
                chk->frameState = FRAME_STATES_A3;/* 不能检测方向，因为级联有上行报文 */
                break;

            case FRAME_STATES_A3:
                chk->cs += *rxBuf;
                if (chk->pbufPos == 13)
                {
                    chk->frameState = FRAME_STATES_LINK_USER_DATA;
                }
                break;

            case FRAME_STATES_LINK_USER_DATA:
                chk->cs += *rxBuf;
                if (chk->pbufPos == (5 + chk->dLen))
                {
                    chk->frameState = FRAME_STATES_CS;
                }
                break;

            case FRAME_STATES_CS:
                if (*rxBuf == chk->cs)
                {
                    chk->frameState = FRAME_STATES_END;
                }
                else
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    assert(0);
                }
                break;

            case FRAME_STATES_END:
                if (*rxBuf == 0x16)
                {
                    chk->frameState = FRAME_STATES_COMPLETE;
                }
                else
                {
                    chk->frameState = FRAME_STATES_NULL;
                    PBUF_FREE(chk->ppbuf);
                    assert(0);
                }
                break;
            default:
                break;
        }

        if (chk->frameState != FRAME_STATES_NULL)
        {
            memcpy((u8 *) chk->ppbuf->payLoad + chk->pbufPos, rxBuf, 1);
            chk->pbufPos++;
        }

        /* 完整报文，调用处理函数接口 */
        if (chk->frameState == FRAME_STATES_COMPLETE)
        {
            AppBuf rxAppBuf;
					
            chk->ppbuf->len = chk->pbufPos;

            APPBUF_RAW_INIT((&rxAppBuf), chanType, chk->ppbuf);

            UTM_DO(&rxAppBuf);

            PBUF_FREE(chk->ppbuf);
            
            chk->frameState = FRAME_STATES_NULL;
            chk->pbufPos = 0;
            chk->dLen = 0;
        }
        rxLen--;
        rxBuf++;
    }
    return;
}

