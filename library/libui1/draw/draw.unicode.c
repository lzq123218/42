#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
u8* whereisunicodetable();
void say(void*, ...);




static struct temp{
	u64 type;
	u64 id;
	u64 start;
	u64 end;

	u64 pixelbuffer;
	u64 pixelformat;
	u64 width;
	u64 height;
}*haha;
static u8* unicodetable=0;




void printunicodefromvalue(int xxxx, int yyyy, u8* p)
{
	int x,y;
	u16 temp;

	u32* this = (u32*)( haha->pixelbuffer + ( xxxx + (yyyy * haha->width) )*4 );
	for(y=0;y<0x10;y++)
	{
		temp  = p[y*2] << 8;
		temp += p[(y*2)+1];
		for(x=0;x<0x10;x++)
		{
			if( (temp&0x8000) == 0x8000 )
			{
				this[x+(y*1024)]=0xffffffff;
			}
			temp=temp<<1;
		}
	}//for
}
void printunicode(int x,int y,u32 value)
{
	printunicodefromvalue( x , y , unicodetable+(value*0x20) );
}




void printunicodefromvaluebig(int xxxx, int yyyy, u8* p)
{
	int x,y,i,j;
	unsigned short temp;

	u32* this = (u32*)(haha->pixelbuffer + ( xxxx + (yyyy * haha->width) )*4 );
	for(y=0;y<0x10;y++)
	{
		temp  = p[y*2] << 8;
		temp += p[(y*2)+1];

		for(x=0;x<0x10;x++)
		{
			if( (temp&0x8000) == 0x8000 )
			{
				/*
				this[ (x*4) +   (y*4096) ]=0xffffffff;
				this[ (x*4+1) + (y*4096) ]=0xffffffff;
				this[ (x*4+2) + (y*4096) ]=0xffffffff;
				this[ (x*4+3) + (y*4096) ]=0xffffffff;
				this[ (x*4) +   (y*4096+1024) ]=0xffffffff;
				this[ (x*4+1) + (y*4096+1024) ]=0xffffffff;
				this[ (x*4+2) + (y*4096+1024) ]=0xffffffff;
				this[ (x*4+3) + (y*4096+1024) ]=0xffffffff;
				this[ (x*4) +   (y*4096+2048) ]=0xffffffff;
				this[ (x*4+1) + (y*4096+2048) ]=0xffffffff;
				this[ (x*4+2) + (y*4096+2048) ]=0xffffffff;
				this[ (x*4+3) + (y*4096+2048) ]=0xffffffff;
				this[ (x*4) +   (y*4096+3072) ]=0xffffffff;
				this[ (x*4+1) + (y*4096+3072) ]=0xffffffff;
				this[ (x*4+2) + (y*4096+3072) ]=0xffffffff;
				this[ (x*4+3) + (y*4096+3072) ]=0xffffffff;
				*/

				for(i=0;i<16;i++)
				{
				for(j=0;j<16;j++)
				{
					this[ (y*1024*16)+(i*1024) + (x*16+j) ]=0xffffffff;
				}//forj
				}//fori
			}//forx

			temp=temp<<1;
		}
	}//for
}
void printunicodebig(int x,int y,u32 value)
{
	printunicodefromvaluebig( x , y , unicodetable+(value*0x20) );
}




void unicode_create(void* home,void* me)
{
	unicodetable=(u8*)whereisunicodetable();

	haha=me;
	haha->type=0;
	haha->id=0x65646f63696e75;
}
void unicode_delete()
{
}
