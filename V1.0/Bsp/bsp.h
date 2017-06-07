
#ifndef __BSP_H
#define __BSP_H
/**********CAN速率定义***********/
#define CAN500K 4
#define CAN125K 16
/********启动控制引脚定义********/
#define RCC_APBxPeriph_M35_PWRKEY  RCC_APB2Periph_GPIOB
#define RCC_APBxPeriph_M35_STATUS  RCC_APB2Periph_GPIOB

#define M35_PWRKEY_IO     GPIOB
#define M35_STATUS_IO     GPIOB

#define M35_PWRKEY_PIN    GPIO_Pin_10
#define M35_STATUS_PIN    GPIO_Pin_11

/********串口控制引脚定义********/
#define RCC_APBxPeriph_M35_USART_IO  RCC_APB2Periph_GPIOA
#define RCC_APBxPeriph_M35_USART	 RCC_APB2Periph_USART1

#define M35_USART_IO     GPIOA
#define M35_USART_TXD    GPIO_Pin_9
#define M35_USART_RXD	 GPIO_Pin_10
#define M35_USART        USART1
#define M35_USART_IRQn	 USART1_IRQn

#define RCC_APBxPeriph_GPS_USART_IO  RCC_APB2Periph_GPIOA
#define RCC_APBxPeriph_GPS_USART	 RCC_APB1Periph_USART2
#define GPS_USART_IO     GPIOA
#define GPS_USART_TXD    GPIO_Pin_2
#define GPS_USART_RXD	 GPIO_Pin_3
#define GPS_USART        USART2
#define GPS_USART_IRQn	 USART2_IRQn
/**********LED引脚定义***********/
#define RCC_APBxPeriph_LED_IO   RCC_APB2Periph_GPIOA

#define LED_IO      GPIOA
#define LED_D5      GPIO_Pin_3
#define LED_D6      GPIO_Pin_4						
#define LED_D7      GPIO_Pin_5
#define LED_D8      GPIO_Pin_6

#define LED_WriteBit      GPIO_WriteBit

/*********BSP.C函数定义**********/
void M35_GPIOInit(void);
void M35_USARTx_Config(u32 Baud);
void GPS_USARTx_Config(u32 Baud);
void NVIC_Config(void);
void LED_init(void);
void LED4_ONOFF(u8 time);
void ClearRAM(u8* ram,u32 n);
void CAN1_GPIOConfig(void);
void CAN_Cofig(u8 velocity);
void CAN1_Config32BitFilter(u32 id1, u32 id2, u32 mid1, u32 mid2);
void CAN1_Config16BitFilter(int FMInum,u16 id1, u16 id2, u16 mid1, u16 mid2);
#endif

