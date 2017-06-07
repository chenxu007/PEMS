#ifndef __WLM_H__
#define __WLM_H__

#include "stm32f10x.h"
#include <AppBuf.h>

typedef struct
{
    u8 transmitDelay;       //��λΪ��
    u8 retryTimes;          //�ط�����
    u8 heartBeatInterval;   //�������ڣ���λ����
    
    u8 mIP[4];              //��IP
    u16 mPort;              //���˿�
    u8 bIP[4];              //��IP
    u16 bPort;              //���˿�
    u8 apn[16];             //apn
}WlmParam;

typedef struct
{
    bool   moduleisinsert;   /**< ģ���Ƿ����*/
    bool   flagoflogined;    /**< �Ƿ��¼����վ */
    bool   flagofconnect;    /**< �Ƿ�������*/
//    oper_type_e  oper_type;  /**< ��Ӫ������*/
//    modulever_t  mver;       /**< ģ��汾��Ϣ*/
    s32  ncsq;             /**< �ź�ǿ��*/
    u32 flow;             /**< �ܵ�����,�ṩ��ͳ�ƽ���*/
    u8  imsicode[16];     /**< IMSI��*/
//    wlm_states_e  nlinkstate;       /**< ����״̬*/
//    time_t nlasttime;        /**< ���һ�ν��յ����ݵ�ʱ��*/
//   ip_addr_t lastconnip;    /**< ���һ�����ӳɹ�����վIP,������IP���Ǳ�IP */
//    uint16_t lastconnport;   /**< ���һ�����ӳɹ�����վ�˿�*/
}WlmInfo;

extern void WlmInit(void);
extern void WlmSend(AppBuf *appBuf);
#endif

