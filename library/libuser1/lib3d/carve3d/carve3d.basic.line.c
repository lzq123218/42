#include "libuser.h"
#define acc 32
int line3d_vars(struct arena* win, int id, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->mod;
	struct glsrc* src = &mod[id].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (24*vlen);
	*ibuf = (src->ibuf) + (4*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carveline(struct arena* win, u32 rgb,
	vec3 va, vec3 vb)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 2, 1);

	vbuf[ 0] = va[0];
	vbuf[ 1] = va[1];
	vbuf[ 2] = va[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vb[0];
	vbuf[ 7] = vb[1];
	vbuf[ 8] = vb[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
}
void carveline_bezier(struct arena* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vt)
{
	int j;
	float t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, acc + 1, acc);

	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*va[0] + 2*t*(1.0-t)*vt[0] + t*t*vb[0];
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*va[1] + 2*t*(1.0-t)*vt[1] + t*t*vb[1];
		vbuf[6*j+2] = (1.0-t)*(1.0-t)*va[2] + 2*t*(1.0-t)*vt[2] + t*t*vb[2];

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		if(j >= acc)break;
		ibuf[2*j+0] = vlen + j;
		ibuf[2*j+1] = vlen + j+1;
	}
}
void carveline_special(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu,
	float sa, float da)
{
	int j;
	float s,t;
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, acc + 1, acc);

	for(j=0;j<=acc;j++)
	{
		s = j * da / acc;
		t = j * vr[0] / acc;

		vbuf[6*j + 0] = t * cosine(sa+s);
		vbuf[6*j + 1] = t * sine(sa+s);
		vbuf[6*j + 2] = vu[2] * j / acc;

		vbuf[6*j + 3] = rr;
		vbuf[6*j + 4] = gg;
		vbuf[6*j + 5] = bb;

		if(j >= acc)break;
		ibuf[2*j + 0] = vlen + j;
		ibuf[2*j + 1] = vlen + j+1;
	}
}




void carveline_yshape(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 4, 3);

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

	vbuf[18] = (v0[0]+v1[0]+v2[0])/3;
	vbuf[19] = (v0[1]+v1[1]+v2[1])/3;
	vbuf[20] = (v0[2]+v1[2]+v2[2])/3;
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+3;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void carveline_triangle(struct arena* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 3, 3);

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

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen;
	ibuf[5] = vlen+2;
}




void carveline_rect(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 4, 4);

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vc[0] + vr[0] - vf[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1];
	vbuf[ 8] = vc[2] + vr[2] - vf[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = vc[0] - vr[0] + vf[0];
	vbuf[13] = vc[1] - vr[1] + vf[1];
	vbuf[14] = vc[2] - vr[2] + vf[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = vc[0] + vr[0] + vf[0];
	vbuf[19] = vc[1] + vr[1] + vf[1];
	vbuf[20] = vc[2] + vr[2] + vf[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+3;
	ibuf[5] = vlen+2;
	ibuf[6] = vlen+0;
	ibuf[7] = vlen+2;
}
void carveline_hexagon(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float v[4];
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 6, 6);

	//0
	vbuf[ 0] = vc[0] + vr[0];
	vbuf[ 1] = vc[1] + vr[1];
	vbuf[ 2] = vc[2] + vr[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	//180
	vbuf[18] = vc[0] - vr[0];
	vbuf[19] = vc[1] - vr[1];
	vbuf[20] = vc[2] - vr[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	v[0] = vr[0];
	v[1] = vr[1];
	v[2] = vr[2];
	quaternion_operation(v, vu, PI/3);

	//60
	vbuf[ 6] = vc[0] + v[0];
	vbuf[ 7] = vc[1] + v[1];
	vbuf[ 8] = vc[2] + v[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	//240
	vbuf[24] = vc[0] - v[0];
	vbuf[25] = vc[1] - v[1];
	vbuf[26] = vc[2] - v[2];
	vbuf[27] = rr;
	vbuf[28] = gg;
	vbuf[29] = bb;

	v[0] -= vr[0];
	v[1] -= vr[1];
	v[2] -= vr[2];

	//120
	vbuf[12] = vc[0] + v[0];
	vbuf[13] = vc[1] + v[1];
	vbuf[14] = vc[2] + v[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	//300
	vbuf[30] = vc[0] - v[0];
	vbuf[31] = vc[1] - v[1];
	vbuf[32] = vc[2] - v[2];
	vbuf[33] = rr;
	vbuf[34] = gg;
	vbuf[35] = bb;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
	ibuf[6] = vlen+3;
	ibuf[7] = vlen+4;
	ibuf[8] = vlen+4;
	ibuf[9] = vlen+5;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+0;
}
void carveline_circle(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
#define lineacc (acc*2)
	int j;
	float q[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, lineacc, lineacc);

	for(j=0;j<lineacc;j++)
	{
		vbuf[6*j+0] = vr[0];
		vbuf[6*j+1] = vr[1];
		vbuf[6*j+2] = vr[2];
		quaternion_operation(&vbuf[6*j], vu, j*tau/lineacc);

		vbuf[6*j+0] += vc[0];
		vbuf[6*j+1] += vc[1];
		vbuf[6*j+2] += vc[2];

		vbuf[6*j+3] = rr;
		vbuf[6*j+4] = gg;
		vbuf[6*j+5] = bb;

		ibuf[2*j+0] = vlen+j;
		ibuf[2*j+1] = vlen+(j+1)%lineacc;
	}
}




void carveline_pyramid3()
{
}
void carveline_pyramid4()
{
}
void carveline_pyramid5()
{
}
void carveline_pyramid6()
{
}
void carveline_cone(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	int a,b,j,k;
	float s,t;
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, acc + 2, acc * 3);

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		a = j*6;

		vbuf[a+0] = vc[0] + r[0];
		vbuf[a+1] = vc[1] + r[1];
		vbuf[a+2] = vc[2] + r[2];

		vbuf[a+3] = rr;
		vbuf[a+4] = gg;
		vbuf[a+5] = bb;

		//bottom
		ibuf[a+0] = vlen+acc;
		ibuf[a+1] = vlen+j;

		//upper
		ibuf[a+2] = vlen+acc+1;
		ibuf[a+3] = vlen+j;

		//curve
		ibuf[a+4] = vlen+j;
		ibuf[a+5] = vlen+(j+1)%acc;
	}

	a = acc*6;

	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];
	vbuf[a+0] = rr;
	vbuf[a+1] = gg;
	vbuf[a+2] = bb;

	vbuf[a+3] = vc[0]+vu[0];
	vbuf[a+4] = vc[1]+vu[1];
	vbuf[a+5] = vc[2]+vu[2];
	vbuf[a+3] = rr;
	vbuf[a+4] = gg;
	vbuf[a+5] = bb;
}




void carveline_prism3()
{
}
void carveline_prism4(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 8, 12);

	vbuf[ 0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2] - vu[2];
	vbuf[ 3] = rr;
	vbuf[ 4] = gg;
	vbuf[ 5] = bb;

	vbuf[ 6] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[ 8] = vc[2] + vr[2] - vf[2] - vu[2];
	vbuf[ 9] = rr;
	vbuf[10] = gg;
	vbuf[11] = bb;

	vbuf[12] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[13] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[14] = vc[2] - vr[2] + vf[2] - vu[2];
	vbuf[15] = rr;
	vbuf[16] = gg;
	vbuf[17] = bb;

	vbuf[18] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[19] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[20] = vc[2] + vr[2] + vf[2] - vu[2];
	vbuf[21] = rr;
	vbuf[22] = gg;
	vbuf[23] = bb;

	vbuf[24] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[25] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[26] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[27] = rr;
	vbuf[28] = gg;
	vbuf[29] = bb;

	vbuf[30] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[31] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[32] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[33] = rr;
	vbuf[34] = gg;
	vbuf[35] = bb;

	vbuf[36] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[37] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[38] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[39] = rr;
	vbuf[40] = gg;
	vbuf[41] = bb;

	vbuf[42] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[43] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[44] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[45] = rr;
	vbuf[46] = gg;
	vbuf[47] = bb;

	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+0;
	ibuf[ 3] = vlen+2;
	ibuf[ 4] = vlen+3;
	ibuf[ 5] = vlen+1;
	ibuf[ 6] = vlen+3;
	ibuf[ 7] = vlen+2;

	ibuf[ 8] = vlen+0;
	ibuf[ 9] = vlen+4;
	ibuf[10] = vlen+1;
	ibuf[11] = vlen+5;
	ibuf[12] = vlen+2;
	ibuf[13] = vlen+6;
	ibuf[14] = vlen+3;
	ibuf[15] = vlen+7;

	ibuf[16] = vlen+4;
	ibuf[17] = vlen+5;
	ibuf[18] = vlen+4;
	ibuf[19] = vlen+6;
	ibuf[20] = vlen+7;
	ibuf[21] = vlen+5;
	ibuf[22] = vlen+7;
	ibuf[23] = vlen+6;
}
void carveline_prism5()
{
}
void carveline_prism6()
{
}
void carveline_cask()
{
}
void carveline_cylinder(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	int a,b,j,k;
	float s,t;
	float r[4];

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, acc * 2, acc * 3);

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		a = j*12;
		b = j*6;
		vbuf[a+ 0] = vc[0] - vu[0] + r[0];
		vbuf[a+ 1] = vc[1] - vu[1] + r[1];
		vbuf[a+ 2] = vc[2] - vu[2] + r[2];
		vbuf[a+ 3] = rr;
		vbuf[a+ 4] = gg;
		vbuf[a+ 5] = bb;

		vbuf[a+ 6] = vc[0] + vu[0] + r[0];
		vbuf[a+ 7] = vc[1] + vu[1] + r[1];
		vbuf[a+ 8] = vc[2] + vu[2] + r[2];
		vbuf[a+ 9] = rr;
		vbuf[a+10] = gg;
		vbuf[a+11] = bb;

		ibuf[a+0] = vlen + j*2;
		ibuf[a+1] = vlen + ((j+1)%acc)*2;

		ibuf[a+2] = vlen + 1 + j*2;
		ibuf[a+3] = vlen + 1 + ((j+1)%acc)*2;

		ibuf[a+4] = vlen + j*2;
		ibuf[a+5] = vlen + 1 + j*2;
	}
}




void carveline_tetrahedron()
{
}
void carveline_octahedron()
{
}
void carveline_dodecahedron(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 20, 30);

	for(j=0;j<20*6;j+=6)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}

	//(+-1, +-1, +-1)
	vbuf[  0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[  1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[  2] = vc[2]-vr[2]-vf[2]-vu[2];

	vbuf[  6] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[  7] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[  8] = vc[2]+vr[2]-vf[2]-vu[2];

	vbuf[ 12] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[ 13] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[ 14] = vc[2]-vr[2]+vf[2]-vu[2];

	vbuf[ 18] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[ 19] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[ 20] = vc[2]+vr[2]+vf[2]-vu[2];

	vbuf[ 24] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[ 25] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[ 26] = vc[2]-vr[2]-vf[2]+vu[2];

	vbuf[ 30] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[ 31] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[ 32] = vc[2]+vr[2]-vf[2]+vu[2];

	vbuf[ 36] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[ 37] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[ 38] = vc[2]-vr[2]+vf[2]+vu[2];

	vbuf[ 42] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[ 43] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[ 44] = vc[2]+vr[2]+vf[2]+vu[2];

	//(0, +-a, +-b)
	vbuf[ 48] = vc[0] - a*vf[0] - b*vu[0];
	vbuf[ 49] = vc[1] - a*vf[1] - b*vu[1];
	vbuf[ 50] = vc[2] - a*vf[2] - b*vu[2];

	vbuf[ 54] = vc[0] + a*vf[0] - b*vu[0];
	vbuf[ 55] = vc[1] + a*vf[1] - b*vu[1];
	vbuf[ 56] = vc[2] + a*vf[2] - b*vu[2];

	vbuf[ 60] = vc[0] - a*vf[0] + b*vu[0];
	vbuf[ 61] = vc[1] - a*vf[1] + b*vu[1];
	vbuf[ 62] = vc[2] - a*vf[2] + b*vu[2];

	vbuf[ 66] = vc[0] + a*vf[0] + b*vu[0];
	vbuf[ 67] = vc[1] + a*vf[1] + b*vu[1];
	vbuf[ 68] = vc[2] + a*vf[2] + b*vu[2];

	//(+-b, 0, +-a)
	vbuf[ 72] = vc[0] - b*vr[0] - a*vu[0];
	vbuf[ 73] = vc[1] - b*vr[1] - a*vu[1];
	vbuf[ 74] = vc[2] - b*vr[2] - a*vu[2];

	vbuf[ 78] = vc[0] + b*vr[0] - a*vu[0];
	vbuf[ 79] = vc[1] + b*vr[1] - a*vu[1];
	vbuf[ 80] = vc[2] + b*vr[2] - a*vu[2];

	vbuf[ 84] = vc[0] - b*vr[0] + a*vu[0];
	vbuf[ 85] = vc[1] - b*vr[1] + a*vu[1];
	vbuf[ 86] = vc[2] - b*vr[2] + a*vu[2];

	vbuf[ 90] = vc[0] + b*vr[0] + a*vu[0];
	vbuf[ 91] = vc[1] + b*vr[1] + a*vu[1];
	vbuf[ 92] = vc[2] + b*vr[2] + a*vu[2];

	//(+-a, +-b, 0)
	vbuf[ 96] = vc[0] - a*vr[0] - b*vf[0];
	vbuf[ 97] = vc[1] - a*vr[1] - b*vf[1];
	vbuf[ 98] = vc[2] - a*vr[2] - b*vf[2];

	vbuf[102] = vc[0] + a*vr[0] - b*vf[0];
	vbuf[103] = vc[1] + a*vr[1] - b*vf[1];
	vbuf[104] = vc[2] + a*vr[2] - b*vf[2];

	vbuf[108] = vc[0] - a*vr[0] + b*vf[0];
	vbuf[109] = vc[1] - a*vr[1] + b*vf[1];
	vbuf[110] = vc[2] - a*vr[2] + b*vf[2];

	vbuf[114] = vc[0] + a*vr[0] + b*vf[0];
	vbuf[115] = vc[1] + a*vr[1] + b*vf[1];
	vbuf[116] = vc[2] + a*vr[2] + b*vf[2];

	//front
	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+8;
	ibuf[ 2] = vlen+1;
	ibuf[ 3] = vlen+8;
	ibuf[ 4] = vlen+8;
	ibuf[ 5] = vlen+10;
	ibuf[ 6] = vlen+4;
	ibuf[ 7] = vlen+10;
	ibuf[ 8] = vlen+5;
	ibuf[ 9] = vlen+10;

	//back
	ibuf[10] = vlen+2;
	ibuf[11] = vlen+9;
	ibuf[12] = vlen+3;
	ibuf[13] = vlen+9;
	ibuf[14] = vlen+9;
	ibuf[15] = vlen+11;
	ibuf[16] = vlen+6;
	ibuf[17] = vlen+11;
	ibuf[18] = vlen+7;
	ibuf[19] = vlen+11;

	//bottom
	ibuf[20] = vlen+0;
	ibuf[21] = vlen+12;
	ibuf[22] = vlen+2;
	ibuf[23] = vlen+12;
	ibuf[24] = vlen+12;
	ibuf[25] = vlen+13;
	ibuf[26] = vlen+1;
	ibuf[27] = vlen+13;
	ibuf[28] = vlen+3;
	ibuf[29] = vlen+13;

	//upper
	ibuf[30] = vlen+4;
	ibuf[31] = vlen+14;
	ibuf[32] = vlen+6;
	ibuf[33] = vlen+14;
	ibuf[34] = vlen+14;
	ibuf[35] = vlen+15;
	ibuf[36] = vlen+5;
	ibuf[37] = vlen+15;
	ibuf[38] = vlen+7;
	ibuf[39] = vlen+15;

	//left
	ibuf[40] = vlen+0;
	ibuf[41] = vlen+16;
	ibuf[42] = vlen+4;
	ibuf[43] = vlen+16;
	ibuf[44] = vlen+16;
	ibuf[45] = vlen+18;
	ibuf[46] = vlen+2;
	ibuf[47] = vlen+18;
	ibuf[48] = vlen+6;
	ibuf[49] = vlen+18;

	//right
	ibuf[50] = vlen+1;
	ibuf[51] = vlen+17;
	ibuf[52] = vlen+5;
	ibuf[53] = vlen+17;
	ibuf[54] = vlen+17;
	ibuf[55] = vlen+19;
	ibuf[56] = vlen+3;
	ibuf[57] = vlen+19;
	ibuf[58] = vlen+7;
	ibuf[59] = vlen+19;
}
void carveline_icosahedron(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, 12, 30);

	for(j=0;j<12*6;j++)
	{
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}

	//(+-m, 0, +-n)
	vbuf[ 0] = vc[0] - m;
	vbuf[ 1] = vc[1];
	vbuf[ 2] = vc[2] - n;

	vbuf[ 6] = vc[0] + m;
	vbuf[ 7] = vc[1];
	vbuf[ 8] = vc[2] - n;

	vbuf[12] = vc[0] - m;
	vbuf[13] = vc[1];
	vbuf[14] = vc[2] + n;

	vbuf[18] = vc[0] + m;
	vbuf[19] = vc[1];
	vbuf[20] = vc[2] + n;

	//(0, +-n, +-m)
	vbuf[24] = vc[0];
	vbuf[25] = vc[1] - n;
	vbuf[26] = vc[2] - m;

	vbuf[30] = vc[0];
	vbuf[31] = vc[1] + n;
	vbuf[32] = vc[2] - m;

	vbuf[36] = vc[0];
	vbuf[37] = vc[1] - n;
	vbuf[38] = vc[2] + m;

	vbuf[42] = vc[0];
	vbuf[43] = vc[1] + n;
	vbuf[44] = vc[2] + m;

	//(+-n, +-m, 0)
	vbuf[48] = vc[0] - n;
	vbuf[49] = vc[1] - m;
	vbuf[50] = vc[2];

	vbuf[54] = vc[0] + n;
	vbuf[55] = vc[1] - m;
	vbuf[56] = vc[2];

	vbuf[60] = vc[0] - n;
	vbuf[61] = vc[1] + m;
	vbuf[62] = vc[2];

	vbuf[66] = vc[0] + n;
	vbuf[67] = vc[1] + m;
	vbuf[68] = vc[2];

	ibuf[ 0] = vlen+2;
	ibuf[ 1] = vlen+3;
	ibuf[ 2] = vlen+3;
	ibuf[ 3] = vlen+7;
	ibuf[ 4] = vlen+2;
	ibuf[ 5] = vlen+7;
	ibuf[ 6] = vlen+7;
	ibuf[ 7] = vlen+11;
	ibuf[ 8] = vlen+3;
	ibuf[ 9] = vlen+11;

	ibuf[10] = vlen+3;
	ibuf[11] = vlen+6;
	ibuf[12] = vlen+3;
	ibuf[13] = vlen+9;
	ibuf[14] = vlen+6;
	ibuf[15] = vlen+9;
	ibuf[16] = vlen+6;
	ibuf[17] = vlen+2;
	ibuf[18] = vlen+9;
	ibuf[19] = vlen+11;

	ibuf[20] = vlen+0;
	ibuf[21] = vlen+1;
	ibuf[22] = vlen+0;
	ibuf[23] = vlen+4;
	ibuf[24] = vlen+0;
	ibuf[25] = vlen+5;

	ibuf[26] = vlen+0;
	ibuf[27] = vlen+8;
	ibuf[28] = vlen+0;
	ibuf[29] = vlen+10;
	ibuf[30] = vlen+1;
	ibuf[31] = vlen+4;

	ibuf[32] = vlen+4;
	ibuf[33] = vlen+8;
	ibuf[34] = vlen+1;
	ibuf[35] = vlen+5;
	ibuf[36] = vlen+5;
	ibuf[37] = vlen+10;
	ibuf[38] = vlen+8;
	ibuf[39] = vlen+10;

	ibuf[40] = vlen+1;
	ibuf[41] = vlen+11;
	ibuf[42] = vlen+5;
	ibuf[43] = vlen+7;
	ibuf[44] = vlen+10;
	ibuf[45] = vlen+2;
	ibuf[46] = vlen+8;
	ibuf[47] = vlen+6;
	ibuf[48] = vlen+4;
	ibuf[49] = vlen+9;
	ibuf[50] = vlen+1;
	ibuf[51] = vlen+9;
	ibuf[52] = vlen+5;
	ibuf[53] = vlen+11;
	ibuf[54] = vlen+10;
	ibuf[55] = vlen+7;
	ibuf[56] = vlen+8;
	ibuf[57] = vlen+2;
	ibuf[58] = vlen+4;
	ibuf[59] = vlen+6;
}
void carveline_sphere(struct arena* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
#define accx (acc)
#define accy (acc|0x1)
	int j,k,a,b;
	float c,s;
	vec3 tc, tr, tf;

	float bb = (float)(rgb&0xff) / 256.0;
	float gg = (float)((rgb>>8)&0xff) / 256.0;
	float rr = (float)((rgb>>16)&0xff) / 256.0;

	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, line3d, &vbuf, &ibuf, accx*accy+2, accx*accy+accx*(accy+1));

	for(k=0;k<accy;k++)
	{
		s = (2*k-accy+1)*PI/(2*accy+2);
		c = cosine(s);
		s = sine(s);

		tc[0] = vc[0] + vu[0]*s;
		tc[1] = vc[1] + vu[1]*s;
		tc[2] = vc[2] + vu[2]*s;
		tr[0] = vr[0]*c;
		tr[1] = vr[1]*c;
		tr[2] = vr[2]*c;
		tf[0] = vf[0]*c;
		tf[1] = vf[1]*c;
		tf[2] = vf[2]*c;

		for(j=0;j<accx;j++)
		{
			s = j*tau/accx;
			c = cosine(s);
			s = sine(s);

			a = (k*accx + j)*6;
			b = (k*accx + j)*2;

			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;

			vbuf[a+3] = rr;
			vbuf[a+4] = gg;
			vbuf[a+5] = bb;

			ibuf[b+0] = vlen+(k*accx)+j;
			ibuf[b+1] = vlen+(k*accx)+(j+1)%accx;
		}
	}

	a = accx*accy*6;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = rr;
	vbuf[a+ 4] = gg;
	vbuf[a+ 5] = bb;

	vbuf[a+ 6] = vc[0]+vu[0];
	vbuf[a+ 7] = vc[1]+vu[1];
	vbuf[a+ 8] = vc[2]+vu[2];
	vbuf[a+ 9] = rr;
	vbuf[a+10] = gg;
	vbuf[a+11] = bb;


	for(k=0;k<accx;k++)
	{
		a = accy*accx*2 + (accy-1)*k*2;
		for(j=0;j<accy-1;j++)
		{
			ibuf[a + 2*j + 0] = vlen+k+(j*accx);
			ibuf[a + 2*j + 1] = vlen+k+(j*accx)+accx;
		}
	}

	a = accy*accx*2 + (accy-1)*accx*2;
	for(j=0;j<accx;j++)
	{
		ibuf[a + 4*j + 0] = vlen+accx*accy;
		ibuf[a + 4*j + 1] = vlen+j;
		ibuf[a + 4*j + 2] = vlen+accx*accy+1;
		ibuf[a + 4*j + 3] = vlen+accx*(accy-1)+j;
	}
}