/*
1:	add name in library/libui1/makefile
2:	add code in character.c character_init() and character_kill()
3:	replace "example" to "whatname"----->		:0,$s/example/whatname/g
4:	do your logic in this code
*/




#define QWORD unsigned long long
#define DWORD unsigned int
//
void backgroundcolor();
//
void say(char*,...);





static void example_read_html()
{
}
static void example_read_pixel()
{
}
static void example_read_text()
{
}
static void example_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		example_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
	{
		example_read_html();
	}

	//pixel
	else
	{
		example_read_pixel();
	}
}




static void example_write()
{
}




static void example_list()
{
}
static void example_change()
{
}




static void example_start()
{
}
static void example_stop()
{
}
void example_init(char* base,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x746e6f66;

	this[10]=(QWORD)example_start;
	this[11]=(QWORD)example_stop;
	this[12]=(QWORD)example_list;
	this[13]=(QWORD)example_change;
	this[14]=(QWORD)example_read;
	this[15]=(QWORD)example_write;
}
void example_kill()
{
}
