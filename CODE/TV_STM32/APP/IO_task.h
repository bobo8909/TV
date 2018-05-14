#ifndef __IO_TASK_H
#define __IO_TASK_H

#include "sys.h"

#define OFF 0
#define ON 1

typedef enum 
{
	MANNED_DRIVING = 0, //manned
	AUTOMATIC_DRIVING , // 
}DRIVING_MODE;

extern DRIVING_MODE g_DrivingMode;


void IO_task(void);

#endif
