#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libuser.h"




void initvertex(struct arena* w)
{
	struct texandobj* mod = w->mod;

//---------------------0--------------------------
	//vao
	glGenVertexArrays(1, &mod[0].vao);
	glBindVertexArray(mod[0].vao);

	//ibo
	glGenBuffers(1, &mod[0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[0].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------1--------------------------
	//vao
	glGenVertexArrays(1, &mod[1].vao);
	glBindVertexArray(mod[1].vao);

	//ibo
	glGenBuffers(1, &mod[1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[1].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[1].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------2--------------------------
	//vao
	glGenVertexArrays(1, &mod[2].vao);
	glBindVertexArray(mod[2].vao);

	//ibo
	glGenBuffers(1, &mod[2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[2].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[2].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------3--------------------------
	//vao
	glGenVertexArrays(1, &mod[3].vao);
	glBindVertexArray(mod[3].vao);

	//ibo
	glGenBuffers(1, &mod[3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[3].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[3].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);

/*
//-------------------------4--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[4].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);
*/


//---------------------4--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//ibo
	glGenBuffers(1, &mod[4].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[4].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[4].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------5--------------------------
	//vao
	glGenVertexArrays(1, &mod[5].vao);
	glBindVertexArray(mod[5].vao);

	//ibo
	glGenBuffers(1, &mod[5].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[5].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[5].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[5].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------6--------------------------
	//vao
	glGenVertexArrays(1, &mod[6].vao);
	glBindVertexArray(mod[6].vao);

	//ibo
	glGenBuffers(1, &mod[6].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[6].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[6].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[6].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------7--------------------------
	//vao
	glGenVertexArrays(1, &mod[7].vao);
	glBindVertexArray(mod[7].vao);

	//ibo
	glGenBuffers(1, &mod[7].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[7].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[7].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[7].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);

//-------------------------0x80--------------------------
//drawarray.point(vertex,colour)
	glGenVertexArrays(1, &mod[0x80].vao);
	glBindVertexArray(mod[0x80].vao);

	glGenBuffers(1, &mod[0x80].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x80].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x81--------------------------
	//drawelement.line(vertex,colour)
	glGenVertexArrays(1, &mod[0x81].vao);
	glBindVertexArray(mod[0x81].vao);

	glGenBuffers(1, &mod[0x81].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x81].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x81].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x81].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x82---------------------------
	//drawarray.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x82].vao);
	glBindVertexArray(mod[0x82].vao);

	glGenBuffers(1, &mod[0x82].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x82].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------0x83---------------------------
	//drawelement.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x83].vao);
	glBindVertexArray(mod[0x83].vao);

	glGenBuffers(1, &mod[0x83].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x83].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x83].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x83].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//------------------------0x84--------------------------
	glGenVertexArrays(1, &mod[0x84].vao);
	glBindVertexArray(mod[0x84].vao);

	glGenBuffers(1, &mod[0x84].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x84].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x85-------------------------
	glGenVertexArrays(1, &mod[0x85].vao);
	glBindVertexArray(mod[0x85].vao);

	glGenBuffers(1, &mod[0x85].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x85].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x85].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x85].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x86-------------------------
	glGenVertexArrays(1, &mod[0x86].vao);
	glBindVertexArray(mod[0x86].vao);

	glGenBuffers(1, &mod[0x86].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x86].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x86].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x86].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);
}