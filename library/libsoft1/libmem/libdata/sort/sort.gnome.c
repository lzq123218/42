#define u8 unsigned char




void sort_gnome(u8* buf, int len)
{
	u8 t;
	int j=0;
	while(1)
	{
		if(j>=len)break;
		if( (j==0) | (buf[j-1] <= buf[j]) )
		{
			j++;
		}
		else
		{
			t = buf[j-1];
			buf[j-1] = buf[j];
			buf[j] = t;

			j--;	
		}
	}
}
