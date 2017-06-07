#ifndef __APPBUF_H__
#define __APPBUF_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include <PBuf.h>


#define APPBUF_RAW             0x01 /*Ϊ��Լ��ԭʼ���뱨��*/
#define APPBUF_POAIN           0x02 /*Ϊ��ԼӦ�ò����뱨��*/
#define APPBUF_POAOUT          0x03 /*Ϊ��ԼӦ�ò��������*/

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
     * ԭʼ���Ĵӵ�1���ֽڿ�ʼ,Ӧ�ò����ݱ�����ӵ�15���ֽڿ�ʼ,ΪDADT��
     * 1.����β���ĸ�����Ϣ��Ԫ��ɾ��
     * 2.frame->tot_len = 14 + �����򳤶ȣ�������aux����
     */
    PBuf* pbuf;
    /**
     * buf����
     * 1: ԭʼ����;
     * 2: ϵͳ�ڲ�ƴװ�ĵ������ԼӦ�ò㱨��
     * 3: ϵͳӦ����������͵������ԼӦ�ò㱨��
     */
    u8 bufTyp;
    u8 chTyp;                     /**< �ŵ����� */
    union
    {
        /**
         * ������Ϣ
         */
        struct
        {
            u8 afn;                 /**< �����ͱ��ĵ�Ӧ�ù����� */
            u8 prm;                 /**< ���������ͻ���Ӧ���� */
        } in;

        /**
         * �����Ϣ
         */
        struct
        {
            u8 afn;                 /**< �����ͱ��ĵ�Ӧ�ù����� */
            u8 prm;                 /**< ���������ͻ���Ӧ���� */
            u8 fts;                 /**< ֡���ͱ�ʶ:�������Ƕ�� */
            u8 con;                 /**< ȷ�ϱ�ʶ:0 - ����ȷ�ϣ� 1 - Ҫȷ�� */
            void (*callback)(void);    /**< ���ķ��ͺ���õĹ��Ӻ���*/
        } out;
    } poa;
} AppBuf;

#ifdef __cplusplus
}
#endif 

#endif
