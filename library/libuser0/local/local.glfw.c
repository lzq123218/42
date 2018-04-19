#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "arena.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define _drop_ hex32('d','r','o','p')




int lowlevel_input();
int fixarg(void* dst, void* src);
//
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);
//
void matrixmultiply_4(float*, float*);
double squareroot(double);




static struct arena* win;
static u8* dragdata[0x1000];
static int width = 512;
static int height = 512;
//
static GLuint font3dprogram;
static GLuint font2dprogram;
static GLuint simpleprogram;
static GLuint prettyprogram;
static GLuint directprogram;
static GLuint glsl2dprogram;
/*
static float camera[4] = {100.0f, -100.0f, 100.0f};
static float center[4] = {0.0f, 0.0f, 0.0f};
static float above[4] = {0.0f, 0.0f, 1.0f};
*/
static GLfloat modelmatrix[4*4] = {  
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
static GLfloat viewmatrix[4*4] = {  
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
static GLfloat projmatrix[4*4] = {  
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, -1.0f,
	0.0f, 0.0f, -0.2f, 0.0f
};
struct texandobj
{
	GLuint program;
	GLuint vao;

	GLuint tex;
	GLuint tlen;
	void* tbuf;

	GLuint ibo;
	GLuint ilen;
	void* ibuf;

	GLuint vbo;
	GLuint vlen;
	void* vbuf;
};
//0x00+0:	font0000
//0x00+1:	font4000
//0x00+2:	font8000
//0x00+3:	fontc000
//0x00+?:	tex?
//0x80+0:	????
//0x80+1:	point(drawarray)
//0x80+2:	line(drawarray)
//0x80+3:	trigon(drawarray)
//0x80+4:	line(drawelement)
//0x80+5:	trigon(drawelement)
static struct texandobj mod[256];




char glsl2dvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"vcolor = colour;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n"
};
char glsl2dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char simplevert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"uniform mat4 simplemvp;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"vcolor = colour;\n"
		"gl_Position = simplemvp * vec4(vertex,1.0);\n"
	"}\n"
};
char simplefrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char prettyvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec3 normal;\n"
	"uniform mat4 prettymvp;\n"
	"uniform mediump vec3 ambientcolor;\n"
	"uniform mediump vec3 lightcolor;\n"
	"uniform mediump vec3 lightposition;\n"
	"uniform mediump vec3 eyeposition;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 N = normalize(normal);\n"
		"mediump vec3 L = normalize(vec3(lightposition - vertex));\n"
		"mediump vec3 E = normalize(eyeposition-vertex);\n"
		"mediump vec3 R = reflect(-L, N);\n"
		"mediump float SN = max(dot(N, L), 0.0);\n"
		"mediump float RV = max(dot(R, E), 0.0);\n"
		"mediump vec3 ambient = ambientcolor;\n"
		"mediump vec3 diffuse = lightcolor * SN;\n"
		"mediump vec3 specular = vec3(0.0, 0.0, 0.0);\n"
		"if(SN>0.0)specular = lightcolor * pow(RV, 4.0);\n"
		"vcolor = colour*(ambient + diffuse + specular);\n"
		"gl_Position = prettymvp * vec4(vertex,1.0);\n"
	"}\n"
};
char prettyfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char font3dvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"uniform mat4 prettymvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = prettymvp * vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char font3dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex2d;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv).rrrr;\n"
	"}\n"
};
char font2dvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char font2dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex2d;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv).rrrr;\n"
	"}\n"
};
char directvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"uniform mat4 prettymvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = prettymvp * vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char directfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex2d;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv);\n"
	"}\n"
};
GLuint compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	if(!shader)return 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (GL_TRUE == compileStatus)return shader;

	GLint infoLogLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength)
	{
		char* infoLog = (char*)malloc(infoLogLength);
		if (infoLog)
		{
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			say("Could not compile shader %d:\n%s", type, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
	}
	return 0;
}
GLuint shaderprogram(void* v, void* f)
{
	GLuint vShader = compileShader(GL_VERTEX_SHADER, v);
	if (!vShader)return 0;

	GLuint fShader = compileShader(GL_FRAGMENT_SHADER, f);
	if (!fShader)return 0;

	//3.glsl program
	GLuint prog = glCreateProgram();
	if(0 == prog)
	{
		printf("ERROR : create program failed");
		exit(1);
	}

	glAttachShader(prog, vShader);
	glAttachShader(prog, fShader);
	glLinkProgram(prog);

	GLint linkStatus;
	glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
	if(GL_TRUE == linkStatus)return prog;

	printf("ERROR : link shader program failed");
	GLint logLen;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
	if(logLen > 0)
	{
		char *log = (char*)malloc(logLen);
		GLsizei written;
		glGetProgramInfoLog(prog, logLen, &written, log);
		printf("Program log :%s\n", log);
	}

	glDeleteProgram(prog);
	return 0;
}
void initshader()  
{  
	//1.check version
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor: %s\n", vendor);
	printf("GL Renderer: %s\n", renderer);
	printf("GL Version (string): %s\n", version);
	printf("GLSL Version: %s\n", glslVersion);
	printf("GL Version (integer): %x.%x\n", major, minor);

	font3dprogram = shaderprogram(font3dvert, font3dfrag);
	mod[0x00].program = font3dprogram;
	mod[0x01].program = font3dprogram;
	mod[0x02].program = font3dprogram;
	mod[0x03].program = font3dprogram;

	font2dprogram = shaderprogram(font2dvert, font2dfrag);
	mod[0x04].program = font2dprogram;
	mod[0x05].program = font2dprogram;
	mod[0x06].program = font2dprogram;
	mod[0x07].program = font2dprogram;
/*
	directprogram = shaderprogram(directvert, directfrag);
	mod[0x04].program = directprogram;
*/
	simpleprogram = shaderprogram(simplevert, simplefrag);
	mod[0x80].program = simpleprogram;
	mod[0x81].program = simpleprogram;

	prettyprogram = shaderprogram(prettyvert, prettyfrag);
	mod[0x82].program = prettyprogram;
	mod[0x83].program = prettyprogram;

	glsl2dprogram = shaderprogram(glsl2dvert, glsl2dfrag);
	mod[0x84].program = glsl2dprogram;
	mod[0x85].program = glsl2dprogram;
	mod[0x86].program = glsl2dprogram;
	mod[0x87].program = glsl2dprogram;
}
void initobject()  
{
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

/*
//-------------------------2d screen--------------------------
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
void inittexture()
{
	int j;
	u8* buf = win->buf;


//---------------------[0000,3fff]------------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0x80;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048, 
			(j&0x7f)<<4, (j&0xff80)>>3, j
		);
	}
	for(j=0x20;j<0x80;j++)
	{
		drawascii_alpha(buf, 2048, 2048,
			j<<4, 0, j
		);
	}

	glGenTextures(1, &(mod[0].tex));
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);


//---------------------[4000,7fff]----------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
		);
	}

	glGenTextures(1, &(mod[1].tex));
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);


//------------------[8000,bfff]---------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
		);
	}

	glGenTextures(1, &(mod[2].tex));
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);


//----------------[c000,ffff]--------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
		);
	}

	glGenTextures(1, &(mod[3].tex));
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RED, 2048, 2048, 0,
		GL_RED, GL_UNSIGNED_BYTE, buf
	);

/*
//------------------2d screen--------------------
	glGenTextures(1, &(mod[4].tex));
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, buf
	);
*/
	/*
	glGenFramebuffers(1, &shadowfb);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowfb);

	glGenTextures(1, &shadowtexture);
	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_DEPTH_COMPONENT, 1024, 1024, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		shadowtexture, 0
	);
	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("error@fbo\n");
	}
*/
}




void fixmodel()
{
	//matrix = movematrix * rotatematrix * scalematrix
}
void fixview()
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = win->camera.cx;
	float cy = win->camera.cy;
	float cz = win->camera.cz;

	//uvn.n = front
	float nx = win->camera.fx;
	float ny = win->camera.fy;
	float nz = win->camera.fz;
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = ny*1 - nz*0;
	float uy = nz*0 - nx*1;
	float uz = nx*0 - ny*0;
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above cross(right, front)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;
	norm = squareroot(vx*vx + vy*vy + vz*vz);
	vx /= norm;
	vy /= norm;
	vz /= norm;

	viewmatrix[0] = ux;
	viewmatrix[1] = vx;
	viewmatrix[2] = -nx;
	viewmatrix[3] = 0.0f;

	viewmatrix[4] = uy;
	viewmatrix[5] = vy;
	viewmatrix[6] = -ny;
	viewmatrix[7] = 0.0f;

	viewmatrix[8] = uz;
	viewmatrix[9] = vz;
	viewmatrix[10] = -nz;
	viewmatrix[11] = 0.0f;

	viewmatrix[12] = -cx*ux - cy*uy - cz*uz;
	viewmatrix[13] = -cx*vx - cy*vy - cz*vz;
	viewmatrix[14] = cx*nx + cy*ny + cz*nz;
	viewmatrix[15] = 1.0f;
}
void fixprojection()
{
/*
	cot45, 0, 0, 0,
	0, cot45, 0, 0,
	0, 0, (f+n)/(f-n), -1,
	0, 0, (2*f*n)/(f-n), 0
*/
	float w = (float)width;
	float h = (float)height;
	projmatrix[0] = h / w;
	glViewport(0, 0, width, height);
}
void fixmatrix(GLfloat* cameramvp)
{
	int x;

	fixmodel();
	fixview();
	fixprojection();

	for(x=0;x<16;x++)cameramvp[x] = modelmatrix[x];
	matrixmultiply_4(cameramvp, viewmatrix);
	matrixmultiply_4(cameramvp, projmatrix);
}
void fixlight()
{
	GLfloat light0[4] = {0.0f, 0.0f, 1000.0f};
	GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat cam[3];
	cam[0] = win->camera.cx;
	cam[1] = win->camera.cy;
	cam[2] = win->camera.cz;

	GLint ac = glGetUniformLocation(prettyprogram, "ambientcolor");
	glUniform3fv(ac, 1, ambientcolor);

	GLint dc = glGetUniformLocation(prettyprogram, "lightcolor");
	glUniform3fv(dc, 1, lightcolor);

	GLint dp = glGetUniformLocation(prettyprogram, "lightposition");
	glUniform3fv(dp, 1, light0);

	GLint ep = glGetUniformLocation(prettyprogram, "eyeposition");
	glUniform3fv(ep, 1, cam);
}
void callback_display()
{
	GLfloat cameramvp[4*4];
	fixmatrix(cameramvp);

	//set
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


//--------------------glsl2dprogram------------------
	//point,line
	glUseProgram(glsl2dprogram);

	glBindVertexArray(mod[0x84].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x84].vlen);

	glBindVertexArray(mod[0x85].vao);
	glDrawElements(GL_LINES, 2*mod[0x85].ilen, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(mod[0x86].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x86].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------simpleprogram------------------
	//point,line
	glUseProgram(simpleprogram);
	glUniformMatrix4fv(glGetUniformLocation(simpleprogram, "simplemvp"), 1, GL_FALSE, cameramvp);

	glBindVertexArray(mod[0x80].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x80].vlen);

	glBindVertexArray(mod[0x81].vao);
	glDrawElements(GL_LINES, 2*mod[0x81].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------prettyprogram------------------
	//stl,triangle
	glUseProgram(prettyprogram);
	glUniformMatrix4fv(glGetUniformLocation(prettyprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	fixlight();

	glBindVertexArray(mod[0x82].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[0x82].vlen);

	glBindVertexArray(mod[0x83].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x83].ilen, GL_UNSIGNED_SHORT, 0);


	//
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//--------------------font3dprogram------------------
	glUseProgram(font3dprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glBindVertexArray(mod[0].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glBindVertexArray(mod[1].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[1].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glBindVertexArray(mod[2].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[2].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glBindVertexArray(mod[3].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[3].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------font2dprogram------------------
	glUseProgram(font2dprogram);
	glUniform1i(glGetUniformLocation(font2dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glBindVertexArray(mod[4].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[4].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glBindVertexArray(mod[5].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[5].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glBindVertexArray(mod[6].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[6].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glBindVertexArray(mod[7].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[7].ilen, GL_UNSIGNED_SHORT, 0);

/*
//------------------directprogram-------------------
	glUseProgram(directprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glBindVertexArray(mod[4].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[4].vlen);
*/

	//
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void callback_update()
{
/*
//--------------------------------
	win->fmt = hex64('r','g','b','a','8','8','8','8');
	win->height = 1024;
	win->width = win->stride = 1024;
	actorread(win, 0, 0, 0);

	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, win->buf
	);
*/
//--------------------------------
	win->fmt = hex32('v','b','o',0);
	win->height = height;
	win->width = win->stride = width;
	actorread(win, 0, 0, 0);


//----------------------font3d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0].ilen, mod[0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0].vlen,mod[0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[1].ilen, mod[1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[1].vlen, mod[1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[2].ilen,mod[2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[2].vlen,mod[2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[3].ilen, mod[3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[3].vlen, mod[3].vbuf);


//----------------------font2d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[4].ilen, mod[4].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[4].vlen,mod[4].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[5].ilen, mod[5].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[5].vlen, mod[5].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[6].ilen,mod[6].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[6].vlen,mod[6].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[7].ilen, mod[7].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[7].vlen, mod[7].vbuf);
/*
	//2d screen
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[4].vlen, mod[4].vbuf);
*/

//----------------------3d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x80].vlen, mod[0x80].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x81].ilen, mod[0x81].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x81].vlen, mod[0x81].vbuf);

	//drawarray: trigon
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0x82].vlen, mod[0x82].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x83].ilen, mod[0x83].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 36*mod[0x83].vlen, mod[0x83].vbuf);


//----------------------2d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x84].vlen, mod[0x84].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x85].ilen, mod[0x85].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x85].vlen, mod[0x85].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x86].ilen, mod[0x86].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 24*mod[0x86].vlen, mod[0x86].vbuf);
}
void callback_keyboard(GLFWwindow* window, int key, int scan, int action, int mods)
{
	struct event e;
    printf("key=%x,scan=%x,action=%x,mods=%x\n", key, scan, action, mods);

	if(0 == action)return;
	if(0x100 == key)
	{
		e.why = 0x1b;
		e.what = hex32('k','b','d',0);
	}
	else if(0x101 == key)
	{
		e.why = 0xd;
		e.what = hex32('c','h','a','r');
	}
	else if((key >= 0x106)&&(key <= 0x109))
	{
		if(key == 0x109)e.why = 0x48;		//up
		else if(key == 0x108)e.why = 0x50;	//down
		else if(key == 0x107)e.why = 0x4b;	//left
		else if(key == 0x106)e.why = 0x4d;	//right
		e.what = hex32('k','b','d',0);
	}
	else if((key >= 0x122)&&(key <= 0x12d))
	{
		e.why = 0xf1 + key - 0x122;
		e.what = hex32('k','b','d',0);
	}
	else
	{
		e.why = key;
		e.what = hex32('c','h','a','r');
	}

	e.where = (u64)win;
	actorwrite(0, 0, win, 0, &e, 0x20);
	//eventwrite(why, what, where, 0);
}
void callback_mouse(GLFWwindow* window, int button, int action, int mods)
{
	u64 x,y,temp;
	struct event e;
	double xpos, ypos;

	glfwGetCursorPos(window, &xpos, &ypos);
	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	if(1 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2b70;
		e.where = (u64)win;
		actorwrite(0, 0, win, 0, &e, 0x20);
	}
	else if(0 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2d70;
		e.where = (u64)win;
		actorwrite(0, 0, win, 0, &e, 0x20);
	}
}
//void callback_move(int x,int y)
static void callback_move(GLFWwindow* window, double xpos, double ypos)
{
	u64 x,y,temp;
	struct event e;

	if(0 != glfwGetMouseButton(window, 0))temp = 'l';
	else if(0 != glfwGetMouseButton(window, 1))temp = 'r';
	else return;

	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	e.why = x + (y<<16) + (temp<<48);
	e.what = 0x4070;
	e.where = (u64)win;
	actorwrite(0, 0, win, 0, &e, 0x20);
}
void callback_scroll(GLFWwindow* window, double x, double y)
{
	struct event e;
	printf("%f,%f\n", x, y);

	e.where = (u64)win;
	e.what = 0x2b70;
	if(y > 0.0)	//wheel_up
	{
		e.why = ((u64)'f')<<48;
		actorwrite(0, 0, win, 0, &e, 0x20);
		//eventwrite(why, 0x2b70, where, 0);
	}
	else	//wheel_down
	{
		e.why = ((u64)'b')<<48;
		actorwrite(0, 0, win, 0, &e, 0x20);
		//eventwrite(why, 0x2b70, where, 0);
	}
}
void callback_drop(GLFWwindow* window, int count, const char** paths)
{
    int j,ret=0;
    for(j=0;j<count;j++)
	{
		//printf("%s\n", paths[j]);
		ret += snprintf((void*)dragdata+ret, 0x1000-ret, "%s\n", paths[j]);
	}

	eventwrite(0, _drop_, (u64)win, 0);
}
void callback_reshape(GLFWwindow* window, int w, int h)
{
	printf("%x,%x\n", w, h);
	width = w;
	height = h;
}
void* uievent(struct arena* this)
{
	//1.glfw
	GLFWwindow* fw = glfwCreateWindow(512, 512, "42", NULL, NULL);
	if(fw == NULL)
	{
		printf("error@glfwCreateWindow\n");
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(fw);

	//2.glew
	glewExperimental = 1;
	if(glewInit() != GLEW_OK)
	{
		printf("error@glewInit\n");
		return 0;
	}

	//3.init
	initshader();
	inittexture();
	initobject();

	//4.callbacks
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	//5.wait
	while(1)
	{
		if(glfwWindowShouldClose(fw) != 0)break;

		callback_update();
		callback_display();

		glfwSwapBuffers(fw);
		glfwPollEvents();
	}

	eventwrite(0,0,0,0);
	return 0;
}




int windowread(void* dc,void* df,void* sc,void* sf)
{
	//return snprintf(buf, 0x1000, "%s", dragdata);
}
void windowwrite(void* dc,void* df,void* sc,void* sf,u8* buf,int len)
{
}
void windowchange(int what)
{
}
void windowlist()
{
}
void windowstop()
{
}
void windowstart(struct arena* w)
{
	if(0 != win)
	{
		w->type = 0;
		w->fmt = 0;
		return;
	}

	w->type = hex32('w','i','n',0);
	w->fmt = hex32('v','b','o',0);
	w->irel = 0;
	w->orel = 0;

	w->mod = mod;
	w->buf = malloc(0x400000);

	w->width = 512;
	w->height = 512;
	w->depth = 512;
	w->stride = 512;

	//target
	w->target.cx = 0.0;
	w->target.cy = 0.0;
	w->target.cz = 0.0;

	w->target.rx = 1.0;
	w->target.ry = 0.0;
	w->target.rz = 0.0;

	w->target.fx = 0.0;
	w->target.fy = 1.0;
	w->target.fz = 0.0;

	w->target.ux = 0.0;
	w->target.uy = 0.0;
	w->target.uz = 1.0;

	//camera
	w->camera.cx = 512.0;
	w->camera.cy = -512.0;
	w->camera.cz = 512.0;

	w->camera.fx = (w->target.cx)-(w->camera.cx);
	w->camera.fy = (w->target.cy)-(w->camera.cy);
	w->camera.fz = (w->target.cz)-(w->camera.cz);

	w->camera.ux = 0.0;
	w->camera.uy = 0.0;
	w->camera.uz = 1.0;

//--------------------font3d-------------------
	//[0000,3fff]
	mod[0].vbuf = malloc(0x200000);
	mod[0].vlen = 0;
	mod[0].ibuf = malloc(0x100000);
	mod[0].ilen = 0;

	//[4000,7fff]
	mod[1].vbuf = malloc(0x200000);
	mod[1].vlen = 0;
	mod[1].ibuf = malloc(0x100000);
	mod[1].ilen = 0;

	//[8000,bfff]
	mod[2].vbuf = malloc(0x200000);
	mod[2].vlen = 0;
	mod[2].ibuf = malloc(0x100000);
	mod[2].ilen = 0;

	//[c000,ffff]
	mod[3].vbuf = malloc(0x200000);
	mod[3].vlen = 0;
	mod[3].ibuf = malloc(0x100000);
	mod[3].ilen = 0;

//--------------------font2d-------------------
	//[0000,3fff]
	mod[4].vbuf = malloc(0x200000);
	mod[4].vlen = 0;
	mod[4].ibuf = malloc(0x100000);
	mod[4].ilen = 0;

	//[4000,7fff]
	mod[5].vbuf = malloc(0x200000);
	mod[5].vlen = 0;
	mod[5].ibuf = malloc(0x100000);
	mod[5].ilen = 0;

	//[8000,bfff]
	mod[6].vbuf = malloc(0x200000);
	mod[6].vlen = 0;
	mod[6].ibuf = malloc(0x100000);
	mod[6].ilen = 0;

	//[c000,ffff]
	mod[7].vbuf = malloc(0x200000);
	mod[7].vlen = 0;
	mod[7].ibuf = malloc(0x100000);
	mod[7].ilen = 0;
/*
	//2d screen
	mod[4].vbuf = malloc(0x100000);
	mod[4].vlen = 0;
*/
//--------------------3d-------------------
	//drawarray.point
	mod[0x80].vbuf = malloc(0x100000);
	mod[0x80].vlen = 0;

	//drawelement.line
	mod[0x81].ibuf = malloc(0x100000);
	mod[0x81].ilen = 0;
	mod[0x81].vbuf = malloc(0x100000);
	mod[0x81].vlen = 0;

	//drawarray.trigon
	mod[0x82].vbuf = malloc(0x1000000);
	mod[0x82].vlen = 0;

	//drawelement.trigon
	mod[0x83].ibuf = malloc(0x100000);
	mod[0x83].ilen = 0;
	mod[0x83].vbuf = malloc(0x1000000);
	mod[0x83].vlen = 0;

//----------------------2d--------------------
	//drawarray.point
	mod[0x84].vbuf = malloc(0x100000);
	mod[0x84].vlen = 0;

	//drawelement.line
	mod[0x85].ibuf = malloc(0x100000);
	mod[0x85].ilen = 0;
	mod[0x85].vbuf = malloc(0x100000);
	mod[0x85].vlen = 0;

	//drawelement.trigon
	mod[0x86].ibuf = malloc(0x100000);
	mod[0x86].ilen = 0;
	mod[0x86].vbuf = malloc(0x100000);
	mod[0x86].vlen = 0;

	win = w;
	threadcreate(uievent, w);
}
void deletewindow()
{
}
void createwindow()
{
}




void freewindow()
{
	glfwTerminate();
}
void initwindow()
{
	win = 0;
	if(glfwInit() == 0)
	{
		printf("error@glfwInit\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);	//4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	//MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
