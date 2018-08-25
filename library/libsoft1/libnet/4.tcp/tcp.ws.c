#include "libsoft.h"
void sha1sum(u8* out, u8* in, int len);
void base64_encode(u8* out,u8* in, int len);
void datastr2hexstr(void* o, void* i, int len);
void decstr2data(void* i, void* o);
//
int findzero(void* p);
int findhead(void* p);
int findtail(void* p);
u8* findstr(void* src, int max, void* target, int tarlen);




int websocket_read(u8* buf, int len, u8* dst, int max)
{
//#define dbg say
#define dbg(fmt,...) do{}while(0)
	int i,j,k;
	int type,masked;
	u8 key[4];
	u64 count;

	//byte0.bit7
	if((buf[0]&0x80)==0x80)dbg("tail,");
	else dbg("part,");

	//byte0.[3,0]
	k = buf[0]&0xf;
	if(k==0)
	{
		type=0;
		dbg("external,");
	}
	else if(k==1)
	{
		type=1;
		dbg("text,");
	}
	else if(k==2)
	{
		type=2;
		dbg("binary,");
	}
	else if(k==9)
	{
		dbg("ping\n");
		return 0;
	}
	else if(k==0xa)
	{
		dbg("pong\n");
		return 0;
	}
	else if(k==8)
	{
		//dbg("[%d]type8->close\n",fd);
		//epoll_del(fd);
		return -1;
	}
	else
	{
		//dbg("[%d]known->close\n",fd);
		//epoll_del(fd);
		return -2;
	}

	//byte1.bit7
	if( (buf[1]>>7) == 1)
	{
		masked=1;
		dbg("masked,");
	}
	else
	{
		masked=0;
		dbg("unmasked,");
	}

	//
	k = buf[1]&0x7f;
	if(k==127)
	{
		count = ((u64)buf[2]<<56)
			+ ((u64)buf[3]<<48)
			+ ((u64)buf[4]<<40)
			+ ((u64)buf[5]<<32)
			+ ((u64)buf[6]<<24)
			+ ((u64)buf[7]<<16)
			+ ((u64)buf[8]<<8)
			+ buf[9];
		k = 10;
	}
	else if(k==126)
	{
		count = (buf[2]<<8) + (buf[3]);
		k = 4;
	}
	else
	{
		count = k;
		k = 2;
	}
	dbg("count=%llx,", count);

	if(masked != 1)dbg("\n");
	else
	{
		*(u32*)key = *(u32*)(buf + k);
		j = k;
		k += 4;
		dbg("key=%x\n",*(u32*)key);

		if(type==1)
		{
			dst[0] = buf[0]&0x8f;
			dst[1] = buf[1]&0x7f;
			for(i=0;i<count;i++)
			{
				dst[i] = buf[i+k] ^ key[i%4];
				//dbg("%c",buf[j+i]);
			}
			dst[count] = 0;
			//dbg("\n");
		}//type=ascii
	}//masked=1

	dbg("%s\n",dst);
	return count;
}
int websocket_write(u8* buf, int len, u8* dst, int max)
{
	int hl;

	//len
	dst[0] = 0x81;
	if(len<=125)
	{
		hl = 2;
		dst[1] = len;
	}
	else if(len<0xffff)
	{
		hl = 4;
		dst[1] = 126;
		dst[2] = (len>>8)&0xff;
		dst[3] = len&0xff;
	}
	else
	{
		hl = 10;
		dst[1] = 127;
		dst[2] = 0;
		dst[3] = 0;
		dst[4] = 0;
		dst[5] = 0;
		dst[6] = (len>>24)&0xff;
		dst[7] = (len>>16)&0xff;
		dst[8] = (len>>8)&0xff;
		dst[9] = (len)&0xff;
	}

	return hl;
}




int wsclient_read()
{
	return 0;
}
int wsclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int wsclient_delete(struct element* ele)
{
	//unlink
	//delete
	return 0;
}
int wsclient_create(struct element* ele, u8* url, u8* buf, int len)
{
	int ret;
	void* obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	ret = mysnprintf(buf, 0x1000,
		"GET %s HTTP/1.1\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Key: x3JJHMbDL1EzLkh9GBhXDw==\r\n"
		"\r\n",
		"/"
	);

	ret = systemwrite(obj, 0, ele, 0, buf, ret);
	if(ret <= 0)return 0;

	ele->type = hex32('w','s',0,0);
	ele->stage1 = 0;
	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 1;
}








int websocket_read_handshake(u8* buf, int len, u8* dst, int max)
{
	int j;
	u8 sha1buf[0x100];
	u8 base64buf[0x100];
	u8* Sec_WebSocket_Key;

	//
	Sec_WebSocket_Key = findstr(buf, len, "Sec-WebSocket-Key", 17);
	if(Sec_WebSocket_Key == 0)return 0;
	Sec_WebSocket_Key += 19;

	//在Sec_WebSocket_Key尾巴上添加一个固定的字符串
	j = findtail(Sec_WebSocket_Key);
	j += mysnprintf(Sec_WebSocket_Key + j, 256,
		"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	say("Sec_WebSocket_Key=%s\n", Sec_WebSocket_Key);

	//对这个字符串做一次sha1
	sha1sum(sha1buf, Sec_WebSocket_Key, j);
	say("sha1=");
	for(j=0;j<20;j++)say("%02x", sha1buf[j]);
	say("\n");

	//把sha1的结果以base64格式编码
	base64_encode(base64buf, sha1buf, 20);
	say("base64=%s\n", base64buf);

	//把base64的结果作为accept密钥
	return mysnprintf(dst, 256,
		"HTTP/1.1 101 Switching Protocols\r\n"
		"Upgrade: websocket\r\n"
		"Connection: Upgrade\r\n"
		"Sec-WebSocket-Accept: %s\r\n"
		"\r\n",
		base64buf
	);
}
int wsserver_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int ret;
	u8 tmp[0x1000];
	if(0 == ele->stage1)
	{
		ret = websocket_read_handshake(buf, len, tmp, 256);
		ret = systemwrite(obj, pin, ele, sty, tmp, ret);

		ele->stage1 = 1;
		return 0;
	}

	ret = websocket_read(buf, len, tmp, 0x1000);
	say("%.*s\n", ret, tmp);
	return 0;
}
int wsserver_read(
	struct element* ele, void* sty,
	struct object* obj, void* pin)
{
	return 0;
}
int wsserver_delete(struct element* ele)
{
	return 0;
}
int wsserver_create(struct element* ele)
{
	return 0;
}
/*
#define ws 0x7377
#define WS 0x5357
#define wss 0x737377
#define WSS 0x535357
int ws_client(struct element* ele, int fd, u8* buf, int len)
{
	return ws;
}
int ws_server(struct element* ele, int fd, u8* buf, int len)
{
	int ret = ele[fd].stage1;
	if(ret == 0)
	{
		int ret = websocket_read_handshake(fd, buf, len);
		if(ret <= 0)goto theend;

		ret = writesocket(fd, 0, buf, ret);
		if(ret <= 0)goto theend;

		ele[fd].stage1 = 1;
		goto theend;
	}

	ele[fd].stage1 = 2;
	ele[fd].len = len;
	ele[fd].buf = buf;

theend:
	return WS;
}
int wss_client(struct element* ele, int fd, u8* buf, int len)
{
	return wss;
}
int wss_server(struct element* ele, int fd, u8* buf, int len)
{
	return WSS;
}
*/