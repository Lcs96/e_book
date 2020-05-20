#include <encoding_manager.h>
#include <config.h>

static PT_EncodeOpr g_aptEncode[aptEncodeMax];


int EncodeRegister(PT_EncodeOpr ptEncode)
{
	int i;
	for (i = 0; i < aptEncodeMax; i++)
		{
			if (g_aptEncode[i] == NULL)
				{
					g_aptEncode[i] = ptEncode;
					return 0;
				}
		}
	printf("can't register %s\n", ptEncode->name);
	return -1;
}

int ShowRegEncode(void)
{
	int i;
	for (i = 0; i < aptEncodeMax; i++)
		{
			if (g_aptEncode[i] != NULL)
				{
					printf("%s\n",g_aptEncode[i]->name);
				}
		}
	return 0;
}

PT_EncodeOpr GetEncodeOpr(char * pcName)
{
	int i;
	for (i = 0; i < aptEncodeMax; i++)
		{
			if ((g_aptEncode[i] != NULL) && (strcmp(g_aptEncode[i]->name, pcName) == 0))
				{
					return g_aptEncode[i];
				}
		}
	printf("can't find %s's Encodeopr\n", g_aptEncode[i]->name);
	return -1;
}


PT_EncodeOpr SelectEncodeOfFile(unsigned char *pucBufferStart)
{
	int i;
	for (i == 0; i < aptEncodeMax; i++)
		{
			if ((g_aptEncode[i] != NULL) && g_aptEncode[i]->IsThisEncode(pucBufferStart))
				{
					return g_aptEncode[i];
				}
		}
	printf("can't support this file\n");
	return -1;
}


int EncodeInit()
{
	return UTF8Init();
}












