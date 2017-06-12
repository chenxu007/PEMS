
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
	
    delay_init(72);  //��ʼ���δ�ʱ�ӣ����ھ�ȷ��ʱ
    M35_GPIOInit();  //�ֻ�ģ�������������
    NVIC_Config();
    RTC_Init();
    OBD2_CANInit();  //��ʼ��CAN
    M35_USARTx_Config(115200);
	GPS_USARTx_Config(9600);
	
//  CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);		//��CAN�ж�
    while(1)
    {
        LED_init();
		ChanInit();
        M35_Start();                                  //����M35ģ��
        if (M35_TCPConnect() == ERROR)continue;	    //�����������TCP����
//      LED4_ONOFF(1);
        M35REBUFStructure.ReStart = RESET;
        ChanSendLinkFrame(CHAN_TYPE_GPRS, LINK_LOGIN, tmp);
        CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
        while(1)
        {
            if (M35REBUFStructure.ReStart == SET) break;  //�����������Ͽ�������ģ�飻
	    
            CheckReportState();//����ģ�鵱ǰ�Ƿ��ϱ�

            if(1 == Flag_CanReport)//mode0�յ����µ�Can�����ϱ�;mode1���ڵ����ϱ�
            {
                CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);		//��CAN�ж�
            
                Flag_CanReport = 0;
                CanReport();
   	
                CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		//��CAN�ж�
            }	
            if(1 == Flag_HeartReport)//��������
            {
                Flag_HeartReport = 0;
                //HeartReport();
            }
		

            if(1 == Flag_GpsReport)  //�յ�"GPRMC"��һ֡����,�����ڵ��ˣ��ϱ�GPS����
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
