
#include "stm32f10x.h"
#include "includes.h"
#include "stm32f10x_can.h"
#include <UTM.h>
u8 CanUpdateFlag = 0;	/*0没有更新 1有更新*/
u8 Flag_GPS_OK = 0;
u8 Flag_1s = 0;
u8 GPS_RX_Buffer[68];
u8 GPS_RX_Count = 0;

    
TimeTypeDef MainTime;
extern CanDSstru CAN_DS[100];
extern CanRxMsg RxMessage;
/**
  * @brief  main
  * @param  None
  * @retval int
  */
int main(void)
{
//  ErrorStatus err; 
    u16 i;
// 	char *TCP_send_cmd1 = "AT+QISEND=9\r\n";
// 	char *TCP_send_cmd2 = "AT+QISEND=18\r\n";
// 	char *TCP_send_cmd3 = "AT+QISEND=108\r\n";
// 	char *TCP_send_cmd;
	
    delay_init(72);  //初始化滴答时钟，用于精确延时
    M35_GPIOInit();  //手机模块控制引脚配置
    NVIC_Config();
    OBD2_CANInit();  //初始化CAN
    M35_USARTx_Config(115200);
	GPS_USARTx_Config(9600);
	
//  CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);		//关CAN中断
    while(1)
    {
        LED_init();
		ChanInit();
        M35_Start();                                  //启动M35模块
        if (M35_TCPConnect() == ERROR)continue;	    //与服务器建立TCP链接
//      LED4_ONOFF(1);
        M35REBUFStructure.ReStart = RESET;
        while(1)
        {
            if (M35REBUFStructure.ReStart == SET) break;  //与服务器意外断开，重启模块；
	    
            CheckCANCycle();//检查Can上报周期到了没有
            CheckGPSCycle();//检查GPS上报周期到了没有
            if((1 == Flag_CAN_Cycle)&&(1 == CanUpdateFlag))//收到更新的Can数据且周期到了上报
            {
                CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);		//关CAN中断
            
                for(i=MIN_USER_CANID;i<=MAX_USER_CANID;i++)
                {
                    CanActiveReport(id, (u8*)CAN_DS[id - MIN_USER_CANID].Data);
                    CAN_DS[id - MIN_USER_CANID].UpdateFlag = 0;
                }
                CanUpdateFlag = 0;
                delay_ms(300);
   	
                CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		//开CAN中断
            }	
            if(1 == Flag_1s)//发心跳包
            {
            
            }
		

            if(1 == Flag_GPS_Cycle)&&(1 == Flag_GPS_OK)			//收到"GPRMC"这一帧数据,且周期到了，上报GPS数据
            {
                
            }
            delay_ms(300);
		

        }


    }
}



