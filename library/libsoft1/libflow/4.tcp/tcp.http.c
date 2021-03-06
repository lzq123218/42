#include "libsoft.h"
int findzero(void*);
int findhead(void*);
int findtail(void*);
int openreadclose(void* name, u64 off, void* mem, u64 len);
int openwriteclose(void* name, u64 off, void* mem, u64 len);
int nodetree_rootread(void*, void*, void*, int);
int nodetree_rootwrite(void*, void*, void*, int);
int wsserver_rootread(void*, void*, void*, void*, void* buf, int len);
int wsserver_rootwrite(void*, void*, void*, void*, void* buf, int len);
int tlsserver_write(void*, void*, void*, void*, void* buf, int len);




int httpclient_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k;
	void* dc;
	void* df;
	struct relation* orel;
	if(0 == ele)return 0;
	if(0 == obj)return 0;
	//say("%.*s\n", len, buf);

	if(0 == ele->stage1)
	{
		k = 0;
		for(j=0;j<=len;j++)
		{
			if((j>=len)|(0xa == buf[j])|(0xd == buf[j]))
			{
				say("%.*s\n", j-k, buf+k);
				if(j >= len)break;
				if((0xd == buf[k])&&(0xa == buf[k+1]))
				{
					buf += k+2;
					len -= k+2;
					break;
				}

				if(0xd == buf[j])j++;
				k = j+1;
			}
		}
	}
	ele->stage1 += 1;

	//if no o rel
	orel = ele->orel0;
	if(0 == orel)
	{
		printmemory(buf, len);
		return 0;
	}

	//send to o rel
	while(1)
	{
		if(0 == orel)break;

		dc = (void*)(orel->dstchip);
		df = (void*)(orel->dstfoot);
		if(_win_ == orel->dsttype)
		{
			arena_rootwrite(dc, df, ele, 0, buf, len);
		}
		else if(_act_ == orel->dsttype)
		{
			actor_rootwrite(dc, df, ele, 0, buf, len);
		}

		orel = samesrcnextdst(orel);
	}

	return 0;
}
int httpclient_read()
{
	return 0;
}
int httpclient_delete(struct element* ele)
{
	return 0;
}
int httpclient_create(struct element* ele, u8* url)
{
	int ret;
	void* obj;
	u8 buf[0x1000];

	obj = systemcreate(_tcp_, url);
	if(0 == obj)return 0;

	ret = mysnprintf(buf, 0x1000,
		"GET %s HTTP/1.1\r\n"
		"Host: %s\r\n"
		"\r\n",
		"/", url
	);

	ret = system_leafwrite(obj, 0, ele, 0, buf, ret);
	if(ret <= 0)return 0;

	ele->type = _http_;
	ele->stage1 = 0;
	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 1;
}




void httpserver_post(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	u8* POST)
{
	say("%.*s\n", len, buf);
}
void httpserver_get(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len,
	u8* GET)
{
	int ret;
	if(0 == ncmp(GET, "/favicon.ico", 12))return;

	//read data
	ele->obj = obj;
	len = nodetree_rootread(ele, sty, buf, len);
	if(len <= 0)return;
}
int httpserver_leafwrite(
	struct element* ele, void* sty,
	struct object* sc, void* sf,
	u8* buf, int len)
{
	u8 tmp[0x1000];
	if(0 == buf)
	{
		len = mysnprintf(tmp, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/html\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			len
		);
		buf = tmp;
	}

	system_leafwrite(ele->obj, 0, ele, sty, buf, len);
	return 0;
}
int httpserver_leafread(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int httpserver_rootwrite(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	printmemory(buf,len);
	return 0;
}
int httpserver_rootread(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	return 0;
}
int httpserver_delete(struct element* ele)
{
	return 0;
}
int httpserver_create(struct element* ele, u8* url)
{
	return 0;
}




int httpmaster_write(
	struct element* ele, void* sty,
	struct object* obj, void* pin,
	u8* buf, int len)
{
	int j,k,ret;
	u8* GET = 0;
	u8* POST = 0;
	u8* Upgrade = 0;
	u8* Connection = 0;
	struct element* e;

	//https
	if(0x16 == buf[0])
	{
		e = arterycreate(_Tls_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, obj, 0, _fd_);
			tlsserver_write(e, sty, obj, pin, buf, len);
		}
		return 0;
	}

	//parse
	k = 0;
	for(j=0;j<=len;j++)
	{
		if((j<len)&&(0xd != buf[j])&&(0xa != buf[j]))continue;

		//say("%.*s\n", j-k, buf+k);
		if(ncmp(buf+k, "Connection: ", 12) == 0)Connection = buf+k+12;
		else if(ncmp(buf+k, "Upgrade: ", 9) == 0)Upgrade = buf+k+9;
		else if(ncmp(buf+k, "POST ", 5) == 0)POST = buf+k+5;
		else if(ncmp(buf+k, "GET ", 4) == 0)GET = buf+k+4;

		if(0xa == buf[j+1])j++;
		k = j+1;
	}

	//websocket
	if((0 != Connection)&&(0 != Upgrade))
	{
		e = arterycreate(_Ws_, 0);
		if(e)
		{
			relationcreate(e, 0, _art_, obj, 0, _fd_);
			wsserver_rootwrite(e, sty, obj, pin, buf, len);
		}
		return 0;
	}

	if((0 == ele->orel0) | ((0 == GET)&&(0 == POST)))
	{
		//debug
		printmemory(buf, len);

		//response
		ret = mysnprintf(buf+len, 0x1000,
			"HTTP/1.1 200 OK\r\n"
			"Content-type: text/plain\r\n"
			"Content-Length: %d\r\n"
			"\r\n",
			len
		);

		//send response
		system_leafwrite(obj, pin, ele, sty, buf+len, ret);

		//send context
		system_leafwrite(obj, pin, ele, sty, buf, len);
	}
	else if(GET)httpserver_get(ele,sty, obj,pin, buf+len,0, GET);
	else if(POST)httpserver_post(ele,sty, obj,pin, buf+len,0, POST);

	//close or not
	if(0 != Connection)
	{
		if(0 == ncmp(Connection, "keep-alive", 10))return 0;
	}
	systemdelete(obj);
	return 0;
}
int httpmaster_read()
{
	return 0;
}
int httpmaster_delete(struct element* ele)
{
	return 0;
}
int httpmaster_create(struct element* ele, u8* url)
{
	int ret;
	void* obj = systemcreate(_TCP_, url);
	if(0 == obj)return 0;

	relationcreate(ele, 0, _art_, obj, 0, _fd_);
	return 0;
}