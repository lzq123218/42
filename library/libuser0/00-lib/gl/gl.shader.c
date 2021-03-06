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
	char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 origcolor;\n"
		"in mediump vec2 texuv;\n"
		"uniform sampler2D tex0;\n"
		"out mediump vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(origcolor,1.0)*texture(tex0, texuv).aaaa;\n"
		"}\n"
	};
#else
	#include <GL/glew.h>
	char fontfrag[] = {
		GLSL_VERSION
		"in mediump vec3 origcolor;\n"
		"in mediump vec2 texuv;\n"
		"uniform sampler2D tex0;\n"
		"out mediump vec4 FragColor;\n"
		"void main()\n"
		"{\n"
			"FragColor = vec4(origcolor,1.0)*texture(tex0, texuv).rrrr;\n"
		"}\n"
	};
#endif




char glsl2dvert[] = {
	GLSL_VERSION
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
	GLSL_VERSION
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char simplevert[] = {
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"vcolor = colour;\n"
		"gl_Position = cammvp * vec4(vertex,1.0);\n"
	"}\n"
};
char simplefrag[] = {
	GLSL_VERSION
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char prettyvert[] = {
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec3 normal;\n"
	"uniform mat4 cammvp;\n"
	"uniform mediump vec3 eyepos;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 ambient = vec3(0.5, 0.5, 0.5);\n"
		"mediump vec3 lightcolor = vec3(0.5, 0.5, 0.5);\n"
		"mediump vec3 lightposition = vec3(0.0, 0.0, 1000.0);\n"
		"mediump vec3 N = normalize(normal);\n"
		"mediump vec3 L = normalize(vec3(lightposition - vertex));\n"
		"mediump vec3 E = normalize(eyepos-vertex);\n"
		"mediump vec3 R = reflect(-L, N);\n"
		"mediump float SN = max(dot(N, L), 0.0);\n"
		"mediump float RV = max(dot(R, E), 0.0);\n"
		"mediump vec3 diffuse = lightcolor * SN;\n"
		"mediump vec3 specular = vec3(0.0, 0.0, 0.0);\n"
		"if(SN>0.0)specular = lightcolor * pow(RV, 4.0);\n"
		"vcolor = colour*(ambient + diffuse + specular);\n"
		"gl_Position = cammvp * vec4(vertex,1.0);\n"
	"}\n"
};
char prettyfrag[] = {
	GLSL_VERSION
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char font3dvert[] = {
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = cammvp * vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char font2dvert[] = {
	GLSL_VERSION
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
char directvert[] = {
	GLSL_VERSION
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"uniform mat4 cammvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = cammvp * vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char directfrag[] = {
	GLSL_VERSION
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex0;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex0, texuv);\n"
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
void initshader(struct arena* win)  
{
	GLuint tmp;
	struct datapair* mod = win->mod;

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

	tmp = shaderprogram(font3dvert, fontfrag);
	mod[font3d0].dst.shader = tmp;
	mod[font3d1].dst.shader = tmp;
	mod[font3d2].dst.shader = tmp;
	mod[font3d3].dst.shader = tmp;

	tmp = shaderprogram(font2dvert, fontfrag);
	mod[font2d0].dst.shader = tmp;
	mod[font2d1].dst.shader = tmp;
	mod[font2d2].dst.shader = tmp;
	mod[font2d3].dst.shader = tmp;

	tmp = shaderprogram(simplevert, simplefrag);
	mod[point3d].dst.shader = tmp;
	mod[line3d].dst.shader = tmp;

	tmp = shaderprogram(prettyvert, prettyfrag);
	mod[trigon3d].dst.shader = tmp;

	tmp = shaderprogram(glsl2dvert, glsl2dfrag);
	mod[point2d].dst.shader = tmp;
	mod[line2d].dst.shader = tmp;
	mod[trigon2d].dst.shader = tmp;
}
