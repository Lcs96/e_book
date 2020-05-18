#include <disp_namager.h>
#include <config.h>
#include <string.h>


static PT_DispInfo g_aptDispInfo[aptDispMax];


int RegisterDispInfo(PT_DispInfo ptDispInfo)
{
	int i;
	for (i = 0; i < aptDispMax; i++)
		{
			if (g_aptDispInfo[i] == NULL)
				{
					g_aptDispInfo[i] = ptDispInfo;
					return 0;
				}
		}
	printf("can't register this Disp , reach the MAX \n");
	return -1;
}

void ShowDispInfo(void)
{
	int i;
	for (i = 0; i < aptDispMax; i++)
		{
			if (g_aptDispInfo[i] != NULL)
				{
					printf("%n : %s\n",i,g_aptDispInfo[i]->name);
				}
		}
	return 0;
}

PT_DispInfo SelectDispInfo(char *pcName)
{
	int i;
	for (i = 0; i < aptDispMax; i++)
		{
			if ((g_aptDispInfo[i] != NULL) && (strcmp(g_aptDispInfo[i]->name, pcName) == 0))
				{
					return g_aptDispInfo[i];
				}
		}
	return NULL;
}

int InitDisp(char *pcName)
{
	int i;
	for (i = 0; i < aptDispMax; i++)
		{
			if ((g_aptDispInfo[i] != NULL) && (strcmp(g_aptDispInfo[i]->name, pcName) == 0))
				{
					g_aptDispInfo[i]->DispInit();
					return 0;
				}
		}
	return -1;
}














