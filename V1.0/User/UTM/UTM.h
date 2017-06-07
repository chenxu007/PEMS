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
#define PRM_REQU            0x01    /**< 启动站报文             */
#define PRM_RESP            0x00    /**< 从动站报文             */

/*Frame Type Sign*/
#define FRAME_MIDDLE        0x00    /**< 多帧：中间帧           */
#define FRAME_END           0x01    /**< 多帧：结束帧           */
#define FRAME_FIRST         0x02    /**< 多帧：第1帧            */
#define FRAME_SINGLE        0x03    /**< 单帧                   */

/* AFN */
#define AFN_CONFIRM         0x00    /**< 确认/否认              */
#define AFN_RESET           0x01    /**< 复位                   */
#define AFN_LINKCHECK       0x02    /**< 链路接口检测           */
#define AFN_CTLCMD          0x03    /**< 控制命令               */
#define AFN_SETPARAM        0x04    /**< 设置参数               */
#define AFN_QUERYPARAM      0x05    /**< 查询参数               */
#define AFN_ACTIVEREPORT    0x06    /**< 主动上报               */
#define AFN_QUERYREALDATA   0x07    /**< 查询实时数据           */
#define AFN_FILETRANSMIT    0x0F    /**< 文件传输               */
#define AFN_DATATRANSMIT    0x10    /**< 数据转发               */


#define UTM_BSN_DATA_OFF     16u


void UTM_DO (AppBuf * appBuf); 
void CanActiveReport(u16 canId, u8 *dataArry);


#ifdef __cplusplus
}
#endif 

#endif
