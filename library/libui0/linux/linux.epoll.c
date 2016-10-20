#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/types.h>
//
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
#define IPADDRESS "0.0.0.0"
#define PORT 2222
#define MAXSIZE 1024
//
void datastr2hexstr(u8* out, u8* in, int len);
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
u32 getrandom();
//
void printmemory(char*,int);
void say(char*,...);




//
static int width;
static int height;
//
static int listenfd=-1;
static int epollfd=-1;
static struct epoll_event epollevent[MAXSIZE];
//
static int clienttype[MAXSIZE];
static int websocket_last;
static int websocket_count;
//
static char* http_response = "HTTP/1.1 200 OK\r\nContent-type: text/html\r\n\r\n";
static int http_response_size;
static char http_context[0x100000];
static int http_context_size;
//
static char event_queue[0x100000];
static int event_count=0;
//
static unsigned char recvbuf[0x100000];
static unsigned char* sendbuf;
//
static u8 buf1[256];
static u8 buf2[256];
static u8 fixed_salt[256];
static u8 temp_salt[256];



//
static char* GET = 0;
static char* Connection = 0;
static char* Upgrade = 0;
static char* Sec_WebSocket_Key = 0;
static void explainstr(char* buf, int max)
{
	int flag;
	int linehead;

	GET = 0;
	Connection = 0;
	Upgrade = 0;
	Sec_WebSocket_Key = 0;

	linehead = 0;
	while(1)
	{
		if(strncmp(buf+linehead, "GET ", 4) == 0)GET = buf+linehead+4;
		else if(strncmp(buf+linehead, "Connection: ", 12) == 0)Connection = buf+linehead+12;
		else if(strncmp(buf+linehead, "Upgrade: ", 9) == 0)Upgrade = buf+linehead+9;
		else if(strncmp(buf+linehead, "Sec-WebSocket-Key: ", 19) == 0)Sec_WebSocket_Key = buf+linehead+19;

		//eat until next character
		flag=0;
		while(1)
		{
			if(buf[linehead] == 0)
			{
				//printf("[0x0@(%d,%d)]\n",linehead,max);
			}
			else if(buf[linehead] == 0xd)
			{
				flag=1;
				//printf("[0xd@(%d,%d)]\n",linehead,max);
			}
			else if(buf[linehead] == 0xa)
			{
				flag=1;
				//printf("[0xa@(%d,%d)]",linehead,max);
				printf("\n");
			}
			else
			{
				if(flag==0)
				{
					printf("%c", buf[linehead]);
				}
				else break;
			}

			linehead++;
			if(linehead >= max)break;
		}

		if(linehead >= max)break;
	}
	printf("GET@%llx,Connection@%llx,Upgrade@%llx,Sec-WebSocket-Key@%llx\n",
		(u64)GET,
		(u64)Connection,
		(u64)Upgrade,
		(u64)Sec_WebSocket_Key
	);
}
void epoll_del(u32 fd)
{
	int j;
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &ev);

	if(fd < MAXSIZE)
	{
		if(clienttype[fd] == 0x1f)
		{
			websocket_count--;
			if(fd == websocket_last)
			{
				for(j=fd-1;j>2;j--)
				{
					if(clienttype[j] == 0x1f)
					{
						websocket_last = j;
						break;
					}
				}
			}//less search
		}
		clienttype[fd] = 0;
	}
	close(fd);
	//printf("[3,%d]\n",websocket_last);
}
void epoll_add(u32 fd)
{
	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev);

	if(fd < MAXSIZE)clienttype[fd] = 1;
}








void windowwrite()
{
	int fd;
	int ret;

	u8 type;
	u8* base;

	u64 headlen;
	u64 bodylen;

	//no one
	if(websocket_count == 0)
	{
		printf("@windowwrite:count=0\n");
		return;
	}

	//type
	if(*(u32*)sendbuf == 0x6c6d7468)
	{
		type = 1;
		base = sendbuf + 0x1000;

		bodylen = strlen(base);
	}
	else
	{
		type = 2;
		base = sendbuf + (4*width);

		bodylen = (4*width) * (height-1);
	}

	//len
	if(bodylen<=125)
	{
		headlen = 2;
		*(base-2) = 0x80|type;
		*(base-1) = bodylen;
	}
	else if(bodylen<0xffff)
	{
		headlen = 4;
		*(base-4) = 0x80|type;
		*(base-3) = 126;
		*(base-2) = (bodylen>>8)&0xff;
		*(base-1) = bodylen&0xff;
	}
	else
	{
		headlen = 10;
		*(base-10)= 0x80|type;
		*(base-9) = 127;
		*(base-8) = (bodylen>>56)&0xff;
		*(base-7) = (bodylen>>48)&0xff;
		*(base-6) = (bodylen>>40)&0xff;
		*(base-5) = (bodylen>>32)&0xff;
		*(base-4) = (bodylen>>24)&0xff;
		*(base-3) = (bodylen>>16)&0xff;
		*(base-2) = (bodylen>>8)&0xff;
		*(base-1) = (bodylen)&0xff;
	}

	//write
	//printmemory(base-headlen, 0x200);
	for(fd=3;fd<MAXSIZE;fd++)
	{
		if(clienttype[fd] == 0x1f)
		{
			ret = write(fd, base-headlen, headlen+bodylen);
			if(ret > 0) printf("=>%d\n",fd);
			else printf("error@%d\n\n\n\n\n",fd);
		}
	}
}
void windowread()
{
}
void windowlist()
{
}
void windowchoose()
{
}








void serve_websocket(int fd, int nread)
{
	int i,j,k;
	int type,masked;
	unsigned char key[4];
	u64 len;

	//fd
	printf("[%d]",fd);

	//byte0.bit7
	if((recvbuf[0]&0x80)==0x80)printf("tail,");
	else printf("part,");

	//byte0.[3,0]
	k = recvbuf[0]&0xf;
	if(k==0)
	{
		type=0;
		printf("external,");
	}
	else if(k==1)
	{
		type=1;
		printf("text,");
	}
	else if(k==2)
	{
		type=2;
		printf("binary,");
	}
	else if(k==9)
	{
		printf("ping\n");
		return;
	}
	else if(k==0xa)
	{
		printf("pong\n");
		return;
	}
	else if(k==8)
	{
		printf("[%d]type8->close\n",fd);
		epoll_del(fd);
		return;
	}
	else
	{
		printf("[%d]known->close\n",fd);
		epoll_del(fd);
		return;
	}

	//byte1.bit7
	if( (recvbuf[1]>>7) == 1)
	{
		masked=1;
		printf("masked,");
	}
	else
	{
		masked=0;
		printf("unmasked,");
	}

	//
	k = recvbuf[1]&0x7f;
	if(k==127)
	{
		len	= ((u64)recvbuf[2]<<56)
			+ ((u64)recvbuf[3]<<48)
			+ ((u64)recvbuf[4]<<40)
			+ ((u64)recvbuf[5]<<32)
			+ ((u64)recvbuf[6]<<24)
			+ ((u64)recvbuf[7]<<16)
			+ ((u64)recvbuf[8]<<8)
			+ recvbuf[9];
		k = 10;
		printf("len=%llx,", len);
	}
	else if(k==126)
	{
		len	= (recvbuf[2]<<8)
			+ (recvbuf[3]);
		k = 4;
		printf("len=%llx,", len);
	}
	else
	{
		len = k;
		k = 2;
		printf("len=%llx,", len);
	}

	if(masked != 1)printf("\n");
	else
	{
		*(u32*)key = *(u32*)(recvbuf + k);
		j = k;
		k += 4;
		printf("key=%x\n",*(u32*)key);

		if(type==1)
		{
			recvbuf[0] &= 0x8f;
			recvbuf[1] &= 0x7f;
			for(i=0;i<len;i++)
			{
				event_queue[i] = recvbuf[i+k] ^ key[i%4];
				//printf("%c",recvbuf[j+i]);
			}
			event_queue[len] = 0;
			//printf("\n");
		}//type=ascii
	}//masked=1

reply:
	if(clienttype[fd]==0x10)
	{
		printf("[%d][stage0][client]%s\n", fd, event_queue);

		//
		i = snprintf(recvbuf+2, 20, "4....2..");
		recvbuf[0] = 0x81;
		recvbuf[1] = i;
		i = write(fd, recvbuf, 2+i);

		//
		printf("[%d][stage0][server]%s\n\n", fd, recvbuf+2);
		clienttype[fd] = 0x11;
	}

	else if(clienttype[fd]==0x11)
	{
		printf("[%d][stage1][client]username=%s\n", fd, event_queue);
		if(0)
		{
			i = snprintf(recvbuf+2, 20, "fail");
			recvbuf[0] = 0x81;
			recvbuf[1] = i;
			i = write(fd, recvbuf, 2+i);

			epoll_del(fd);
			printf("[%d]stage1 failed\n", fd);
			return;
		}

		else
		{
			//
			snprintf(fixed_salt, 256, "%08x", getrandom());
			snprintf( temp_salt, 256, "%08x", getrandom());
			i = snprintf(recvbuf+2, 20, "%s,%s", fixed_salt, temp_salt);
			recvbuf[0] = 0x81;
			recvbuf[1] = i;
			i = write(fd, recvbuf, 2+i);

			//
			printf("[%d][stage1][server]challenge=%s\n\n", fd, recvbuf+2);
			clienttype[fd] = 0x12;
		}
	}

	else if(clienttype[fd]==0x12)
	{
		printf("[%d][stage2][client]response=%s\n", fd, event_queue);

		//
		snprintf(buf2, 256, "%s%s", fixed_salt, "42");
		sha1sum(buf1, buf2, strlen(buf2));
		datastr2hexstr(buf2, buf1, 20);
		printf("	%s\n",buf2);

		snprintf(buf1, 256, "%s%s", temp_salt, buf2);
		sha1sum(buf2, buf1, strlen(buf1));
		datastr2hexstr(buf1, buf2, 20);
		printf("	%s\n",buf1);
/*
		if(strncmp(event_queue, buf1, 256) != 0)
		{
			epoll_del(fd);
			printf("[%d][stage2]incorrect\n", fd);
			return;
		}
*/
		//write
		i = snprintf(recvbuf+2, 20, "correct");
		recvbuf[0] = 0x81;
		recvbuf[1] = i;
		i = write(fd, recvbuf, 2+i);

		//count
		websocket_count++;
		if(fd > websocket_last)websocket_last = fd;

		//type
		printf("[%d][stage2][server]status=%s\n\n", fd, recvbuf+2);
		clienttype[fd] = 0x1f;

		//event
		*(u64*)(event_queue+0) = 0xabcdef;
		event_count = 1;
	}

	else if(clienttype[fd]==0x1f)
	{
		printf("[%d]:%s\n", fd, event_queue);

		len = *(u32*)event_queue;
		if(len == 0x2064626b)
		{
			len = atoi(event_queue+4);
			*(u64*)(event_queue+8) = len;
			*(u64*)(event_queue+0) = 0x64626b;
		}
		else if(len == 0x72616863)
		{
			len = atoi(event_queue+5);
			*(u64*)(event_queue+8) = len;
			*(u32*)(event_queue+4) = 0;
		}
		event_count = 1;
	}
}
void handshake_websocket(int fd)
{
	int j=0;
	while(1)
	{
		if(Sec_WebSocket_Key[j] == 0xa)
		{
			buf1[j] = 0;
			break;
		}
		if(Sec_WebSocket_Key[j] == 0xd)
		{
			buf1[j] = 0;
			break;
		}

		buf1[j] = Sec_WebSocket_Key[j];
		j++;
	}

	snprintf(buf2, 256,"%s%s", buf1, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	printf("%s\n",buf2);

	sha1sum( buf1, buf2, strlen(buf2) );
	for(j=0;j<20;j++)printf("%.2x",buf1[j]);
	printf("\n");

	base64_encode( buf2 ,buf1, 20 );
	printf("%s\n",buf2);

	snprintf(recvbuf, 0x10000,
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n\r\n",

		buf2
	);
	printf("%s",recvbuf);
	j = write(fd, recvbuf, strlen(recvbuf));

	//handshake
	printf("[%d]staging\n\n", fd);
	clienttype[fd] = 0x10;
}
void handshake_http(int fd)
{
	int ret;

	for(ret=0;ret<0x1000;ret++)
	{
		if(GET[ret] <= 0xd)goto byebye;
		if(GET[ret] == 0x20)
		{
			GET[ret] = 0;
			break;
		}
	}
	printf("%s\n",GET);

	//home page
	if( (GET[0]=='/')&&(GET[1]==0) )
	{
		snprintf(GET+1, 16, "42.html");
	}
	printf("%s\n",GET+1);

	//open,read,close
	ret = open(GET+1, O_RDONLY);
	if(ret <= 0)
	{
		printf("error@html\n");
		goto byebye;
	}
	http_context_size = read(ret, http_context, 0x100000);
	close(ret);

	//send
	ret = write(fd, http_response, strlen(http_response) );
	printf("writing http_response\n");
	ret = write(fd, http_context, http_context_size);
	printf("writing http_context\n");

byebye:
	epoll_del(fd);
	printf("[%d]done->close\n\n\n\n\n", fd);
}
static void handle_read(int fd)
{
	int type;
	int nread;

	//出错就滚，正常往下
	nread = read(fd, recvbuf, MAXSIZE);
	if(nread <= 0)
	{
		if (nread == -1)printf("[%d]read error\n", fd);
		else if (nread == 0)printf("[%d]fd closed\n", fd);

		epoll_del(fd);
		return;
	}
	recvbuf[nread] = 0;
	//printf("[%d]%d bytes\n", fd, nread);

	//这是个普通socket
	type = clienttype[fd];
	if(type == 1)
	{
		//普通socket，而且不是GET请求，丢弃
		explainstr(recvbuf, nread);
		if(GET == 0)return;

		//可能是http，websocket
		else
		{
			//这是个websocket请求
			if( (Upgrade != 0) && (Sec_WebSocket_Key != 0) )
			{
				handshake_websocket(fd);
			}

			//http请求根
			else if(GET != 0)
			{
				handshake_http(fd);
			}

			//http请求其他
			else
			{
				printf("[%d]ignore->close\n\n\n\n\n", fd);
				epoll_del(fd);
			}

			return;
		}
	}

	//websocket
	else if( (type >= 0x10) && (type <= 0x1f) )
	{
		serve_websocket(fd, nread);
		return;
	}

	//ssh?
	//vpn?
	//tunnel?
}
static void handle_accpet(int listenfd)
{
	int fd;
	struct sockaddr_in cliaddr;

	socklen_t cliaddrlen = sizeof(struct sockaddr_in);
	fd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddrlen);
	if (fd == -1)printf("accept error\n");

	//
	printf("[%d]accept@%s:%d\n",
		fd,
		inet_ntoa(cliaddr.sin_addr),
		cliaddr.sin_port
	);
	if(fd > MAXSIZE)
	{
		printf("[%d]MAXSIZE->close\n\n\n\n\n", fd);
		close(fd);
		return;
	}

	epoll_add(fd);
}
static void handle_events()
{
	int i;
	int fd;
	int num = epoll_wait(epollfd, epollevent, MAXSIZE, -1);

	for (i = 0;i < num;i++)
	{
		if (epollevent[i].events & EPOLLIN)
		{
			fd = epollevent[i].data.fd;

			if(fd == listenfd)
			{
				handle_accpet(listenfd);
			}
			else handle_read(fd);
		}
	}
}








void uievent(char* what, char* who, char* where, char* when)
{
	while(1)
	{
		if(event_count > 0)
		{
			*(u64*)who = *(u64*)event_queue;
			*(u64*)what = *(u64*)(event_queue + 8);
			event_count = 0;

			//printmemory(event_queue, 16);
			return;
		}

		handle_events();
	}
}
void windowstop()
{
	//only close all client
	//listenfd and epollfd unchanged
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
	//
	sendbuf = addr;

	//
	*(u64*)pixfmt = 0;
	snprintf(pixfmt, 5, "html");

	//
	width = x;

	//
	height = y;
}
void windowdelete(int num)
{
	int j;
	for(j=0;j<MAXSIZE;j++)
	{
		if(clienttype[j] != 0)epoll_del(j);
	}
	if(listenfd>0)epoll_del(listenfd);
	if(epollfd>0)close(epollfd);
}
int windowcreate()
{
	int ret;
	struct sockaddr_in servaddr;
	struct sigaction sa;




	//clean
	for(ret=0;ret<MAXSIZE;ret++)
	{
		clienttype[ret] = 0;
	}
	websocket_count = 0;
	websocket_last = 3;




	//socket
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, IPADDRESS, &servaddr.sin_addr);
	servaddr.sin_port = htons(PORT);

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1)
	{
		printf("error@socket\n");
		exit(-1);
	}

	ret = 1;
	ret = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &ret, 4);
	if(ret<0)
	{
		printf("error@setsockopet\n");
		exit(-1);
	}

	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
	{
		printf("error@bind\n");
		exit(-1);
	}
	listen(listenfd, 5);




	//epoll
	epollfd = epoll_create(MAXSIZE);
	epoll_add(listenfd);
	if(listenfd < MAXSIZE)clienttype[listenfd] = 0;




	//do not stop when SIGPIPE
	sa.sa_handler=SIG_IGN;
	sigaction(SIGPIPE,&sa,0);
}
