#include <input_manager.h>

PT_InputOpr g_ptInputOprHead;
static fd_set g_tRFds;
static int g_iMaxFd = -1;

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




int InputInit(void)
{
	PT_InputOpr ptTmp;
	ptTmp = g_ptInputOprHead;
	FD_ZERO(&g_tRFds);
	int iError = -1;
	while (ptTmp != NULL)
		{
			if(ptTmp->InputInit() == 0)
				{
					FD_SET(ptTmp->iFd, &g_tRFds);
					ptTmp = ptTmp->ptNext;
					if(g_iMaxFd < ptTmp->iFd)
						{
							g_iMaxFd = ptTmp->iFd;
						}
					iError = 0;
				}
		}
	g_iMaxFd++;
	return iError;
}


int InputGetVal(PT_InputEvent ptInputEvent)
{
	PT_InputOpr ptTmp;
	ptTmp = g_ptInputOprHead;
	int iNum;
	iNum = select(g_iMaxFd, g_tRFds, NULL, NULL, NULL);
	if (iNum != 0)
		{
				while (ptTmp != NULL)
			{
				if (FD_ISSET(ptTmp->iFd, &g_tRFds))
					{
						if (ptTmp->InputGet(ptInputEvent) == 0)
							{
								return 0;
							}
					}
				ptTmp = ptTmp->ptNext;
			}
		}
	return -1;
}

int InputDevInit(void)
{
	int iError = 0;
	iError = RegisterTS();
	return iError;
}
