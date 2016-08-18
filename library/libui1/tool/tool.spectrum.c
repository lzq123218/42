#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void rectbody(int x1, int y1, int x2, int y2, DWORD color);
void backgroundcolor(DWORD);
void background1();
//
void arteryread(char* rdi,QWORD rsi,QWORD rcx);
void arterywrite(char* rdi,QWORD rsi,QWORD rcx);
void data2hexstring(QWORD,char*);
int compare(char*,char*);
//
unsigned int getrandom();
void printmemory(char*,int);
void say(char*,...);




static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
}*haha;

//before
static double* databuf=0;
static int maxpower;

//after
static double* real;		//8*2048=0x4000
static double* imag;		//8*2048=0x4000
static double* power;		//8*1024=0x2000
static double* phase;		//8*1024=0x2000




static void spectrum_write(QWORD type,QWORD key)
{
	if(type==0x656c6966706f7264)		//'dropfile'
	{
	}
	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
		}
		else if(key==0x27)		//right	0x4d
		{
		}
		else if(key==0x26)		//up	0x4b
		{
		}
		else if(key==0x28)		//down	0x4d
		{
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==9)					//tab
		{
		}
		else if(key==0x8)			//backspace
		{
		}
		else if(key==0xd)			//enter
		{
		}
		else
		{
		}
	}
	else if(type==0x7466656C207A7978)		//'xyz left'
	{
		int i;
		for(i=0;i<2048;i++)
		{
			//real[i] = (double)(i*63);
			real[i] = (double)(getrandom()%maxpower);
		}
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
	}
}
static void spectrum_read()
{
	int x,y;
	int width,height,min;

	width = haha->width;
	height = haha->height;
	if(width<height)min=width;
	else min=height;

	rectbody(0, 0, min, min, 0);
	for(x=0;x<1024;x++)
	{
		y = min - (int)(real[x] * (double)min / (double)maxpower);

		rectbody(x*min/1024, y, x*min/1024, min, 0xffffffff);
//say("%x,%x\n",leftupper,rightbottom);
	}
}




static void spectrum_list(QWORD* this)
{
}
static void spectrum_into()
{
}




void spectrum_start()
{
	int j;
	backgroundcolor(0);

	maxpower=32768;
	for(j=0;j<2048;j++)
	{
		real[j] = (double)(j*31);
		//real[j] = (double)(random()%maxpower);
	}
}
void spectrum_stop()
{
}
void spectrum_create(void* uibuf,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x6d75727463657073;

	this[10]=(QWORD)spectrum_start;
	this[11]=(QWORD)spectrum_stop;
	this[12]=(QWORD)spectrum_list;
	this[13]=(QWORD)spectrum_into;
	this[14]=(QWORD)spectrum_read;
	this[15]=(QWORD)spectrum_write;

	databuf=(double*)(uibuf+0x200000);
	real=(double*)(uibuf+0x300000);
	imag=(double*)(uibuf+0x340000);
	power=(double*)(uibuf+0x380000);
	phase=(double*)(uibuf+0x3c0000);
}
void spectrum_delete()
{
}
