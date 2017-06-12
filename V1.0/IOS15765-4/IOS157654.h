
#ifndef __IOS157654_H
#define __IOS157654_H

#define MAX_USER_CANID 68
#define MIN_USER_CANID 10
#define TOTAL_USER_CANID 60

typedef struct
{
    u8 Data[8]; /*!< Contains the data to be received. It ranges from 0 to 
                        0xFF. */
    u8 UpdateFlag;	/*0没有更新 1有更新*/					

}CanDataStru;

typedef struct
{
    u16 UpdateNum; /*更新的can数据条数*/
    CanDataStru CanData[TOTAL_USER_CANID];	/*[0]~[xx]依次存放id0x000a~0x00xx的can数据，只放data以及一个更新标志位，上传清0 */					

}CanDataSheetStru;

extern u8 Flag_CanUpdate;
extern CanDataSheetStru CanDataSheet;
extern CanRxMsg RxMessage;


void OBD2_CANInit(void);
extern void CAN1_DS_UPDATE(CanRxMsg* RxMessage);
ErrorStatus SendFrame(CanTxMsg* TxMessage);

#endif


