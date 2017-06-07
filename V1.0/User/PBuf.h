#ifndef __PBUF_H__
#define __PBUF_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include <assert.h>

#define PBUF_ALLOC(pbuf) \
    pbuf = PBufAlloc(); \
    assert(NULL != pbuf)
    
#define PBUF_FREE(pbuf) \
    PBufFree(pbuf); \
    pbuf = NULL

#define PBUF_BYTE_MAX   512     

typedef struct 
{
    u8 isUsed;
    
    /** pointer to the actual data in the buffer */
    u8 payLoad[PBUF_BYTE_MAX];

    u16 len;
}PBuf;

extern PBuf * PBufAlloc (void);
extern void PBufFree(PBuf * pbuf);

#ifdef __cplusplus
}
#endif 

#endif
