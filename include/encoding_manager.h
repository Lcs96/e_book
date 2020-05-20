#ifndef _ENCODING_MANAGER_H
#define _ENCODING_MANAGER_H

#include <fonts_manager.h>
#include <config.h>

typedef struct EncodeOpr{
	char *name;
	PT_FontOpr aptSupportFonts[aptFontsMax];
	int (*IsThisEncode)(unsigned char *BufferHead);
	int (*GetOneCode)(unsigned char *BufferStart, unsigned char *BufferEnd, unsigned int *puiCode)
}T_EncodeOpr, *PT_EncodeOpr;

int FreeTypeRegister(void);
int EncodeRegister(PT_EncodeOpr ptEncode);
int ShowRegEncode(void);
PT_EncodeOpr GetEncodeOpr(char * pcName);
PT_EncodeOpr SelectEncodeOfFile(unsigned char *pucBufferStart);
int EncodeInit(void);

#endif /*_ENCODING_MANAGER_H*/