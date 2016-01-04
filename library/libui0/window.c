#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>
void initwindowworker(char* addr);
void killwindowworker();
unsigned long long windowresolution();




//画板的位置
static char* window=0;
char* whereiswindow(int i)	//请求第一个窗户(忽略)
{
	return window;
}
QWORD howiswindow(int i)	//请求第几个窗户(忽略)
{
	return windowresolution();
}




//unicode字符表的位置
static unsigned char* unicodetable=0;
unsigned char* whereisunicodetable()
{
	//return (unsigned char*)&_binary_unicode_unicode_start;
	return unicodetable;
}




void initwindow(char* addr)
{
	//开窗口先
	window=(char*)malloc(0x400000);
	initwindowworker(window);

	//从当前目录读取unicode点阵字符文件进入内存
	int ret=0;
	FILE* fp=fopen("unicode.bin","r");
	if(fp==NULL)return;

	ret=fread(addr , 0x1000 , 0x200 , fp);	//0x200,000=2MB
	if(ret<0x200)return;

	if(fp!=NULL)fclose(fp);
}
void killwindow()
{
	//1024*1024*4
	if(window != 0)
	{
		killwindowworker();
		free(window);
		window=0;
	}
}
