#ifndef __UTM_H__
#define __UTM_H__

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include <AppBuf.h>


typedef int (*FUNCPTR) ();

#define PROTOCOL_V0         1
/* Frame PRM */
#define PRM_REQU            0x01    /**< ����վ����             */
#define PRM_RESP            0x00    /**< �Ӷ�վ����             */

/*Frame Type Sign*/
#define FRAME_MIDDLE        0x00    /**< ��֡���м�֡           */
#define FRAME_END           0x01    /**< ��֡������֡           */
#define FRAME_FIRST         0x02    /**< ��֡����1֡            */
#define FRAME_SINGLE        0x03    /**< ��֡                   */

/* AFN */
#define AFN_CONFIRM         0x00    /**< ȷ��/����              */
#define AFN_RESET           0x01    /**< ��λ                   */
#define AFN_LINKCHECK       0x02    /**< ��·�ӿڼ��           */
#define AFN_CTLCMD          0x03    /**< ��������               */
#define AFN_SETPARAM        0x04    /**< ���ò���               */
#define AFN_QUERYPARAM      0x05    /**< ��ѯ����               */
#define AFN_ACTIVEREPORT    0x06    /**< �����ϱ�               */
#define AFN_QUERYREALDATA   0x07    /**< ��ѯʵʱ����           */
#define AFN_FILETRANSMIT    0x0F    /**< �ļ�����               */
#define AFN_DATATRANSMIT    0x10    /**< ����ת��               */


#define UTM_BSN_DATA_OFF     16u


void UTM_DO (AppBuf * appBuf); 
void CanActiveReport(u16 canId, u8 *dataArry);


#ifdef __cplusplus
}
#endif 

#endif
