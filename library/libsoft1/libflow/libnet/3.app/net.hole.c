#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
void printmemory(void*, int);
void say(void*, ...);




struct object
{
	//[0x00,0x0f]
	u64 type_sock;  //raw, bt, udp, tcp?
	u64 stage0;
	u64 type_road;  //ssh, tls?
	u64 stage1;
	u64 type_app;   //http2, ws, rdp, vnc?
	u64 stage2;
	u64 type_data;  //html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 addr_src[0x20];
	u8 addr_dst[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};




#define HOLE 0x454c4f48
#define hole 0x656c6f68
int serve_hole_s(struct object* obj, int fd, u8* buf, int len)
{
	u64* aa;
	u64* bb;
	u8* q = obj[fd].addr_src;
	say(
		"[%d.%d.%d.%d:%d]%x,%x\n",
		q[4],q[5],q[6],q[7],
		(((int)q[2])<<8) + q[3],
		buf[0], buf[1], buf[2], buf[3]
	);
	if(buf[0] == '1')
	{
		aa = (void*)(obj[fd].data);
		bb = (void*)(obj[fd].addr_src);
		aa[1] = bb[0];

		if(aa[0] != 0)
		{
			writesocket(fd, aa, 0, 8);

			bb[0] = aa[0];
			writesocket(fd, aa+1, 0, 8);

			aa[0] = aa[1] = 0;
		}
	}
	else if(buf[0] == '0')
	{
		aa = (void*)(obj[fd].data);
		bb = (void*)(obj[fd].addr_src);
		aa[0] = bb[0];

		if(aa[1] != 0)
		{
			writesocket(fd, aa+1, 0, 8);

			bb[0] = aa[1];
			writesocket(fd, aa, 0, 8);

			aa[0] = aa[1] = 0;
		}
	}
	return 0;
}
int serve_hole_c(struct object* obj, int fd, u8* buf, int len)
{
	u64* aa;
	u64* bb;
	if((buf[0] == 2) && (buf[1] == 0) )
	{
		aa = (void*)(obj[fd].addr_src);
		bb = (void*)buf;

		say("target=%llx\n",bb[0]);
		aa[0] = bb[0];

		writesocket(fd, "hole\n", 0, 5);
		writesocket(fd, "hole\n", 0, 5);
	}

	printmemory(buf, len);
	return 0;
}
int serve_hole(struct object* obj, int fd, u8* buf, int len)
{
	u64 type = obj[fd].type_road;
	if(type == HOLE)serve_hole_s(obj, fd, buf, len);
	else if(type == hole)serve_hole_c(obj, fd, buf, len);
	return 0;
}
