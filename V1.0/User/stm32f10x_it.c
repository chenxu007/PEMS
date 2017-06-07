/**
  ******************************************************************************
  * @file    EXTI/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "includes.h"
/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */ 
 /*������ID�Ƚϱ��Բ�ͬ��ID�����Ӧ�Ľ��յ�Ԫ�� */	
/*************************************************/

//  uint32_t CompareID_DS[6][2]= 
//  {  
//  	                     	    	{0x00A,0x000},				  /*  1 DS0001 ������ת�� DS0003 ��������ѹ DS0008 ȼ����λ  DS0009 ȼ���ͺ���   00A    */
// 					                {0x00B,0x001},				  /*  2 DS0004 ������Сʱ�� DS0006 ������ѹ�� DS0011 ����ѹ��   000B                    */
// 					                {0x00C,0x002},				  /*  3 DS0005 ����    DS0007 ��ص�ѹ           000C                                   */
//   				                    {0x00E,0x003},				  /*  4 DS0002 ������ˮ��  DS0010 ����ѹ��  000E                                        */
// 					                {0x00F,0x004},				  /*  5 DS0012 ��������         00F                                                     */
// 					                {0x011,0x005},				  /*  6 DS0013 ������           011                                                     */
//                      }; 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/
extern CanRxMsg RxMessage;
extern TimeTypeDef MainTime;
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}



/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
M35RbufTypeDef M35REBUFStructure;
/**
  * @brief  This function handles USART2 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
  u8 data;
  data = data;
  if(USART_GetFlagStatus(M35_USART, USART_FLAG_ORE)!=RESET)
  {
    data = M35_USART->DR;
  }
  if(USART_GetFlagStatus(M35_USART,USART_FLAG_RXNE)!=RESET)
  {
    M35REBUFStructure.M35Rebuf[M35REBUFStructure.cnt++] = M35_USART->DR;
    USART_ITConfig(M35_USART,USART_IT_IDLE,ENABLE);
  }
  if(USART_GetFlagStatus(M35_USART,USART_FLAG_IDLE)!=RESET)
  {
    USART_ITConfig(M35_USART,USART_IT_IDLE,DISABLE);
	  M35REBUFStructure.flay = SET;
	  M35REBUFStructure.cnt = 0;
  }
}   

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  //CanRxMsg RxMessage;
//  u8 i;
  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
  CAN1_DS_UPDATE(&RxMessage);                                                                                    
}

 
extern u8 Flag_GPS_OK;
extern u8 GPS_RX_Buffer[];
extern u8 GPS_RX_Count;

void USART2_IRQHandler(void)
{
	u8 data;
	
	data = data;
	  if(USART_GetFlagStatus(GPS_USART, USART_FLAG_ORE)!=RESET)
  {
    data = GPS_USART->DR;
		USART_SendData(GPS_USART, USART_ReceiveData(GPS_USART));
  }
  if(USART_GetFlagStatus(GPS_USART,USART_FLAG_RXNE)!=RESET)
  {
    //M35REBUFStructure.M35Rebuf[M35REBUFStructure.cnt++] = M35_USART->DR;
	  GPS_RX_Count = 0;
		Flag_GPS_OK = 0;
    USART_ITConfig(GPS_USART,USART_IT_IDLE,ENABLE);
  }
  if(USART_GetFlagStatus(GPS_USART,USART_FLAG_IDLE)!=RESET)
  {
    USART_ITConfig(GPS_USART,USART_IT_IDLE,DISABLE);
	//M35REBUFStructure.flay = SET;
	//M35REBUFStructure.cnt = 0;
		GPS_RX_Count = 0;
		Flag_GPS_OK = 0;
  }
	
	if(data == '$')
	{
		GPS_RX_Count = 0;
		Flag_GPS_OK = 0;		
	}

	GPS_RX_Buffer[GPS_RX_Count++] = data;

	if(GPS_RX_Count >= 59)
	{
		GPS_RX_Count = 59;
        if(GPS_RX_Buffer[4] == 'M' && GPS_RX_Buffer[52] == ',' && GPS_RX_Buffer[59] == ',')
        {
            Flag_GPS_OK = 1;
        }            
	}
}




void RTC_IRQHandler(void)
{
    if(MainTime.sec < 59)
    {
        MainTime.second++;
    }
    else if(MainTime.min < 59)
    {
        MainTime.second = 0;
        MainTime.min++
    }
    else if(MainTime.hour < 23)
    {
        MainTime.second = 0;
        MainTime.min = 0;
        MainTime.hour++;
    }
    else if(MainTime.day < 30)
    {
        MainTime.second = 0;
        MainTime.min = 0;
        MainTime.hour = 0;
        MainTime.day++;
    }
    else
    {
        MainTime.second = 0;
        MainTime.min = 0;
        MainTime.hour = 0;
        MainTime.day = 0;
    }
    Flag_1s = 1;
}

