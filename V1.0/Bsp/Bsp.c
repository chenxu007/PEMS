

#include "includes.h"


/**
  * @brief  printf函数配置。
  * 
  * 
  */
#if 1
#pragma import(__use_no_semihosting)             
                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;         
_sys_exit(int x) 
{ 
	x = x; 
} 

int fputc(int ch, FILE *f)
{   
 
  while((M35_USART->SR&0x40) == 0);
  M35_USART->DR = (u8)ch;      
  return ch;
}

_ttywrch(int ch)
{
ch = ch;
}

#endif

/**
  * @brief  配置M35通信模块所用控制IO.
  * @param  None
  * @retval None
  */
void M35_GPIOInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APBxPeriph_M35_PWRKEY | RCC_APBxPeriph_M35_STATUS,ENABLE);

  GPIO_InitStructure.GPIO_Pin = M35_PWRKEY_PIN;       
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(M35_PWRKEY_IO, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = M35_STATUS_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(M35_STATUS_IO, &GPIO_InitStructure);

}
/**
  * @brief  M35涉及串口配置.
  * @param  Baud:串口波特率
  * @retval None
  */
void M35_USARTx_Config(u32 Baud)
{
  u8 data;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APBxPeriph_M35_USART_IO | RCC_APBxPeriph_M35_USART,ENABLE);
  				 
  GPIO_InitStructure.GPIO_Pin = M35_USART_TXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M35_USART_IO,&GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = M35_USART_RXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(M35_USART_IO,&GPIO_InitStructure);


  USART_InitStructure.USART_BaudRate = Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(M35_USART,&USART_InitStructure);
  data = data;
  data = M35_USART->DR;
  data = M35_USART->SR;
  USART_ITConfig(M35_USART,USART_IT_RXNE,ENABLE);
  USART_Cmd(M35_USART,ENABLE);
}



/**
  * @brief  GPS涉及串口配置.
  * @param  Baud:串口波特率
  * @retval None
  */
void GPS_USARTx_Config(u32 Baud)
{
  u8 data;
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APBxPeriph_GPS_USART_IO,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APBxPeriph_GPS_USART,ENABLE);
  				 
  GPIO_InitStructure.GPIO_Pin = GPS_USART_TXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPS_USART_IO,&GPIO_InitStructure);
    
  GPIO_InitStructure.GPIO_Pin = GPS_USART_RXD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPS_USART_IO,&GPIO_InitStructure);


  USART_InitStructure.USART_BaudRate = Baud;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None ;
  USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
  USART_Init(GPS_USART,&USART_InitStructure);
  data = data;
  data = GPS_USART->DR;
  data = GPS_USART->SR;
  USART_ITConfig(GPS_USART,USART_IT_RXNE,ENABLE);
  USART_Cmd(GPS_USART,ENABLE);
}




/**
  * @brief  中断优先级设置.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);   
    NVIC_InitStructure.NVIC_IRQChannel = M35_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    NVIC_InitStructure.NVIC_IRQChannel = GPS_USART_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}




/**
  * @brief  LED引脚初始化.
  * @param  None
  * @retval None
  */
void LED_init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APBxPeriph_LED_IO,ENABLE);

  GPIO_InitStructure.GPIO_Pin = LED_D5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LED_IO,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_D6;
  GPIO_Init(LED_IO,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_D7;
  GPIO_Init(LED_IO,&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = LED_D8;
  GPIO_Init(LED_IO,&GPIO_InitStructure);

  LED_WriteBit(LED_IO, LED_D5 | LED_D6 | LED_D7 | LED_D8, Bit_SET);
}
/**
  * @brief  4个LED闪烁.
  * @param  time: 闪烁次数
  * @retval None
  */
void LED4_ONOFF(u8 time)
{
  u8 i;
  for (i = 0;i < time; i++)
  { 
    LED_WriteBit(LED_IO, LED_D5 | LED_D6 | LED_D7 | LED_D8, Bit_RESET);
    delay_ms(500);
    LED_WriteBit(LED_IO, LED_D5 | LED_D6 | LED_D7 | LED_D8, Bit_SET);
    delay_ms(500);
  }
}
void ClearRAM(u8* ram,u32 n)
{
  u32 i;
  for (i = 0;i < n;i++)
  {
    ram[i] = 0x00;
  }
}
/**
  * @brief  初始化CAN通信引脚
  * @param  None
  * @retval None
  */
void CAN1_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_Remap1_CAN1,ENABLE);
}

/**
  * @brief  配置CAN速率
  * @param  velocity 分频值
  * @retval None
  */
void CAN_Cofig(u8 velocity)
{
  CAN_InitTypeDef  CAN_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);

  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
    
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_11tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_6tq;
  CAN_InitStructure.CAN_Prescaler = velocity;
  CAN_Init(CAN1, &CAN_InitStructure);
}
/**
  * @brief  配置扩展数据滤波器
  * @param  id1:效验位1, id2:效验位2, mid1:屏蔽位1, mid2:屏蔽位2
  * @retval None
  */
void CAN1_Config32BitFilter(u32 id1, u32 id2, u32 mid1, u32 mid2)
{
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CAN_FilterInitStructure.CAN_FilterNumber = 0;							
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;  
  CAN_FilterInitStructure.CAN_FilterIdHigh = id1>>13;
  CAN_FilterInitStructure.CAN_FilterIdLow = (id1<<3)|4;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = mid1>>13;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (mid1<<3)|4;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
  CAN_FilterInitStructure.CAN_FilterIdHigh = id2>>13;
  CAN_FilterInitStructure.CAN_FilterIdLow = (id2<<3)|4;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = mid2>>13;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = (mid2<<3)|4;
  CAN_FilterInit(&CAN_FilterInitStructure);
  CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}
/**
  * @brief  配置标准数据滤波器
  * @param  id1:效验位1, id2:效验位2, mid1:屏蔽位1, mid2:屏蔽位2
  * @retval None
  */
void CAN1_Config16BitFilter(int FMInum,u16 id1, u16 id2, u16 mid1, u16 mid2)   //                                                                        
{                                                                                                                         
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;                                                                       
                                                                                                                          
    CAN_FilterInitStructure.CAN_FilterNumber=FMInum;                                                                           
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;                                                         
    CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_16bit;                                                        
    CAN_FilterInitStructure.CAN_FilterIdHigh=id1<<5;                                                                      
    CAN_FilterInitStructure.CAN_FilterIdLow=id2<<5;                                                                       
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=mid1;//mid1<<5;                                                                  
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=mid2;//mid2<<5;                                                                   
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;                                                           
    CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;                                                                  
                                                                                                                          
    CAN_FilterInit(&CAN_FilterInitStructure);

//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);  // gang                                                                          
}






