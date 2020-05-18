#include <disp_manager.h>
#include <config.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>


static struct fb_var_screeninfo var;
static int bytes_pixel;
static int bytes_line;
static int bytes_screen;
static unsigned char *fbmem = NULL;

int FbInit(void);
void FbClean(void);
int FbPutPixel(unsigned int x, unsigned int y, unsigned int color);

static T_DispInfo g_tFbInfo = {
	.name         = "fb",
	.DispInit     = FbInit,
	.DispClean    = FbClean,
	.DispPutPixel = FbPutPixel,	
};

int FbInit(void)
{	
	int fd_fb;
	fd_fb = open("/dev/fb0", O_RDWR);
	if (fd_fb < 0)
		{
			printf("can't open fb\n");
			return -1;
		}
	if (ioctl(fd_fb, FBIOGET_VSCREENINFO, &var))
		{
			printf("can't get var info\n");
			return -1;
		}
	bytes_pixel  = var.bits_per_pixel / 8;
	bytes_line   = var.xres * bytes_pixel;
	bytes_screen = var.yres * bytes_line;
	fbmem = (unsigned char *)mmap(NULL , bytes_screen, PROT_READ | PROT_WRITE, MAP_SHARED, fd_fb, 0);
	if (fbmem < 0)
		{
			printf("can't mmap\n");
			return -1;
		}
	g_tFbInfo.uiXres = var.xres;
	g_tFbInfo.uiYres = var.yres;
	g_tFbInfo.uiBpp  = var.bits_per_pixel;
	
	return 0;
}

void FbClean(void)
{
	memset(fbmem, 0, bytes_screen);
}

int FbPutPixel(unsigned int x, unsigned int y, unsigned int color)
{
	unsigned char  *pcBpp8;
	unsigned short *pcBpp16;
	unsigned int   *pcBpp32;
	unsigned char  red;
	unsigned char  green;
	unsigned char  blue;
	if (x > g_tFbInfo.uiXres || y > g_tFbInfo.uiYres)
		{
			printf("out of region\n");
			return -1;
		}
	pcBpp8  = fbmem + y*bytes_line + x*bytes_pixel;
	pcBpp16 = (unsigned short *)pcBpp8;
	pcBpp32 = (unsigned int   *)pcBpp16;
	switch (var.bits_per_pixel)
		{
			case 8:
				*pcBpp8  = color;
				break;
			case 16:
				red      = (color >> 16) & 0xff;
				green    = (color >> 8)  & 0xff;
				blue     = (color >> 0)  & 0xff;
				*pcBpp16 = ((red>>3)<<11) | ((green>>2)<<5) | (blue>>3);
				break;
			case 32:
				*pcBpp32 = color;
				break;
			default:
				printf("can't support this bpp\n");
				break;
		}
	return 0;
}

int FbRegister()
{
	RegisterDispInfo(&g_tFbInfo);
	return 0;
}





