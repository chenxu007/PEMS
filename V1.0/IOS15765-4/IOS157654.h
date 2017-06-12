
#ifndef __IOS157654_H
#define __IOS157654_H

#define MAX_USER_CANID 68
#define MIN_USER_CANID 10
#define TOTAL_USER_CANID 100

typedef struct
{
    u8 Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */
    u8 UpdateFlag;	/*0没有更新 1有更新*/					

}CanDSstru;

extern u8 Flag_CanUpdate;
extern CanDSstru CAN_DS[];
extern CanRxMsg RxMessage;


void OBD2_CANInit(void);
extern void CAN1_DS_UPDATE(CanRxMsg* RxMessage);
ErrorStatus SendFrame(CanTxMsg* TxMessage);

#endif


