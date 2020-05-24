#include <debug_manager.h>



static T_DebugOpr NetDbgOpr = {
	.name  = "net",
	.isUst = 1;
	DebugInit = NetDbgInit;
	DebugExit = NetDbgExit;
	DebugPrint = NetDbgPrint;
};

int NetDbgInit()
{
	
}

void NetDbgExit()
{

}

int NetDbgPrint(char *buf)
{
	
}


int NetPrintInit()
{
	return DebugOprRegister(&NetDbgOpr);
}













