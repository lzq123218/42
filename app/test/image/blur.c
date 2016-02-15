//martin buttner
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void initwindow();
void killwindow();
void writewindow(QWORD size,void* addr);
void uievent(QWORD* type,QWORD* key);
//
void blur_gauss_x(QWORD size,QWORD radius,BYTE* src,BYTE* dst);
void blur_gauss_y(QWORD size,QWORD radius,BYTE* src,BYTE* dst);
void blur_gauss_xy(QWORD size,QWORD radius,BYTE* src,BYTE* dst);
void blur_gauss_2(QWORD size,QWORD radius,BYTE* src,BYTE* dst);
void blur_box(QWORD size,QWORD radius,BYTE* src,BYTE* dst);




//
static DWORD palette[1024*1024];
static DWORD final[1024*1024];




//
#define _sq(x) ((x)*(x))
#define r(n)(rand()%n)
unsigned char RED(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):RED((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char GREEN(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):GREEN((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char BLUE(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):BLUE((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}




//
void processmessage(QWORD type,QWORD key)
{
	int i;
	printf("%llx,%llx\n",type,key);

	if(type==0x72616863)             //'char'
	{
		//0
		if(key==0x30)
		{
			for(i=0;i<1024*1024;i++)
			{
				final[i]=palette[i];
			}
		}
		//1
		else if(key==0x31)
		{
			blur_gauss_x(0x04000400,9,(BYTE*)palette,(BYTE*)final);
		}
		//2
		else if(key==0x32)
		{
			blur_gauss_y(0x04000400,9,(BYTE*)palette,(BYTE*)final);
		}
		//3
		else if(key==0x33)
		{
			blur_gauss_xy(0x04000400,9,(BYTE*)palette,(BYTE*)final);
		}
		//4
		else if(key==0x34)
		{
			blur_gauss_2(0x04000400,9,(BYTE*)palette,(BYTE*)final);
		}
		//5
		else if(key==0x35)
		{
			blur_box(0x04000400,9,(BYTE*)palette,(BYTE*)final);
		}
	}
}
void main()
{
	//before
	int x,y;
	int r,g,b;
	initwindow();

	//picture
	char* src=(char*)final;
	char* dst=(char*)palette;
	FILE* fp=fopen("4.ppm","r");
	fread(final,0x100000,4,fp);
	fclose(fp);
	for(x=0;x<0x100000;x++)
	{
		dst[0+x*4]=src[0x13+x*3];
		dst[1+x*4]=src[0x12+x*3];
		dst[2+x*4]=src[0x11+x*3];
	}
	processmessage(0x72616863,0x30);

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		writewindow(0x04000400,final);

		//2.等事件，是退出消息就退出
		uievent(&type,&key);
		if( type==0 )break;

		//3.处理事件
		processmessage(type,key);
	}

	//after
	killwindow();
}
