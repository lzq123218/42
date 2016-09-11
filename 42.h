#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char

//libui1
void line(		int x1, int y1, int x2, int y2, u32 color);
void rectbody(		int x1, int y1, int x2, int y2, u32 color);
void rectframe(		int x1, int y1, int x2, int y2, u32 color);
void rect(		int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolor);
void circlebody(	int x, int y, int r, u32 color);
void circleframe(	int x, int y, int r, u32 color);
void circle(		int x, int y, int r, u32 bodycolor, u32 framecolor);

void printascii(	int x, int y, int size, char ch, int data, u32 fg, u32 bg);
void printbyte( 	int x, int y, int size, char ch, int data, u32 fg, u32 bg);
void printstring( 	int x, int y, int size, char* str, int data, u32 fg, u32 bg);
void printdecimal(	int x, int y, int size, u64 data, u32 fg, u32 bg);
void printhexadecimal(	int x, int y, int size, u64 data, u32 fg, u32 bg);
void printdouble(	int x, int y, int size, double data, u32 fg, u32 bg);

void defaultascii(int x,int y,unsigned char ch);
void defaultstring(int x,int y,char* p);
void defaultdecimal(int x,int y,long long dec);
void defaulthexadecimal(int x,int y,unsigned long long hex);
void defaultdouble(int x,int y,double data)
void backgroundcolor(u32 color);

//libui0
#define input_char 0x72616863
#define input_kbd 0x64626b
	#define kbd_esc 0x1b
	#define kbd_f1 0x70
	#define kbd_f2 0x71
	#define kbd_f3 0x72
	#define kbd_f4 0x73
	#define kbd_left 0x25
	#define kbd_up 0x26
	#define kbd_right 0x27
	#define kbd_down 0x28
#define touch_move 0x4070
#define touch_down 0x2b70
#define touch_up 0x2d70
#define mouse_move 0x65766F6D207A7978
#define mouse_left 0x7466656C207A7978
#define mouse_right 0x72676968207A7978
#define mouse_front 0x6B636162207A7978
#define mouse_back 0x6E6F7266207A7978
#define screen_size 0x657a6973
#define drag_file 0x656c6966706f7264

//libsoft1
int compare(char* a,char* b);
int wildcard(char* wild,char* str);
int decstring2data(u8* source,u64* data);
int hexstring2data(u8* source,u64* data);
int data2hexstring(u64 data,u8* string);
int data2decimalstring(u64 data,u8* string);
int double2decimalstring(double data,u8* string);
int utf2unicode(unsigned char* src,unsigned int* dst);
int unicode2utf(unsigned int src,unsigned char* dst);

int buf2typename(u8* p,int max,u64* type,u8** name);
int buf2optval(u8* pp,int max,u8** type,u8** name);
int buf2suffix(u8* p,u8** suffix);
void buf2arg(u8* buf,int max,int* argc,u8** argv);
void buf2addrport(u8* pp,int max,u8* addr,int* port);
char* buf2folder(char* p);
char* buf2filename(char* p);

//libsoft0
unsigned int getrandom();
unsigned long long shi_ji_nian_yue_ri_shi_fen_miao();
unsigned long long hao_wei_na_pi();

//libboot1
void printmemory(char*,int);
void say(char*,...);

//libboot0
char getchar();
void putchar(char ch);
void power_off();
void power_on();