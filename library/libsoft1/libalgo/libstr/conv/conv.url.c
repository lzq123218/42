#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int decstr2data(void*, void*);
void printmemory(char*, int);
void say(char*, ...);




int parseurl(u8* buf, int len, u8* addr, int* port)
{
	int j,k;
	for(j=0;j<16;j++)
	{
		if(0 == buf[j])break;
		if('/' == buf[j])break;
		if(':' == buf[j])
		{
			for(k=0;k<j;k++)addr[k] = buf[k];
			addr[j] = 0;

			j++;
			j += decstr2data(buf+j, port);
			return j;
		}
	}

	for(k=0;k<j;k++)addr[k] = buf[k];
	addr[j] = 0;
	return j;
}