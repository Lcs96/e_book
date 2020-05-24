#ifndef _DEBUG_MANAGER_H
#define _DEBUG_MANAGER_H

typedef struct DebugOpr{
	char *name;
	int isUse;
	int (*DebugInit)(void);
	int (*DebugExit)(void);
	int (*DebugPrint)(char *str);
	struct DebugOpr *ptNext;
}T_DebugOpr, PT_DebugOpr;









#endif /* _DEBUG_MANAGER_H */