#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "libsoft.h"
#define MAXSIZE 4096
void kqueue_add(int);
void kqueue_del(int);
void kqueue_mod(int);




struct object* obj;
//
static int btfd = 0;
static int rawfd = 0;
static int tcpfd = 0;
static int udpfd = 0;




void selfname(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	getsockname(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}
void peername(u64 fd, u32* buf)
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr_in);
	getpeername(fd, (void*)&addr, &len);

	buf[0] = *(u32*)&addr.sin_addr;
	buf[1] = addr.sin_port;
}




int writesocket(int fd, void* tmp, void* buf, int len)
{
	u64 type;
	int ret;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type;
	if((_UDP_ == type)|(_udp_ == type))
	{
		ret = sendto(
			fd, buf, len, 0,
			tmp, sizeof(struct sockaddr_in)
		);
		return ret;
	}

	ret = write(fd, buf, len);
	return ret;
}
int readsocket(int fd, void* tmp, void* buf, int len)
{
	u64 type;
	int ret, cnt=0;
	if(fd == 0)return 0;
	if(buf == 0)return 0;

	type = obj[fd].type;
	if((_UDP_ == type)|(_udp_ == type))
	{
		ret = sizeof(struct sockaddr_in);
		ret = recvfrom(
			fd, buf, len, 0,
			tmp, (void*)&ret
		);
		return ret;
	}

	while(1)
	{
		ret = read(fd, buf, len);
		if(ret > 0)break;
		if(ret == 0)return -1;		//disconnect
		if(errno != 11)return -2;	//errno
		if(cnt >= 100)return 0;

		usleep(1000);
		cnt++;
	}
	return ret;
}
int listsocket()
{
	return 0;
}
int choosesocket()
{
	return 0;
}
void stopsocket(int x)
{
	int ret = close(x);
	obj[x].type = 0;
	obj[x].name = 0;
	printf("---- %d %d, %d\n", x, ret, errno);
}
int startsocket(char* addr, int port, int type)
{
	int fd;
	int ret;
	struct hostent* host;

	//RAW
	if(_RAW_ == type)
	{
		return 0;
	}

	//raw client
	if(_raw_ == type)
	{
		return 0;
	}

	for(ret=0;ret<256;ret++)
	{
		if((addr[ret]>='a')&&(addr[ret]<='z'))
		{
			host = gethostbyname(addr);
			addr = inet_ntoa(*(struct in_addr*)host->h_addr_list[0]);
			break;
		}
	}

	//udp server
	if(_UDP_  == type)
	{
		int ret;
		struct sockaddr_in* self;

		//create
		udpfd = socket(AF_INET, SOCK_DGRAM, 0);
		if (udpfd == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(udpfd > MAXSIZE)
		{
			printf("udpfd>4096\n");
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(udpfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//self
		self = (void*)obj[udpfd].self;
		memset(self, 0, sizeof(struct sockaddr_in));
		self->sin_family = AF_INET;
		self->sin_port = htons(port);
		self->sin_addr.s_addr = htons(INADDR_ANY);
		ret = bind(udpfd, (void*)self, sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(udpfd);
			return 0;
		}

		//done
		kqueue_add(udpfd);
		return udpfd;
	}

	//udp client
	if(_udp_ == type)
	{
		struct sockaddr_in* self;
		struct sockaddr_in* peer;

		//create
		fd = socket(AF_INET, SOCK_DGRAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}
		if(fd > MAXSIZE)
		{
			printf("fd>4096\n");
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//self
		self = (void*)obj[fd].self;
		memset(self, 0, sizeof(struct sockaddr_in));
		self->sin_family = AF_INET;
		self->sin_port = htons(2222);
		self->sin_addr.s_addr = htons(INADDR_ANY);
		ret = bind(fd, (void*)self, sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(fd);
			return 0;
		}

		//peer
		peer = (void*)obj[fd].peer;
		memset(peer, 0, sizeof(struct sockaddr_in));
		peer->sin_family = AF_INET;
		peer->sin_port = htons(port);
		peer->sin_addr.s_addr = inet_addr(addr);

		//done
		kqueue_add(fd);
		return udpfd;
	}

	//tcp server
	if(_TCP_ == type)
	{
		int ret;
		struct sockaddr_in* self;

		//create
		tcpfd = socket(AF_INET, SOCK_STREAM, 0);
		if (tcpfd == -1)
		{
			printf("error@socket\n");
			return 0;
		}
		if(tcpfd > MAXSIZE)
		{
			printf("tcpfd>4096\n");
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(tcpfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//self
		self = (void*)obj[tcpfd].self;
		memset(self, 0, sizeof(struct sockaddr_in));
		self->sin_family = AF_INET;
		self->sin_port = htons(port);
		self->sin_addr.s_addr = htons(INADDR_ANY);
		ret = bind(tcpfd, (void*)self, sizeof(struct sockaddr_in));
		if(ret == -1)
		{
			printf("error@bind\n");
			close(tcpfd);
			return 0;
		}

		//work
		listen(tcpfd, 5);

		//done
		kqueue_add(tcpfd);
		return tcpfd;
	}

	//tcp client
	if(_tcp_ == type)
	{
		int ret;
		struct sockaddr_in* peer;

		//create socket
		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd == -1)
		{
			printf("error%d@socket:%d\n",errno,fd);
			return 0;
		}

		//reuse
		ret = 1;
		ret = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
		if(ret<0)
		{
			printf("error@setsockopet\n");
			return 0;
		}

		//peer
		peer = (void*)obj[fd].peer;
		memset(peer, 0, sizeof(struct sockaddr_in));
		peer->sin_family = AF_INET;
		peer->sin_addr.s_addr = inet_addr(addr);
		peer->sin_port = htons(port);

		//work
		ret = connect(fd, (void*)peer, sizeof(struct sockaddr_in));
		if(ret < 0)
		{
			printf("connect error\n");
			return 0;
		}

		//done
		kqueue_add(fd);
		return fd;
	}

	printf("error@type\n");
	return 0;
}
void deletesocket(int num)
{
}
void createsocket(void* addr)
{
	struct sigaction sa;
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE, &sa, 0);

	obj = addr;
}
