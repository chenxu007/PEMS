

#include "includes.h"


/**
  * @brief  启动或重启M35模块.
  * @param  None
  * @retval None
  */
void M35_Start(void)
{
  if(GPIO_ReadInputDataBit(M35_STATUS_IO,M35_STATUS_PIN) == 1)
  {
    do
    {
      GPIO_SetBits(M35_PWRKEY_IO,M35_PWRKEY_PIN);					  //关机
	 delay_ms(800);
	  GPIO_ResetBits(M35_PWRKEY_IO,M35_PWRKEY_PIN);
	  delay_s(1);//delay_s(3);
    }while(GPIO_ReadInputDataBit(M35_STATUS_IO,M35_STATUS_PIN) == 1); 
  }
  do
  {
    GPIO_SetBits(M35_PWRKEY_IO,M35_PWRKEY_PIN);						  //开机
	delay_s(1);
	delay_ms(100);//delay_ms(200);
    GPIO_ResetBits(M35_PWRKEY_IO,M35_PWRKEY_PIN);

  }while(GPIO_ReadInputDataBit(M35_STATUS_IO,M35_STATUS_PIN) == 0);
  delay_s(2);//  delay_s(5);
  
}
/**
  * @brief  M35模块与TCP建立链接.
  * @param  None
  * @retval ErrorStatus
  */
ErrorStatus M35_TCPConnect(void)
{
    u8 data;
    u8 i;
// //     u8 TcpCmd[50];
// //     Param_F2 IPparam;
// //     
// //     for(i=0;i<50;i++)
// //     {
// //         TcpCmd[i] = 0;
// //     }
    
    SIM_SendCmd("AT+IPR=115200\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    SIM_SendCmd("ATE0&W\r\n","\r\nOK","NULL","NULL",2000,1);
    SIM_SendCmd("NULL","\r\nCall Ready","NULL","NULL",5000,8);
    delay_ms(50);//delay_ms(100);
    SIM_SendCmd("AT+CFUN=1\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    if (SIM_SendCmd("AT+CPIN?\r\n","\r\n+CPIN: READY\r\n\r\nOK","NULL","NULL",2000,1) != 1) return ERROR;
    LED_WriteBit(LED_IO, LED_D8, Bit_RESET);									      //SIM卡就绪 点亮D8
    delay_ms(50);//delay_ms(100);
    data = 0;
    while (SIM_SendCmd("AT+CREG?\r\n","\r\n+CREG: 0,1","NULL","NULL",2000,1) != 1 && data < 10) //确认找网成功
    {
        delay_ms(50);// delay_ms(100);
        data++;																		
    }
    if (data == 10)
    { 
        return ERROR;                                                                             //找网不成功模块重启
    }
    else
    {
        LED_WriteBit(LED_IO, LED_D7 | LED_D8, Bit_RESET);						  //确认找网成功点亮D7
    }
    data = 0;
    while (SIM_SendCmd("AT+CGATT?\r\n","\r\n+CGATT: 1","NULL","NULL",2000,1) != 1 && data < 10) //确认GPRS附着成功
    {
        delay_ms(50);//    delay_ms(100);
        data++;
    }
    if (data == 10)
    {
        return ERROR;                                                                            //GPRS附着不成功模块重启
    }
    else
    {
        LED_WriteBit(LED_IO, LED_D6 | LED_D7 | LED_D8, Bit_RESET);		//确认GPRS附着成功,点亮D6
    }
    SIM_SendCmd("AT+QIFGCNT=0\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    data = SIM_SendCmd("AT+COPS?\r\n","\r\n+COPS: 0,0,\"CHINA MOBILE\"","\r\n+COPS: 0,0,\"CHINA UNICOM GSM\"","NULL",2000,1); //判断运营商
    delay_ms(50);//delay_ms(100);
    if (data == 1)	  //中国移动
    {
        SIM_SendCmd("AT+QICSGP=1,\"CMNET\"\r\n","\r\nOK","NULL","NULL",2000,1);
        delay_ms(50);//delay_ms(100);
    }
    else if (data == 2)  //中国联通
    {
        SIM_SendCmd("AT+QICSGP=1,\"UNINET\"\r\n","\r\nOK","NULL","NULL",2000,1);
        delay_ms(30);//delay_ms(100);
    }
    SIM_SendCmd("AT+QINDI=0\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    SIM_SendCmd("AT+QIHEAD=0\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    SIM_SendCmd("AT+QISHOWRA=0\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//elay_ms(100);
    SIM_SendCmd("AT+QISHOWPT=0\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    SIM_SendCmd("AT+QIMODE=0\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    SIM_SendCmd("AT+QITCFG=3,2,512,1\r\n","\r\nOK","NULL","NULL",2000,1);
    delay_ms(50);//delay_ms(100);
    //SIM_SendCmd("AT+QIOPEN=\"TCP\",\"122.114.240.145\",\"8383\"\r\n","\r\nOK","NULL","NULL",5000,1);
    
    
    //先连接主ip，失败后连接备用ip，再不行就返回error
// //     ParamRequest(2, (void *)&IPparam, PARAM_OPT_READ);
    
// //     strcpy(TcpCmd, "AT+QIOPEN=\"TCP\",\""); 
// //     IPAdressAddToAsciiString(TcpCmd, IPparam.mIP);
// //     strcat(TcpCmd,"\",\"");
// //     U16AddToAsciiString(TcpCmd, IPparam.mPort);
// //     strcat(TcpCmd,"\"\r\n");
// //     SIM_SendCmd(TcpCmd,"\r\nOK","NULL","NULL",5000,1);   
    SIM_SendCmd("AT+QIOPEN=\"TCP\",\"211.149.217.237\",\"8383\"\r\n","\r\nOK","NULL","NULL",5000,1);  
    //SIM_SendCmd("AT+QIOPEN=\"TCP\",\"122.114.240.145\",\"8383\"\r\n","\r\nOK","NULL","NULL",5000,1);     
    data = SIM_SendCmd("NULL","\r\nCONNECT OK","NULL","NULL",2000,1);
    if (data == 1)
    { 
        LED_WriteBit(LED_IO, LED_D5 | LED_D6 | LED_D7 | LED_D8, Bit_SET);//Bit_RESET);
        return SUCCESS;
    }
// //     else//连接备用tcp服务器
// //     {
// //         strcpy(TcpCmd, "AT+QIOPEN=\"TCP\",\"");
// //         IPAdressAddToAsciiString(TcpCmd, IPparam.bIP);
// //         strcat(TcpCmd,"\",\"");
// //         U16AddToAsciiString(TcpCmd, IPparam.bPort);
// //         strcat(TcpCmd,"\"\r\n");
// //         SIM_SendCmd(TcpCmd,"\r\nOK","NULL","NULL",5000,1);   
// //         //SIM_SendCmd("AT+QIOPEN=\"TCP\",\"122.114.240.145\",\"8383\"\r\n","\r\nOK","NULL","NULL",5000,1));      
// //         data = SIM_SendCmd("NULL","\r\nCONNECT OK","NULL","NULL",2000,1);
// //         if (data == 1)
// //         { 
// //             LED_WriteBit(LED_IO, LED_D5 | LED_D6 | LED_D7 | LED_D8, Bit_SET);//Bit_RESET);
// //             return SUCCESS;
// //         }
// //         else
// //         {
            return ERROR;
// //         }
// //     }
}
/**
  * @brief  通过串口往M35发送AT指令或者数据.
  * @param  *Cmd:要发送的内容 *Re1:M35可能回复
  * @retval None
  */
u8 SIM_SendCmd(char *Cmd,char *Re1,char *Re2,char *Re3,u32 TimeOut,u8 time)
{
  u8 i;
  u32 j;

  M35REBUFStructure.flay = RESET;
  M35REBUFStructure.cnt	= 0;

  if (strcmp(Cmd,"NULL"))
  {
    printf(Cmd);
  }

  for (i = 0;i < time;i++)
  {   
    for (j = 0;j < TimeOut;j++)
	{
	  delay_ms(1);
	  if (M35REBUFStructure.flay == SET)
	  {
	    if (!strncmp((char*)(M35REBUFStructure.M35Rebuf),Re1,strlen(Re1)))
	    {
	      return 1;
	    }
	    else if (!strncmp((char*)(M35REBUFStructure.M35Rebuf),Re2,strlen(Re2)))
	    {
	      return 2;
	    }
	    else if (!strncmp((char*)(M35REBUFStructure.M35Rebuf),Re3,strlen(Re3)))
	    {
	      return 3;
	    }
	  }
	}
  }         
  return 0;
}

u8 SIM_SendBuf(u8* buf, u32 len, char *Re1,char *Re2,char *Re3,u32 TimeOut,u8 time)
{
  u8 i;
  u32 j;

  M35REBUFStructure.flay = RESET;
  M35REBUFStructure.cnt	= 0;

  i = 0;
  while(i < len)
  {
    fputc(buf[i], NULL);
    i++;
  }

  for (i = 0;i < time;i++)
  {   
    for (j = 0;j < TimeOut;j++)
	{
	  delay_ms(1);
	  if (M35REBUFStructure.flay == SET)
	  {
	    if (!strncmp((char*)(M35REBUFStructure.M35Rebuf),Re1,strlen(Re1)))
	    {
	      return 1;
	    }
	    else if (!strncmp((char*)(M35REBUFStructure.M35Rebuf),Re2,strlen(Re2)))
	    {
	      return 2;
	    }
	    else if (!strncmp((char*)(M35REBUFStructure.M35Rebuf),Re3,strlen(Re3)))
	    {
	      return 3;
	    }
	  }
	}
  }         
  return 0;
}

/*把ip地址添加到指定字符串中*/
void IPAdressAddToAsciiString(u8 *dst, u8 num[4])
{
    u8 s[] = "0";
    u8 i;
    
    for(i=0;i<4;i++)
    {
        U8AddToAsciiString(dst, num[i]);
        
        if(i<3)
        {
            s[0] = ',';
            strcat(dst,s);
        }
    } 
}


