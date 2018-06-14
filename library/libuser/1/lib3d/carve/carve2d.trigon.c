#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau (PI*2)
#define acc 24
#define trigonv 0x86




void carvesolid2d_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[trigonv].ilen;
	int vlen = mod[trigonv].vlen;
	u16* ibuf = (mod[trigonv].ibuf) + (6*ilen);
	float* vbuf = (mod[trigonv].vbuf) + (24*vlen);
	mod[trigonv].ilen += 1;
	mod[trigonv].vlen += 3;

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = v1[0];
	vbuf[ 7] = v1[1];
	vbuf[ 8] = v1[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = v2[0];
	vbuf[13] = v2[1];
	vbuf[14] = v2[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carvesolid2d_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[trigonv].ilen;
	int vlen = mod[trigonv].vlen;
	u16* ibuf = (mod[trigonv].ibuf) + (6*ilen);
	float* vbuf = (mod[trigonv].vbuf) + (24*vlen);
	mod[trigonv].ilen += 2;
	mod[trigonv].vlen += 4;

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vc[0] + vr[0] - vf[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1];
	vbuf[ 8] = vc[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = vc[0] - vr[0] + vf[0];
	vbuf[13] = vc[1] - vr[1] + vf[1];
	vbuf[14] = vc[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = vc[0] + vr[0] + vf[0];
	vbuf[19] = vc[1] + vr[1] + vf[1];
	vbuf[20] = vc[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carvesolid2d_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	int a,b,j;
	float c,s;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	struct texandobj* mod = win->mod;
	int ilen = mod[trigonv].ilen;
	int vlen = mod[trigonv].vlen;
	u16* ibuf = (mod[trigonv].ibuf) + (6*ilen);
	float* vbuf = (mod[trigonv].vbuf) + (24*vlen);
	mod[trigonv].ilen += acc;
	mod[trigonv].vlen += acc+1;

	for(j=0;j<acc;j++)
	{
		a = j*6;
		b = j*3;

		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2];

		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;

		ibuf[b+0] = vlen + acc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%acc;
	}

	a = acc*6;
	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = 0.0;
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
}