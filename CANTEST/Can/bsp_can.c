/*
This file contains all the external functions to get CAN working :>
*/

/* Include */
#include "stm32f4xx_hal.h"
#include "bsp_can.h"
/*{End} Include */

/* CAN */
extern CAN_HandleTypeDef hcan1;
uint32_t can_count=0;
/*{End} CAN */

/* CAN: RECEIVE */
CAN_RxHeaderTypeDef can1RxHeader;
uint8_t canRxMsg[8];
/*{End} CAN: RECEIVE */

/* CAN: TRANSMIT */
CAN_TxHeaderTypeDef can1TxHeader0;
CAN_TxHeaderTypeDef can1TxHeader1;
uint8_t canTxMsg0[8] = {0};
uint8_t canTxMsg1[8] = {0};
/*{End} CAN: TRANSMIT */

/* CAN: FILTER */
CAN_FilterTypeDef can1Filter;
/*{End} CAN: FILTER */

/* CAN: INITIALIZE */
// MX_CAN1_Init(void) -> cubemx should have built this in main.c . For reference, refer to appendix below.
// HAL_CAN_MspInit() -> cubemx should have built this in stm32f4xx_hal_msp.c . For reference, refer to appendix below.
void CAN_Init_All(void)
{
	can1TxHeader0.IDE = CAN_ID_STD;
	can1TxHeader0.StdId = 0x200;
	can1TxHeader0.DLC = 8;
	
	can1TxHeader1.IDE = CAN_ID_STD;
	can1TxHeader1.StdId = 0x1FF;
	can1TxHeader1.RTR = CAN_RTR_DATA;
	can1TxHeader1.DLC = 8;
	
	can1Filter.FilterActivation = ENABLE;
	can1Filter.FilterMode = CAN_FILTERMODE_IDMASK;
	can1Filter.FilterScale = CAN_FILTERSCALE_32BIT;
	can1Filter.FilterFIFOAssignment = CAN_FilterFIFO0;
	can1Filter.FilterIdHigh = 0x0000;
	can1Filter.FilterIdLow = 0x0000;
	can1Filter.FilterBank = 0;
	HAL_CAN_ConfigFilter(&hcan1,&can1Filter);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_FULL);
	HAL_CAN_Start(&hcan1);
}
/*{End} CAN: INITIALIZE */

/* CAN: USER */
void CAN_SendMsg(CAN_HandleTypeDef* hcan,CAN_TxHeaderTypeDef *canTxHeader,uint8_t* canMsg)
{
	HAL_CAN_AddTxMessage(hcan,canTxHeader,canMsg,(void*)CAN_TX_MAILBOX0);
}
void ME_Test_Chassis(int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq)
{
    canTxMsg0[0] = (uint8_t)(cm1_iq >> 8);
    canTxMsg0[1] = (uint8_t)cm1_iq;
    canTxMsg0[2] = (uint8_t)(cm2_iq >> 8);
    canTxMsg0[3] = (uint8_t)cm2_iq;
    canTxMsg0[4] = (uint8_t)(cm3_iq >> 8);
    canTxMsg0[5] = (uint8_t)cm3_iq;
    canTxMsg0[6] = (uint8_t)(cm4_iq >> 8);
    canTxMsg0[7] = (uint8_t)cm4_iq;
    CAN_SendMsg(&hcan1,&can1TxHeader0,canTxMsg0);
}
void LOOP_Forward_Chassis(void)
{
	ME_Test_Chassis(2000,2000,2000,2000);
}
/*{End} CAN: USER */

/* CAN: RECEIVE PROCESS */
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg)
{      
    can_count++;
		switch(rxHeader->StdId)
		{
				case CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID:
				case CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID:
				case CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID:
				case CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID:
				{
					LOOP_Forward_Chassis();
					break;
				}
		}
}
/*{End} CAN: RECEIVE PROCESS */




/* Start of appendix
void MX_CAN1_Init(void)
{

  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 3;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = ENABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* hcan)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(hcan->Instance==CAN1)
  {

    __HAL_RCC_CAN1_CLK_ENABLE();
  
    PD0     ------> CAN1_RX
    PD1     ------> CAN1_TX 
    
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

  }

}
End of appendix */
