#include <config.h>
#include <debug_manager.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>

#define SERVER_PORT 8888
#define NetPrintBufSize (16*1024)

static int g_iWritePos = 0;
static int g_iReadPos  = 0;
static int g_iSocketServer; 
static int g_iIsSetClient = 0;
static struct sockaddr_in g_tSocketServerAddr;
static struct sockaddr_in g_tSocketClientAddr;
static char *g_pcNetPrintBuf;
static pthread_t g_tSendTreadID;
static pthread_t g_tRecvTreadID;
static pthread_mutex_t g_tNetDbgSendMutex  = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t  g_tNetDbgSendConVar = PTHREAD_COND_INITIALIZER;


static T_DebugOpr NetDbgOpr = {
	.name  = "net",
	.isUst = 1;
	DebugInit = NetDbgInit;
	DebugExit = NetDbgExit;
	DebugPrint = NetDbgPrint;
};

/* empty : return 1; */
int BufEmpty()
{
	return (g_iWritePos == g_iReadPos);
}

/* full : return 1 */
int BufFull()
{
	return ((g_iWritePos + 1) % NetPrintBufSize == g_iReadPos);
}

int BufWrite(char val)
{
	if (BufFull())
		{
			return -1;
		}
	g_pcNetPrintBuf[g_iWritePos] = val;
	g_iWritePos = (g_iWritePos + 1) % NetPrintBufSize;
	return 0;
}

int BufRead(char *pcVal)
{
	if (BufEmpty())
		{
			return -1;
		}
	*pcVal = g_pcNetPrintBuf[g_iReadPos];
	g_iReadPos = (g_iReadPos + 1) % NetPrintBufSize;
	return 0;
}


static void *NetPrintRecvThread(void *pVoid)
{
	socklen_t iAddrLen;
	int iRecvLen;
	char ucRecvBuf[1000];
	struct sockaddr_in tSocketClientAddr;

	while (1)
		{
			iAddrLen = sizeof(struct sockaddr);
			iRecvLen = recvfrom(g_iSocketServer, ucRecvBuf, 999, 0, (struct sockaddr *)&tSocketClientAddr, &iAddrLen);
			if (iRecvLen > 0)
				{
					ucRecvBuf[iRecvLen] = '\0';
					if (strcmp(ucRecvBuf, "setclient") == 0)
						{
							g_tSocketClientAddr = tSocketClientAddr;
							g_iIsSetClient = 1;
						}
					else if (strncmp(ucRecvBuf, "DbgLevel=", 9) == 0)
						{
							SetDbgLevel(ucRecvBuf);
						}
					else 
						{
							SetDbgChanel(ucRecvBuf);
						}
				}
		}
	return NULL;
}

static void *NetPrintSendThread(void *pVoid)
{
	char strTmpBuf[512];
	int iVal;
	int i;
	int iAddrLen;
	int iSendLen;

	while (1)
		{
			pthread_mutex_lock(&g_tNetDbgSendMutex);
			pthread_cond_wait(&g_tNetDbgSendConVar, &g_tNetDbgSendMutex);	
			
			while ((BufEmpty() != 1) && (g_iIsSetClient))
				{
					for (i = 0; i < 512; i++)
					{
						 iVal = BufRead(&strTmpBuf[i]); 
						 if (iVal)
						 	{
						 		break;
						 	}
					}
					iAddrLen = sizeof(struct sockaddr);
					iSendLen = sendto(g_iSocketServer, strTmpBuf, i, 0,
			                      (const struct sockaddr *)&g_tSocketClientAddr, iAddrLen);

				}
			pthread_mutex_unlock(&g_tNetDbgSendMutex);
		}
	return 0;
}

int NetDbgInit()
{
	int iRet;
	g_iSocketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_iSocketServer == -1)
		{
			printf("can't socket\n");
			return -1;
		}
	g_tSocketServerAddr.sin_family      = AF_INET;
	g_tSocketServerAddr.sin_port        = htons(SERVER_PORT);  /* host to net, short */
 	g_tSocketServerAddr.sin_addr.s_addr = INADDR_ANY;
	memset(g_tSocketServerAddr.sin_zero, 0, 8);
	
	iRet = bind(g_iSocketServer, (const struct sockaddr *)&g_tSocketServerAddr, sizeof(struct sockaddr));
	if (-1 == iRet)
	{
		printf("bind error!\n");
		return -1;
	}
	g_pcNetPrintBuf = malloc(NetPrintBufSize);
	if (-1 == g_pcNetPrintBuf)
		{
			close(g_iSocketServer);
			return -1;
		}
	pthread_create(&g_tSendTreadID, NULL, NetPrintSendThread. NULL);
	pthread_create(&g_tSendTreadID, NULL, NetPrintRecvThread. NULL);
	return 0;
}

void NetDbgExit()
{
	free(g_pcNetPrintBuf);
	close(g_iSocketServer);
}

int NetDbgPrint(char *buf)
{
	int iNum;
	int i;
	pthread_mutex_lock(&g_tNetDbgSendMutex);
	iNum = strlen(buf);
	for (i = 0; i < iNum; i++)
		{
			if (BufWrite(buf[i]))
				{
					return -1;
				}
		}
	pthread_cond_signal(&g_tNetDbgSendConVar);
	pthread_mutex_unlock(&g_tNetDbgSendMutex);
	return i;
}


int NetPrintInit()
{
	return DebugOprRegister(&NetDbgOpr);
}













