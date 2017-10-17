#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




struct object
{
	//[0x00,0x0f]
	u64 type_sock;	//raw, bt, udp, tcp?
	u64 haha;
	union{
		void* first;
		u64 pad0;
	};
	union{
		void* last;
		u64 pad1;
	};

	//[0x20,0x3f]
	u64 type_road;	//http2, ws, rdp, vnc?
	u64 stage1;
	u64 type_data;	//html, rgb?
	u64 stage3;

	//[0x40,0x7f]
	u8 self[0x20];
	u8 peer[0x20];

	//[0x80,0xff]
	u8 data[0x80];
};