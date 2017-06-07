#ifndef __APPBUF_H__
#define __APPBUF_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include <PBuf.h>


#define APPBUF_RAW             0x01 /*为规约的原始输入报文*/
#define APPBUF_POAIN           0x02 /*为规约应用层输入报文*/
#define APPBUF_POAOUT          0x03 /*为规约应用层输出报文*/

#ifndef UTM_BSN_DATA_OFF
#define UTM_BSN_DATA_OFF     16u
#endif

#define APPBUF_RAW_INIT(appBuf, type, ppbuf)\
    do\
    {\
        if ((appBuf) != NULL)\
        {\
            (appBuf)->bufTyp = APPBUF_RAW;\
            (appBuf)->chTyp = (type);\
            (appBuf)->pbuf = ppbuf; \
            break;\
        }\
    }while(1)

#define APPBUF_POAIN_INIT(appBuf, type, ppbuf, Afn, Prm) \
    do \
    { \
        if ((appBuf) != NULL) \
        { \
            (appBuf)->bufTyp = APPBUF_POAIN; \
            (appBuf)->chTyp = (type); \
            (appBuf)->pbuf = ppbuf; \
            (appBuf)->poa.in.afn = (Afn); \
            (appBuf)->poa.in.prm = (Prm); \
            break; \
        } \
    } \
    while (1)

#define APPBUF_POAOUT_INIT(appBuf, type, ppbuf, Afn, Prm, Fts, Con, CallBack) \
    do \
    { \
        if ((appBuf) != NULL) \
        { \
            (appBuf)->bufTyp = APPBUF_POAOUT; \
            (appBuf)->chTyp = (type); \
            (appBuf)->pbuf = ppbuf; \
            (appBuf)->pbuf->len = UTM_BSN_DATA_OFF; \
            (appBuf)->poa.out.afn = (Afn); \
            (appBuf)->poa.out.prm = (Prm); \
            (appBuf)->poa.out.fts = (Fts); \
            (appBuf)->poa.out.con = (Con); \
            (appBuf)->poa.out.callback = (CallBack); \
            break; \
        } \
    } \
    while (1)

typedef struct
{
    /**
     * 原始报文从第1个字节开始,应用层数据报文则从第15个字节开始,为DADT；
     * 1.报文尾部的附加信息单元已删除
     * 2.frame->tot_len = 14 + 数据域长度，不包含aux部分
     */
    PBuf* pbuf;
    /**
     * buf类型
     * 1: 原始报文;
     * 2: 系统内部拼装的的输入规约应用层报文
     * 3: 系统应答或主动发送的输出规约应用层报文
     */
    u8 bufTyp;
    u8 chTyp;                     /**< 信道类型 */
    union
    {
        /**
         * 输入信息
         */
        struct
        {
            u8 afn;                 /**< 待发送报文的应用功能码 */
            u8 prm;                 /**< 是主动发送还是应答报文 */
        } in;

        /**
         * 输出信息
         */
        struct
        {
            u8 afn;                 /**< 待发送报文的应用功能码 */
            u8 prm;                 /**< 是主动发送还是应答报文 */
            u8 fts;                 /**< 帧类型标识:单包还是多包 */
            u8 con;                 /**< 确认标识:0 - 无需确认， 1 - 要确认 */
            void (*callback)(void);    /**< 报文发送后调用的钩子函数*/
        } out;
    } poa;
} AppBuf;

#ifdef __cplusplus
}
#endif 

#endif
