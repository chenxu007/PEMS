#ifndef __CHAN_H__
#define __CHAN_H__

#include <AppBuf.h>

typedef enum
{
    CHAN_TYPE_GPRS = 0,
//    CHAN_TYPE_RS232,
//    CHAN_TYPE_SMS,
    CHAN_TYPE_SIZE
}ChanType;

typedef enum
{
    LINK_LOGIN = 1, /**< µÇÂ¼ */
    LINK_LOGOUT,    /**< µÇ³ö */
    LINK_HB         /**< ÐÄÌø */
}LinkType;

extern void ChanInit(void);
extern void ChanSend(AppBuf *appBuf);
extern void ChanSendLinkFrame(ChanType chType, LinkType linkType, void (*onConfirmFun)(void));



#endif

