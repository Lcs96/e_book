#include <input_manager.h>

PT_InputOpr g_ptInputOprHead;
PT_InputEvent g_tInputEvent;

static pthread_mutex_t g_tMutex  = PTHREAD_MUTEX_INITIALIZER;

/* pthread_cond_t表示多线程的条件变量，用于控制线程等待和就绪的条件。
 * = PTHREAD_COND_INITIALIZER;即可完成初始化
 */
static pthread_cond_t  g_tConVar = PTHREAD_COND_INITIALIZER;

int InputRegister(PT_InputOpr ptInputOpr)
{
	PT_InputOpr tmp;
	if (g_ptInputOprHead == NULL)
		{
			g_ptInputOprHead = ptInputOpr;
			return 0;
		}
	else
		{
			tmp = g_ptInputOprHead;
			while(tmp->ptNext != NULL)
				{
					tmp = tmp->ptNext;
				}
			tmp->ptNext = ptInputOpr;
			ptInputOpr->ptNext = NULL;
		}
	return 0;
}


int ShowInputOpr(void)
{
	PT_InputOpr ptTmp;
	ptTmp = g_ptInputOprHead;
	while (ptTmp != NULL)
		{
			printf("%s\n",ptTmp->pcName);
			ptTmp = ptTmp->ptNext;
		}
	return 0;
}


static void *InputEventTreadFunction(void *pVoid)
{
	PT_InputEvent ptInputEvent;
	int (*InputGetEv)(PT_InputEvent ptInputEvent);
	InputGetEv = (int (*)(PT_InputEvent))pVoid;
	while (1)
		{
			if (0 == InputGetEv(ptInputEvent))
				{
					pthread_mutex_lock(&g_tMutex);
					g_tInputEvent = ptInputEvent;
					pthread_cond_signal(&g_tConVar);

					/* 释放互斥量 */
					pthread_mutex_unlock(&g_tMutex);
				}
		}
}

int InputInit(void)
{
	PT_InputOpr ptTmp;
	ptTmp = g_ptInputOprHead;

	int iError = -1;
	while (ptTmp != NULL)
		{
			if(ptTmp->InputInit() == 0)
				{
					pthread_create(&ptTmp->tTreadID, NULL, InputEventTreadFunction, ptTmp->GetInputEvent);			
					ptTmp = ptTmp->ptNext;
					iError = 0;
				}
		}

	return iError;
}


int InputGetVal(PT_InputEvent ptInputEvent)
{
	pthread_mutex_lock(&g_tMutex);
	pthread_cond_wait(&g_tConVar, &g_tMutex);	

	/* 被唤醒后,返回数据 */
	*ptInputEvent = g_tInputEvent;
	pthread_mutex_unlock(&g_tMutex);
	return 0;	
}

int InputDevInit(void)
{
	int iError = 0;
	iError = RegisterTS();
	return iError;
}
