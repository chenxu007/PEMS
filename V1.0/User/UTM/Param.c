/* Param.c*/

/*
 * Copyright (c) 2017 Bin Hong Tech, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable BinHong-tech license agreement.
 */

/*
modification history
--------------------
01a,17jun01,cx_  add file
*/

#include <Param.h>
#include <assert.h>
#include <string.h>

static Param TerminalParam = 
{
    {30, 3, 3},
    {{192, 168, 1, 226}, 8686, {192, 168, 1, 227}, 8686, "cmnet"},
    {0, {0x12,0x34, 0x56, 0x78,0x09}},
    {60},
    {CAN_REPORT_MODE_REAL, 90}
};

void ParamRequest (u8 fn, void *data, ParamOption opt)
{
    void * src;
    void * dest = data;
    u32 size;
    
    switch(fn)
    {
        case 1:
            src = (void*)&TerminalParam.f1;
            size = sizeof(TerminalParam.f1);
            break;
        case 2:
            src = (void*)&TerminalParam.f2;
            size = sizeof(TerminalParam.f2);
            break;
        case 3:
            src = (void*)&TerminalParam.f3;
            size = sizeof(TerminalParam.f3);
            break;
        case 4:
            src = (void*)&TerminalParam.f4;
            size = sizeof(TerminalParam.f4);
            break;
        case 5:
            src = (void*)&TerminalParam.f5;
            size = sizeof(TerminalParam.f5);
            break;
        default:
            assert(0);
    }
    
    if(PARAM_OPT_READ == opt)
        memcpy(dest, src, size);
    else
        memcpy(src, dest, size);
};
