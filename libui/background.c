﻿#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


QWORD whereisscreen();




/*
void gridding()
{
	//横
	int x,y;
	for(y=16;y<640;y+=16)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0x44444444);
		}
	}
	//竖
	for(x=16;x<1024;x+=16)
	{
		for(y=0;y<640;y++)
		{
			point(x,y,0x44444444);
		}
	}
}
*/
void background1()
{
	DWORD thatcolor=0;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=thatcolor;
	}

	/*
	thatcolor=0xfdfdfd;		//这个颜色挺好看的
	//say("thatcolor:%x\n",thatcolor);
	for(y=0;y<16;y++)	//上下
	{
		DWORD color=(QWORD)y*thatcolor;

		//上，编译器会不会用rep stosd指令优化呢?
		DWORD* p=screenbuf+y*1024;
		for(x=y;x<1024-y;x++)p[x]=color;

		//下
		p=screenbuf+(639-y)*1024;
		for(x=y;x<1024-y;x++)p[x]=color;
	}
	for(x=0;x<16;x++)	//左右
	{
		DWORD color=(QWORD)x*thatcolor;

		for(y=x;y<640-x;y++)
		{
			screenbuf[(y*1024)+x]=color;
			screenbuf[(y*1024)+1023-x]=color;
		}
	}
	*/
}









void background2()
{
	DWORD thatcolor=0xf0f0f0f0;
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=thatcolor;
	}

	//上下
	DWORD b=thatcolor&0xff;
	DWORD g=(thatcolor>>8)&0xff;
	DWORD r=(thatcolor>>16)&0xff;
	thatcolor=(b>>4)+( (g>>4) << 8 )+( (r>>4) << 16 );
	for(y=0;y<16;y++)
	{
		DWORD color=(QWORD)y*thatcolor;

		//上，编译器会不会用rep stosd指令优化呢?
		DWORD* p=screenbuf+y*1024;
		for(x=y;x<1024-y;x++)p[x]=color;

		//下
		p=screenbuf+(639-y)*1024;
		for(x=y;x<1024-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		DWORD color=(QWORD)x*thatcolor;

		for(y=x;y<640-x;y++)
		{
			screenbuf[(y*1024)+x]=color;
			screenbuf[(y*1024)+1023-x]=color;
		}
	}
}








void background3()
{
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=0x456789ab;
	}
}








void background4()
{
	DWORD* screenbuf=(DWORD*)whereisscreen();

	//用指定颜色清屏
	int x,y;
	for(x=0;x<1024*640;x++)
	{
		screenbuf[x]=0;
	}
}