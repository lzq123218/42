#include <stdio.h>
#include <stdlib.h>
#include "libuser.h"

#ifdef __ANDROID__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>
#else
	#include <GL/glew.h>
#endif





void uploadvertex(struct gldst* dst, struct glsrc* src)
{
	void* buf;
	u32 w,h,fmt;

	//vao
	if(0 == dst->vao)glGenVertexArrays(1, &dst->vao);
	glBindVertexArray(dst->vao);

	//idx
	w = dst->ibo_deq;
	h = src->ibuf_enq;
	buf = (void*)(src->ibuf);
	if((w != h) && (0 != buf))
	{
		//say("@1: %d\n", dst->vao);
		w = src->ibuf_w;
		h = src->ibuf_h;
		if(0 == dst->ibo)
		{
			glGenBuffers(1, &dst->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, w*h, buf, GL_STATIC_DRAW);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, w*h, buf);
		}
	}

	//vbo
	w = dst->vbo_deq;
	h = src->vbuf_enq;
	buf = (void*)(src->vbuf);
	if((w != h) && (0 != buf))
	{
		//say("@2: %d\n", dst->vao);
		w = src->vbuf_w;
		h = src->vbuf_h;
		if(0 == dst->vbo)
		{
			glGenBuffers(1, &dst->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferData(GL_ARRAY_BUFFER, w*h, buf, GL_STATIC_DRAW);

			fmt = src->vbuf_fmt;
			if(vbuffmt_33 == fmt)
			{
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, w, (void*)12);
				glEnableVertexAttribArray(1);
			}
			else if(vbuffmt_333 == fmt)
			{
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, w, (void*)12);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, w, (void*)24);
				glEnableVertexAttribArray(2);
			}
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, w*h, buf);
		}
	}
}
void initvertex(struct arena* win)
{
	struct datapair* mod;
	struct glsrc* src;
	mod = win->mod;


//--------------------font3d-------------------
	//[0000,3fff]
	src = &mod[font3d0].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font3d0].dst, src);


	//[4000,7fff]
	src = &mod[font3d1].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font3d1].dst, src);


	//[8000,bfff]
	src = &mod[font3d2].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font3d2].dst, src);


	//[c000,ffff]
	src = &mod[font3d3].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font3d3].dst, src);


//--------------------font2d-------------------
	//[0000,3fff]
	src = &mod[font2d0].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font2d0].dst, src);


	//[4000,7fff]
	src = &mod[font2d1].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font2d1].dst, src);


	//[8000,bfff]
	src = &mod[font2d2].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font2d2].dst, src);


	//[c000,ffff]
	src = &mod[font2d3].src;

	src->vbuf = malloc(0x200000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x200000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[font2d3].dst, src);


//--------------------3d-------------------
	//drawarray.point3d
	src = &mod[point3d].src;

	src->vbuf = malloc(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->method = 'v';
	src->geometry = 1;

	src->vbuf_enq = 1;
	uploadvertex(&mod[point3d].dst, src);


	//drawelement.line3d
	src = &mod[line3d].src;

	src->vbuf = malloc(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x22;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0x100000/4;

	src->method = 'i';
	src->geometry = 2;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[line3d].dst, src);


	//drawelement.trigon3d
	src = &mod[trigon3d].src;

	src->vbuf = malloc(0x1000000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x100000/36;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[trigon3d].dst, src);


	//light3d
	src = &mod[light3d].src;

	src->vbuf = malloc(0x1000000);
	src->vbuf_fmt = vbuffmt_333;
	src->vbuf_w = 4*3*3;
	src->vbuf_h = 0x100000/36;


//----------------------2d--------------------
	//drawarray.point2d
	src = &mod[point2d].src;

	src->vbuf = malloc(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->method = 'v';
	src->geometry = 1;

	src->vbuf_enq = 1;
	uploadvertex(&mod[point2d].dst, src);


	//drawelement.line2d
	src = &mod[line2d].src;

	src->vbuf = malloc(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x22;
	src->ibuf_w = 2*2;
	src->ibuf_h = 0x100000/4;

	src->method = 'i';
	src->geometry = 2;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[line2d].dst, src);


	//drawelement.trigon2d
	src = &mod[trigon2d].src;

	src->vbuf = malloc(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;

	src->ibuf = malloc(0x100000);
	src->ibuf_fmt = 0x222;
	src->ibuf_w = 2*3;
	src->ibuf_h = 0x100000/6;

	src->method = 'i';
	src->geometry = 3;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
	uploadvertex(&mod[trigon2d].dst, src);


	//light2d
	src = &mod[light2d].src;

	src->vbuf = malloc(0x100000);
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 4*3*2;
	src->vbuf_h = 0x100000/24;
}
/*
void initvertex(struct arena* win)
{
	struct texandobj* mod = win->mod;

//---------------------font3d0--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d0].vao);
	glBindVertexArray(mod[font3d0].vao);

	//ibo
	glGenBuffers(1, &mod[font3d0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d0].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d0].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------font3d1--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d1].vao);
	glBindVertexArray(mod[font3d1].vao);

	//ibo
	glGenBuffers(1, &mod[font3d1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d1].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d1].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------font3d2--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d2].vao);
	glBindVertexArray(mod[font3d2].vao);

	//ibo
	glGenBuffers(1, &mod[font3d2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d2].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d2].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------font3d3--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d3].vao);
	glBindVertexArray(mod[font3d3].vao);

	//ibo
	glGenBuffers(1, &mod[font3d3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d3].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d3].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//---------------------font2d0--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d0].vao);
	glBindVertexArray(mod[font2d0].vao);

	//ibo
	glGenBuffers(1, &mod[font2d0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d0].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d0].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------font2d1--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d1].vao);
	glBindVertexArray(mod[font2d1].vao);

	//ibo
	glGenBuffers(1, &mod[font2d1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d1].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d1].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------font2d2--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d2].vao);
	glBindVertexArray(mod[font2d2].vao);

	//ibo
	glGenBuffers(1, &mod[font2d2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d2].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d2].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------font2d3--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d3].vao);
	glBindVertexArray(mod[font2d3].vao);

	//ibo
	glGenBuffers(1, &mod[font2d3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d3].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d3].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------vert3da--------------------------
//drawarray.point(vertex,colour)
	glGenVertexArrays(1, &mod[vert3da].vao);
	glBindVertexArray(mod[vert3da].vao);

	glGenBuffers(1, &mod[vert3da].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3da].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert3da].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------vert3db--------------------------
	//drawelement.line(vertex,colour)
	glGenVertexArrays(1, &mod[vert3db].vao);
	glBindVertexArray(mod[vert3db].vao);

	glGenBuffers(1, &mod[vert3db].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3db].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert3db].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert3db].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3db].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert3db].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------vert3dc---------------------------
	//drawelement.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[vert3dc].vao);
	glBindVertexArray(mod[vert3dc].vao);

	glGenBuffers(1, &mod[vert3dc].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3dc].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert3dc].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert3dc].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3dc].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, mod[vert3dc].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//------------------------vert2da--------------------------
	glGenVertexArrays(1, &mod[vert2da].vao);
	glBindVertexArray(mod[vert2da].vao);

	glGenBuffers(1, &mod[vert2da].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2da].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert2da].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------vert2db-------------------------
	glGenVertexArrays(1, &mod[vert2db].vao);
	glBindVertexArray(mod[vert2db].vao);

	glGenBuffers(1, &mod[vert2db].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2db].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert2db].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert2db].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2db].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert2db].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------vert2dc-------------------------
	glGenVertexArrays(1, &mod[vert2dc].vao);
	glBindVertexArray(mod[vert2dc].vao);

	glGenBuffers(1, &mod[vert2dc].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2dc].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert2dc].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert2dc].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2dc].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert2dc].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);
}
*/