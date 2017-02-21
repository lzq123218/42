#define u64 unsigned long long
#define u32 unsigned long long
#include <jni.h>
#include <android/bitmap.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//
void characterstart(char* pixbuf, char* pixfmt, int width, int height);
void characterstop();
void characterwrite(u64* p);
void characterread();
//
void motion_explain(u64* p);
void network_explain(u64* p);
void sound_explain(u64* p);
void vision_explain(u64* p);
//
void say(char* , ...);
void birth();
void death();




//
static int pressed=0;
static int xxxx=0;
static int yyyy=0;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj, jobject bitmap)
{
	AndroidBitmapInfo  info;
	void*              pixels;
	int                ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		say("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
		say("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	//draw pixel
	characterread();

	//
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong value)
{
	u64 p[4] = {value, type, 0, 0};
	motion_explain(p);
	//say("(%llx,%llx),%d,%d\n",type,value,sizeof(type),sizeof(value));

	characterwrite(p);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Start(JNIEnv* env, jobject obj, jobject bitmap)
{
	AndroidBitmapInfo  info;
	int                ret;
	void*              pixels;
	say("start\n");

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		say("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}
	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
		say("Bitmap format is not RGBA_8888 !");
		return;
	}
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
		say("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	characterstart(pixels, "rgba8888", info.width, info.height-64);
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	say("JNI_OnLoad\n");
	birth();
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	say("JNI_OnUnLoad\n");
	death();
}




void uievent()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowcreate()
{
}
void windowdelete()
{
}
