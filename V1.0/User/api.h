#ifndef __API_H__
#define __API_H__

extern u8 Flag_CanReport;	
extern u8 Flag_GpsReport;
extern u8 Flag_HeartReport;

typedef struct
{
    u8    sec;
    u8    min;
    u8    hour;
    u8    day;
}TimeTypeDef;


void GetMainTime(void *time);
void SetMainTime(void *time);
void MainTimeGo(void);
void U16AddToAsciiString(u8 *dst, u16 num);
void U8AddToAsciiString(u8 *dst, u8 num);
void CheckReportState(void);
void CanReport(void);
void HeartReport(void);
void GpsReport(void);






















#endif