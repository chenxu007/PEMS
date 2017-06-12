
#ifndef __INCLUDES_H
#define __INCLUDES_H

#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "bsp.h"
#include "m35.h"
#include "delay.h"
#include "ios157654.h"
#include "param.h"
#include  <stdio.h>
#include  <string.h>
#include "api.h"
#include "UTM.h"
#include "Chan.h"

// typedef struct
// {
//   __IO char  *ID;
//   __IO char  *Format;
//   __IO float *DS;
//   __IO char  *Unit;
// }DSOutPutFormatTypeDef;


extern __IO char TCP_Package[255];
extern void tmp(void); 
#endif
