
#include "stm32f10x.h"
#include "includes.h"
#include "stm32f10x_can.h"

u8 GPS_RX_Buffer[68];
u8 GPS_RX_Count = 0;
void tmp(void);    
/**
  * @brief  main
  * @param  None
  * @retval int
  */
int main(void)
{
//  ErrorStatus err; 
    u16 i;
	
    delay_init(72);  //初始化滴答时钟，用于精确延时
    M35_GPIOInit();  //手机模块控制引脚配置
    NVIC_Config();
    RTC_Init();
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
        ChanSendLinkFrame(CHAN_TYPE_GPRS, LINK_LOGIN, tmp);
        CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
        while(1)
        {
            if (M35REBUFStructure.ReStart == SET) break;  //与服务器意外断开，重启模块；
	    
            CheckReportState();//检查各模块当前是否上报

            if(1 == Flag_CanReport)//mode0收到更新的Can数据上报;mode1周期到了上报
            {
                CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);		//关CAN中断
            
                Flag_CanReport = 0;
                CanReport();
   	
                CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		//开CAN中断
            }	
            if(1 == Flag_HeartReport)//发心跳包
            {
                Flag_HeartReport = 0;
                //HeartReport();
            }
		

            if(1 == Flag_GpsReport)  //收到"GPRMC"这一帧数据,且周期到了，上报GPS数据
            {
                Flag_GpsReport = 0;
                GpsReport();
            }
            //delay_s(60);
		

        }


    }
}

void tmp(void)
{
    
}
