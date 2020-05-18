#ifndef _DISP_MANAGER_H
#define _DISP_MANAGER_H

typedef struct DispInfo{
	char *name;
	int uiXres;
	int uiYres;
	int uiBpp;
	int (*DispInit)(void);
	int (*DispClean)(void);
	int (*DispPutPixel)(unsigned int x, unsigned int y, unsigned int color);
}T_DispInfo, *PT_DispInfo; 


int  RegisterDispInfo(PT_DispInfo ptDispInfo);
void ShowDispInfo(void);
int  SelectDispInfo(char *pcName);
int  InitDisp(char *pcName);
int  InitFB(void);



#endif  /* _DISP_MANAGER_H */
