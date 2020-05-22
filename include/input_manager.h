#ifndef _INPUT_MANAGER_H
#define _INPUT_MANAGER_H
#include <sys/time.h>
#include <pthread.h>
#define INPUT_TYPE_STDIN        0
#define INPUT_TYPE_TOUCHSCREEN  1

#define INPUT_VALUE_UP          0   
#define INPUT_VALUE_DOWN        1
#define INPUT_VALUE_EXIT        2
#define INPUT_VALUE_UNKNOWN     -1

typedef struct InputEvent{
	int iType;
	int iVal;
	struct timeval tTime;
}T_InputEvent, PT_InputEvent;

typedef struct InputOpr{
	char *pcName;
	int iFd;
	pthread_t tTreadID;
	int (*InputInit)(void);
	int (*InputExit)(void);
	int (*InputGet)(PT_InputEvent ptInputEvent);
	struct InputOpr *ptNext;
}T_InputOpr, PT_InputOpr;


int InputDevInit(void);
int InputGetVal(PT_InputEvent ptInputEvent);
int InputInit(void);
int ShowInputOpr(void);
int InputRegister(PT_InputOpr ptInputOpr);
int RegisterTS(void);
#endif /* _INPUT_MANAGER_H */