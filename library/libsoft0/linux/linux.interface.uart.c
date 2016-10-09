#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<pthread.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
u64 gettime();
void say(char*,...);




//
static int fd = -1;
static int alive = 1;
static pthread_t id;




int systemuart_list()
{
	return system("ls /dev/tty*");
}
int systemuart_choose()
{
}




void* systemuart_read(void* p)
{
	int ret;
	char buf[256];
	while(alive == 1)
	{
		ret = read(fd, buf, 256);
		buf[ret] = 0;

		printf("%s", buf);
		fflush(stdout);
	}
	return 0;
}
int systemuart_write(char* p)
{
	int ret;
	ret = write(fd, p, strlen(p));
	ret+= write(fd, "\n", 1);
	return ret;
}




int systemuart_start(char* p)
{
	struct termios option;

	fd=open(p , O_RDWR | O_NOCTTY | O_NDELAY);
	if(fd<=0)
	{
		say("error@systemuart_start.open\n");
		return -1;
	}

	//get
	tcgetattr(fd,&option);
	fcntl(fd,F_SETFL,0);

	//8n1
	option.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
	option.c_iflag = IGNPAR;
	option.c_oflag = 0;
	option.c_lflag=0;

	//set
	tcflush(fd,TCIFLUSH);
	tcsetattr(fd,TCSANOW,&option);

	//
	alive = 1;
	pthread_create(&id, NULL, systemuart_read, NULL);
}
int systemuart_stop()
{
	if(fd>0)
	{
		alive = 0;

		close(fd);
		fd = -1;
	}
}




void systemuart_create()
{
}
void systemuart_delete()
{
}
