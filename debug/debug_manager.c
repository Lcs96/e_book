#include <config.h>
#include <debug_manager.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>


static PT_DebugOpr g_ptDebugOprHead;
static int g_iDbgLevel = 8;

int DebugOprRegister(PT_DebugOpr ptDebugOpr)
{
	PT_DebugOpr ptTmp;
	if (g_ptDebugOprHead == NULL)
		{
			g_ptDebugOprHead = ptDebugOpr;
			ptDebugOpr->ptNext = NULL;
		}
	else 
		{
			ptTmp = g_ptDebugOprHead;
			while(ptTmp->ptNext != NULL)
				{
					ptTmp = ptTmp->ptNext;
				}
			ptTmp->ptNext = ptDebugOpr;
			ptDebugOpr->ptNext = NULL;
		}
	return 0;
}


void ShowDebugOpr()
{
	PT_DebugOpr ptTmp;
	ptTmp = g_ptDebugOprHead;
	
	while (ptTmp != NULL)
		{
			printf("%s\n", ptTmp->name);
			ptTmp = ptTmp->ptNext;	
		}
}


PT_DebugOpr GetDebugOpr(char *pcName)
{
	PT_DebugOpr ptTmp;
	ptTmp = g_ptDebugOprHead;
	while (ptTmp != NULL)
		{
			if (strcmp(ptTmp->name, pcName) == 0)
				{
					return ptTmp;
				}
			ptTmp = ptTmp->ptNext;
		}
	return NULL;
}

/* 
 * 设置打印等级 
 * 输入: DbgLevel=1/2/3...
 */
int SetDbgLevel(char *pcBuf)
{
	g_iDbgLevel = pcBuf[9] - '0';
	return 0;
}


/* 
 * 设置打印通道 
 * 输入: NetDbg=0/NetDbg=1/...
 */
int SetDbgChanel(char *pcBuf)
{
	char *pcTmp;
	char *pcNameBuf;
	PT_DebugOpr *ptBuf;
	pcTmp = strchr(pcBuf, '=');
	if (pcTmp == NULL)
		{
			return -1;
		}
	strncpy(pcNameBuf, pcBuf, pcTmp-pcBuf);
	ptBuf = GetDebugOpr(pcNameBuf);
	if (pcTmp[1] - '0')	
		{
			ptBuf->isUse = 1;
		}
	else 
		{
			ptBuf->isUse = 0;
		}
	return 0;
}



/*
 * 打印函数，使用方法同"printf"
 */
int DbgPrint(const char *str, ...)
{
	va_list tTmp;
	int iLevel;
	char cPrintBuf[1000];
	int iNum;
	int iDbgLevelDefault = 4;
	PT_DebugOpr ptTmp = g_ptDebugOprHead;
	va_start(tTmp, str);
	iNum = vsprintf(cPrintBuf, str, tTmp);
	va_end(tTmp);

	if ((cPrintBuf[0] == '<') && (cPrintBuf[2] == '>'))
		{
			iLevel = cPrintBuf[1] - '0';
			if ((iLevel >= 0) && (iLevel <= g_iDbgLevel))
				{
					cPrintBuf += 3;
					while (ptTmp != NULL)
						{
							if (ptTmp->isUse)
								{
									ptTmp->DebugPrint(cPrintBuf);
									ptTmp = ptTmp->ptNext;
								}
						}
				}
			else 
				{
					return -1;
				}
			
		}
	else
		{
			if (iDbgLevelDefault <= g_iDbgLevel)
				{
					cPrintBuf += 3;
					while (ptTmp != NULL)
						{
							if (ptTmp->isUse)
								{
									ptTmp->DebugPrint(cPrintBuf);
									ptTmp = ptTmp->ptNext;
								}
						}
				}
			else 
				{
					return -1;
				}
		}
	return 0;

	
}

/*
 * 将打印方式进行注册
 */
int DbgInitRegister()
{
	int iError;
	iError = NetPrintInit();
	return iError;
}

/*
 * 对isUse == 1 的打印方式进行初始化
 */
int DbgChanelInit()
{
	PT_DebugOpr ptTmp;
	ptTmp = g_ptDebugOprHead;
	
	while (ptTmp != NULL)
		{
			if (ptTmp->isUse)
				{
					ptTmp->DebugInit();
				}
			ptTmp = ptTmp->ptNext;
		}
	return 0;
}



