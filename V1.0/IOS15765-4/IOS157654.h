
#ifndef __IOS157654_H
#define __IOS157654_H

typedef struct
{
    u8 Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */
    u8 UpdateFlag;	/*0没有更新 1有更新*/					

}CanDSstru;

extern u8 CanUpdateFlag;
extern CanDSstru CAN_DS[];


void OBD2_CANInit(void);
extern void CAN1_DS_UPDATE(CanRxMsg* RxMessage);
ErrorStatus SendFrame(CanTxMsg* TxMessage);

#endif


