#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void say(void*, ...);




struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim; 

	u64 w;
	u64 h;
	u64 d;
	u64 t;
};




void ttf_one(struct window* win,
	int xxxx, int yyyy, int size, u32 code)
{
}
void ttf_str(struct window* win,
	int xxxx, int yyyy, int size, u8* code)
{
}
void ttf_create(void* addr)
{
}