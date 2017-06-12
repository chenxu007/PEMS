#include "includes.h"

u8 Flag_CanReport = 0;	
u8 Flag_GpsReport = 0;
u8 Flag_HeartReport = 0;

static TimeTypeDef MainTime = {0,0,0,0};
TimeTypeDef FrontReportCanTime = {0,0,0,0};
TimeTypeDef FrontReportGpsTime = {0,0,0,0};
TimeTypeDef FrontReportHeartTime = {0,0,0,0};

void GetMainTime(void *time)
{
    memcpy(time, (void *)&MainTime, sizeof(TimeTypeDef));
}

void SetMainTime(void *time)
{
    memcpy((void *)&MainTime, time, sizeof(TimeTypeDef));
}

s32 HowLongTime(void *time1, void *time2)
{
    s32 secnum = 0;
    TimeTypeDef t1;
    TimeTypeDef t2;
    
    memcpy((void *)&t1, time1, sizeof(TimeTypeDef));
    memcpy((void *)&t2, time2, sizeof(TimeTypeDef));
    
    if(t1.day != t2.day)
    {
        if(t1.day < t2.day)
        {
            secnum += 60*60*24*(t2.day - t1.day);
        }
        else
        {
            secnum -= 60*60*24*(t1.day - t2.day);
        }
    }
    
    if(t1.hour != t2.hour)
    {
        if(t1.hour < t2.hour)
        {
            secnum += 60*60*(t2.hour - t1.hour);
        }
        else
        {
            secnum -= 60*60*(t1.hour - t2.hour);
        }
    }
    
    if(t1.min != t2.min)
    {
        if(t1.min < t2.min)
        {
            secnum += 60*(t2.min - t1.min);
        }
        else
        {
            secnum -= 60*(t1.min - t2.min);
        }
    }
    
    if(t1.sec != t2.sec)
    {
        if(t1.sec < t2.sec)
        {
            secnum += t2.sec - t1.sec;
        }
        else
        {
            secnum -= t1.sec - t2.sec;
        }
    }

    
}


void MainTimeGo(void)
{
    TimeTypeDef time;
    
    GetMainTime((void *)&time);
    if(time.sec < 59)
    {
        time.sec++;
    }
    else if(time.min < 59)
    {
        time.sec = 0;
        time.min++;
    }
    else if(time.hour < 23)
    {
        time.sec= 0;
        time.min = 0;
        time.hour++;
    }
    else if(time.day < 30)
    {
        time.sec= 0;
        time.min = 0;
        time.hour = 0;
        time.day++;
    }
    else
    {
        time.sec= 0;
        time.min = 0;
        time.hour = 0;
        time.day = 0;
    }
    SetMainTime((void *)&time);
}

void U16AddToAsciiString(u8 *dst, u16 num)
{
    u8 s[] = "0";
    
    if(0!=num/10000)
    {
        s[0] = '0' + num/10000;
        strcat(dst,s);
        s[0] = '0' + (num%10000)/1000;
        strcat(dst,s);
        s[0] = '0' + (num%1000)/100;
        strcat(dst,s);
        s[0] = '0' + (num%100)/10;
        strcat(dst,s);
        s[0] = '0' + num%10;
        strcat(dst,s);
    }
    else if(0!=num/1000)
    {
        s[0] = '0' + num/1000;
        strcat(dst,s);
        s[0] = '0' + (num%1000)/100;
        strcat(dst,s);
        s[0] = '0' + (num%100)/10;
        strcat(dst,s);
        s[0] = '0' + num%10;
        strcat(dst,s);        
    }
    else if(0!=num/100)
    {
        s[0] = '0' + num/100;
        strcat(dst,s);
        s[0] = '0' + (num%100)/10;
        strcat(dst,s);
        s[0] = '0' + num%10;
        strcat(dst,s);        
    }
    else if(0!=num/10)
    {
        s[0] = '0' + num/10;
        strcat(dst,s);
        s[0] = '0' + num%10;
        strcat(dst,s);        
    }
    else
    {
        s[0] = '0' + num;
        strcat(dst,s);         
    }
}


void U8AddToAsciiString(u8 *dst, u8 num)
{
    u8 s[] = "0";
    
    if(0!=num/100)
    {
        s[0] = '0' + num/100;
        strcat(dst,s);
        s[0] = '0' + (num%100)/10;
        strcat(dst,s);
        s[0] = '0' + num%10;
        strcat(dst,s);        
    }
    else if(0!=num/10)
    {
        s[0] = '0' + num/10;
        strcat(dst,s);
        s[0] = '0' + num%10;
        strcat(dst,s);        
    }
    else
    {
        s[0] = '0' + num;
        strcat(dst,s);         
    }
}


void CheckReportState(void)
{
    Param_F1 f1;
    Param_F4 f4;    
    Param_F5 f5;
    TimeTypeDef time;    
    
    ParamRequest(1, (void *)&f1, PARAM_OPT_READ);   
    ParamRequest(4, (void *)&f4, PARAM_OPT_READ);   
    ParamRequest(5, (void *)&f5, PARAM_OPT_READ);
    GetMainTime((void *)&time);    

    if(HowLongTime((void *)&FrontReportHeartTime, (void *)&time) > f1.heartBeatInterval/60)
    {
        Flag_HeartReport = 1;
    }
    
    if(HowLongTime((void *)&FrontReportGpsTime, (void *)&time) > f4.gpsReportInterval)
    {
        Flag_GpsReport = 1;
    }

    if(f5.canReportMode == CAN_REPORT_MODE_REAL)
    {
        if(1 == Flag_CanUpdate)
        {
            Flag_CanReport = 1;
            Flag_CanUpdate = 0;
        }
    }
    else if(HowLongTime((void *)&FrontReportGpsTime, (void *)&time) > f5.canReportInterval)
    {
        Flag_CanReport = 1;
    }        
}


void CanReport(void)
{
    Param_F5 f5;
    u16 i;
    AppBuf appBuf;
    PBuf * ppbuffer;

   
    ParamRequest(5, (void *)&f5, PARAM_OPT_READ);
    if(f5.canReportMode == CAN_REPORT_MODE_REAL)
    {
        for(i = MIN_USER_CANID; i <= MAX_USER_CANID; i++)
        {
            if(1 == CanDataSheet.CanData[i-MIN_USER_CANID].UpdateFlag)
            {
                CanActiveReport(i, (u8 *)CanDataSheet.CanData[i-MIN_USER_CANID].Data);
            }
        }
        CanDataSheet.UpdateNum = 0;
    }
    else
    {
        PBUF_ALLOC(ppbuffer);
    
        APPBUF_POAOUT_INIT((&appBuf), 0, ppbuffer,
            AFN_ACTIVEREPORT, PRM_REQU, FRAME_SINGLE, 0, NULL);

        /* can周期上报f2*/
        appBuf.pbuf->payLoad[appBuf.pbuf->len++] = 2;
        
        /* can周期上报条数*/
        appBuf.pbuf->payLoad[appBuf.pbuf->len++] = CanDataSheet.UpdateNum;
        
        for(i = MIN_USER_CANID; i <= MAX_USER_CANID; i++)
        {
            if(1 == CanDataSheet.CanData[i-MIN_USER_CANID].UpdateFlag)
            {
                /* can id*/
                appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)(i & 0x00FF);
                appBuf.pbuf->payLoad[appBuf.pbuf->len++] = (u8)((i >> 8) & 0x00FF);

                /* can data*/
                memcpy(&appBuf.pbuf->payLoad[appBuf.pbuf->len], (u8 *)CanDataSheet.CanData[i-MIN_USER_CANID].Data, 8);
                appBuf.pbuf->len += 8;                
            }
        }

        UTM_DO(&appBuf);

        PBUF_FREE(ppbuffer);
        appBuf.pbuf = NULL;        
    }
}
void HeartReport(void)
{
  ChanSendLinkFrame(CHAN_TYPE_GPRS, LINK_HB, tmp);  
}
void GpsReport(void)
{
    
}