#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<signal.h>
#include<pthread.h>
#include<sys/ioctl.h>
#include<sys/select.h>
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




//窗口
static struct winsize w;
static int width,height;
static int lastwidth=0,lastheight=0;
//输入
static int flag=-1;
static struct termios old;
static struct termios new;
//
static u8* textbuf=0;
static pthread_t id;




static void newsize(int num)
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
}
void* uievent(void* p)
{
	u8 ch;

	while(1)
	{
		if(lastwidth != width)
		{
			lastwidth = width;
			lastheight = height;
			eventwrite(width + (height<<16), 0x657a6973, 0, 0);
		}

		ch = getchar();
		if( (ch == 0xff) | (ch == 0) )
		{
			usleep(10000);
			continue;
		}
		else if(ch==0x1b)
		{
			ch = getchar();
			if( (ch == 0xff) | (ch == 0) )
			{
				usleep(10000);
				ch = getchar();
				if( (ch == 0xff) | (ch == 0) )
				{
					eventwrite(0x1b, 0x64626b, 0, 0);
				}
			}

			else if(ch==0x5b)
			{
				ch = getchar();
				if(ch == 0x41)//up
				{
					eventwrite(0x26, 0x64626b, 0, 0);
				}
				if(ch == 0x42)//down
				{
					eventwrite(0x28, 0x64626b, 0, 0);
				}
				if(ch == 0x44)//left
				{
					eventwrite(0x25, 0x64626b, 0, 0);
				}
				if(ch == 0x43)//right
				{
					eventwrite(0x27, 0x64626b, 0, 0);
				}
			}//5b
		}//1b
		else
		{
			if(ch == 0x7f)ch = 8;
			if(ch == 0xa)ch = 0xd;

			eventwrite(ch, 0x72616863, 0, 0);
		}
	}//while
}
static void attr(u8 bg, u8 fg)
{
	char str[6] = {0x1b, 0x5b, '4', 0, 'm', 0};
	if( (bg > 0) && (bg <8) )
	{
		str[3] = 0x30 + bg;
		printf("%s", str);
	}
	else printf("\033[0m");
}




void windowwrite()
{
	printf("\033[H\033[J");
/*
	printf("%s",textbuf);
	fflush(stdout);
*/
	int x,y;
	u8* p;
	u8 ch,bg=0,fg=0;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			p = textbuf + ((width*y + x)<<2);
			if(p[0] > 0x80)
			{
				//先颜色
				if(bg != p[7])
				{
					bg = p[7];
					attr(bg,fg);
				}

				//这是汉字
				printf("%2s",p);
				x++;
			}
			else
			{
				//先颜色
				if(bg != p[3])
				{
					bg = p[3];
					attr(bg,fg);
				}

				//这是ascii
				ch = p[0];
				if(ch < 0x20)ch = 0x20;
				printf("%c",ch);
			}
		}
	}
	if(bg != 0)attr(0,0);
}
void windowread(char* addr)
{
}




void windowlist()
{
}
void windowchange()
{
}




void windowstart(u8* addr, u8* pixfmt, int x, int y)
{
	textbuf = addr;
	*(unsigned int*)pixfmt = 0x74786574;
}
void windowstop()
{
}




void windowcreate()
{
	ioctl(0, TIOCGWINSZ, &w);
	width=w.ws_col;
	height=w.ws_row;
	signal(SIGWINCH,newsize);

	//
	flag=tcgetattr(STDIN_FILENO,&old);
	new=old;
	new.c_lflag&=~(ICANON|ECHO);
	new.c_cc[VTIME] = 0;
	new.c_cc[VMIN] = 1;
	tcsetattr(STDIN_FILENO,TCSANOW,&new);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);

	//
	pthread_create(&id, NULL, uievent, NULL);
}
void windowdelete()
{
	if(flag!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&old);
}
