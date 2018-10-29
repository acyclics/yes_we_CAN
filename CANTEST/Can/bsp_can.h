#ifndef __bspcan
#define __bspcan

/* Start of Include files */
#include "stm32f4xx_hal.h"
/* End of Include files */

/* CAN */
extern CAN_HandleTypeDef hcan1;
extern uint8_t canRxMsg[8];
extern uint8_t canTxMsg0[8];
extern uint8_t canTxMsg1[8];
extern CAN_TxHeaderTypeDef can1TxHeader0;
extern CAN_TxHeaderTypeDef can1TxHeader1;
extern CAN_RxHeaderTypeDef can1RxHeader;
/*{End} CAN */

/* CAN: Motors */
#define CAN_BUS2_MOTOR1_FEEDBACK_MSG_ID           0x201
#define CAN_BUS2_MOTOR2_FEEDBACK_MSG_ID           0x202 
#define CAN_BUS2_MOTOR3_FEEDBACK_MSG_ID           0x203
#define CAN_BUS2_MOTOR4_FEEDBACK_MSG_ID           0x204
/*{End} CAN: Motors */

/* CAN: INITIALIZE FUNCTIONS */
void CAN_Init_All(void); 
/*{End} CAN: INITIALIZE FUNCTIONS */

/* CAN: USER FUNCTIONS */
void CAN_SendMsg(CAN_HandleTypeDef* hcan,CAN_TxHeaderTypeDef *canTxHeader,uint8_t* canMsg);
void ME_Test_Chassis(int16_t cm1_iq,int16_t cm2_iq,int16_t cm3_iq,int16_t cm4_iq);
/*{End} CAN: USER FUNCTIONS */

/* CAN: RECEIVE FUNCTIONS */
void CanReceiveMsgProcess(CAN_RxHeaderTypeDef *rxHeader,uint8_t* msg);
/*{End} CAN: RECEIVE FUNCTIONS */

#endif
