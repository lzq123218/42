#define u64 unsigned long long
#define u32 unsigned int
//
void printascii(
	int x, int y, int size, char ch, u32 fgcolor, u32 bgcolor);
void printdecimal(
	int x, int y, int size, int data, u32 fg, u32 bg);
void backgroundcolor(
	u64, u64, u64, u64,
	u32);
//
void say(char*,...);




static u64* this;





void font_list()
{
}
void font_change()
{
}
void font_read()
{
	int x,y;
	backgroundcolor(
		this[4], this[5], this[6], this[7],
		0
	);

	for(y=2;y<8;y++)
	{
		for(x=0;x<16;x++)
		{
			printascii(
				x*16,
				y*16,
				1,
				x+y*16,
				0xffffffff,
				0
			);
		}
	}
}
void font_write(u64* who, u64* what, u64* how)
{
}
void font_start()
{
}
void font_stop()
{
}
void font_create(char* base,char* addr)
{
	this=(u64*)addr;
	this[0] = 0x74736574;
	this[1] = 0x746e6f66;

	this[10]=(u64)font_start;
	this[11]=(u64)font_stop;
	this[12]=(u64)font_list;
	this[13]=(u64)font_change;
	this[14]=(u64)font_read;
	this[15]=(u64)font_write;
}
void font_delete()
{
}
