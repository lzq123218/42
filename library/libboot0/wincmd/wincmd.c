#include<stdio.h>
#include<stdarg.h>
#include<conio.h>
#include<windows.h>
#define u8 unsigned char




static u8 last[16] = {0};
static int pos = 0;




static void attr(int val)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, val);
}
static int escapecolor(u8* p)
{
	//reset
	if(p == 0)
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		return 0;
	}

	//reset
	if(p[0] == '0')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		return 0;
	}

	//heavy
	else if(p[0] == '1')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
		return 1;
	}

	//foreground
	if(p[0] == '3')
	{
		if(p[1] == '0')
		{
			attr(0);
		}
		else if(p[1] == '1')
		{
			attr(FOREGROUND_RED);
		}
		else if(p[1] == '2')
		{
			attr(FOREGROUND_GREEN);
		}
		else if(p[1] == '3')
		{
			attr(FOREGROUND_GREEN|FOREGROUND_RED);
		}
		else if(p[1] == '4')
		{
			attr(FOREGROUND_BLUE);
		}
		else if(p[1] == '5')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_RED);
		}
		else if(p[1] == '6')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_GREEN);
		}
		else if(p[1] == '7')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		}
		return 3;
	}

	//background
	else if(p[0] == '4')
	{
		if(p[1] == '0')
		{
			attr(0);
		}
		else if(p[1] == '1')
		{
			attr(BACKGROUND_RED);
		}
		else if(p[1] == '2')
		{
			attr(BACKGROUND_GREEN);
		}
		else if(p[1] == '3')
		{
			attr(BACKGROUND_GREEN|BACKGROUND_RED);
		}
		else if(p[1] == '4')
		{
			attr(BACKGROUND_BLUE);
		}
		else if(p[1] == '5')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_RED);
		}
		else if(p[1] == '6')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_GREEN);
		}
		else if(p[1] == '7')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
		}
		return 4;
	}
}
static void gotoxy(int x, int y)
{
	COORD pos;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &bInfo);

	pos.X = x;
	pos.Y = bInfo.srWindow.Top + y;
	SetConsoleCursorPosition(hStdout, pos);
}
static void clearcmd()
{
	DWORD count;
	HANDLE hStdout;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hStdout, &bInfo);

	COORD pos;
	pos.X = bInfo.srWindow.Left;
	pos.Y = bInfo.srWindow.Top;
	FillConsoleOutputCharacter(
		hStdout,
		(TCHAR)' ',
		bInfo.dwSize.X * bInfo.dwSize.Y,
		pos,
		&count
	);
}
static int escapeposition(u8* p)
{
	int j, k, x=0, y=0;
	for(k=0;k<4;k++)
	{
		if(p[k] == ';')break;
	}
	if(k>=4)return 0;

	for(j=0;j<k;j++)
	{
		y = (y*10) + p[j] - 0x30;
	}

	for(;k<8;k++)
	{
		if((p[k] == 'H')|(p[k] == 'f'))break;
	}
	if(k>=8)return 0;

	j++;
	for(;j<k;j++)
	{
		x = (x*10) + p[j] - 0x30;
	}

	gotoxy(x-1, y-1);
	return 0;
}
static int escapesequence(u8* p)
{
	int j;
	int x,y;
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 1;
/*
	if(p[2] == '?')
	{
		for(j=3;j<8;j++)
		{
			if( (p[j] == 'h') | (p[j] == 'l') )
			{
				gotoxy(0,0);
				for(y=0;y<128;y++)
				{
					for(x=0;x<79;x++)
					{
						printf(" ");
					}
					printf("\b\n");
				}
				gotoxy(0,0);

				return j+1;
			}
		}
	}
*/
	//1b 5b 4a: Clear screen from cursor down
	if(p[2] == 'J')
	{
		clearcmd();
		return 3;
	}

	//1b 5b 4b: Clear line from cursor right
	if(p[2] == 'K')
	{
		//printf("        \b\b\b\b\b\b\b\b");
		return 3;
	}

	//1b 5b m
	if(p[2] == 'm')
	{
		escapecolor(0);
		return 3;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		escapecolor(p+2);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	//1b 5b ? ? h
	if(p[4] == 'h')
	{
		return 5;
	}

	//1b 5b ? ? l
	if(p[4] == 'l')
	{
		return 5;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		escapecolor(p+2);
		return 5;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 6;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 7;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		escapecolor(p+2);
		escapecolor(p+5);
		return 8;
	}

	for(j=2;j<10;j++)
	{
		if(p[j] == 'r')return j+1;
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			escapeposition(p+2);
			return j+1;
		}
	}

	return 0;
}




void lowlevel_output(u8* buf, int len)
{
	int i,j,k=0;
	if(pos != 0)
	{
		if(len < 8)
		{
			for(i=0;i<len;i++)last[pos+i] = buf[i];
		}
		else
		{
			for(i=0;i<8;i++)last[i] = buf[i];
			buf += 8;
			len -= 8;
		}

		j=0;
		while(1)
		{
			if(j>=pos)
			{
				for(j=0;j<pos;j++)last[pos] = 0;
				pos = 0;
				return;
			}

			if(buf[j] != 0x1b)
			{
				printf("%c", buf[j]);
				j++;
				continue;
			}

			i = escapesequence(buf+j);
			if(i != 0)
			{
				//printf("?");
				j += i;
				continue;
			}

			if(pos-j < 8)
			{
				for(i=0;i<pos-j;i++)last[i] = last[j+i];
				for(i=j;i<pos;i++)last[i] = 0;
				pos = j;
				return;
			}
		}
	}

	for(j=0;j<len;j++)
	{
		if(buf[j] >= 0xc0)
		{
			int ret;
			char utf8[8];
			char unicode[8];
			char gbk[8];
			if(j>k)printf("%.*s", j-k, buf+k);

			if(buf[j] < 0xe0)i = 2;
			else if(buf[j] < 0xf0)i = 3;
			else if(buf[j] < 0xf8)i = 4;
			else if(buf[j] < 0xfc)i = 5;
			else if(buf[j] < 0xfe)i = 6;
			k = j+i;

			utf8[i] = 0;
			for(i=i-1;i>=0;i--)utf8[i]=buf[j+i];
			j = k-1;

			ret = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (void*)unicode, 4);
			ret = WideCharToMultiByte(CP_ACP, 0, (void*)unicode, -1, gbk, 4, NULL, NULL);
			printf("%s",gbk);

		}
		else if(buf[j] == 0x1b)
		{
			if(j+7 >= len)
			{
				for(i=0;i<len-j;i++)last[pos+i] = buf[j+i];
				pos += len-j;
				break;
			}
			if(j>k)printf("%.*s", j-k, buf+k);

			i = escapesequence(buf+j);
			if(i != 0)
			{
				k = j+i;
				j = k-1;
			}
		}
	}
	if(j>k)printf("%.*s", j-k, buf+k);
}
int lowlevel_input(u8* buf)
{
	u8 ch;
	while(1)
	{
		ch = getch();
		if(ch == 0xe0)
		{
			ch = getch();
			if(ch == 0x48)return 0x415b1b;
			else if(ch == 0x50)return 0x425b1b;
			else if(ch == 0x4d)return 0x435b1b;
			else if(ch == 0x4b)return 0x445b1b;
			else continue;
		}
		return ch;
	}
	
}
void createserial(u8* arg)
{
	//opened?
	HWND consolewindow = GetConsoleWindow();
	if(consolewindow != NULL)return;

	//new
	AllocConsole();
	freopen("CONIN$","r",stdin);
	freopen("CONOUT$","w",stdout);

	//hide
	consolewindow = GetConsoleWindow();
	ShowWindow(consolewindow,SW_SHOW);
}
void deleteserial()
{
}
void* pollenv()
{
	return 0;
}
void* waitenv()
{
	return 0;
}
