
#include "stm32f10x.h"
#include "includes.h"
#include "stm32f10x_can.h"
#include <UTM.h>
u8 CanUpdateFlag = 0;	/*0û�и��� 1�и���*/
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
	
    delay_init(72);  //��ʼ���δ�ʱ�ӣ����ھ�ȷ��ʱ
    M35_GPIOInit();  //�ֻ�ģ�������������
    NVIC_Config();
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
        while(1)
        {
            if (M35REBUFStructure.ReStart == SET) break;  //�����������Ͽ�������ģ�飻
	    
            CheckCANCycle();//���Can�ϱ����ڵ���û��
            CheckGPSCycle();//���GPS�ϱ����ڵ���û��
            if((1 == Flag_CAN_Cycle)&&(1 == CanUpdateFlag))//�յ����µ�Can���������ڵ����ϱ�
            {
                CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);		//��CAN�ж�
            
                for(i=MIN_USER_CANID;i<=MAX_USER_CANID;i++)
                {
                    CanActiveReport(id, (u8*)CAN_DS[id - MIN_USER_CANID].Data);
                    CAN_DS[id - MIN_USER_CANID].UpdateFlag = 0;
                }
                CanUpdateFlag = 0;
                delay_ms(300);
   	
                CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);		//��CAN�ж�
            }	
            if(1 == Flag_1s)//��������
            {
            
            }
		

            if(1 == Flag_GPS_Cycle)&&(1 == Flag_GPS_OK)			//�յ�"GPRMC"��һ֡����,�����ڵ��ˣ��ϱ�GPS����
            {
                
            }
            delay_ms(300);
		

        }


    }
}



