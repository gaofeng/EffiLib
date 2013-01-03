// PicUtils.cpp : Defines the entry point for the console application.
//
#include "libcjpeg.h"

int main(int argc, char* argv[])
{
	cjpeg("D:\\20121103163143_24λλͼ.bmp", "D:\\output.jpg", 0, 0, 0, "90", 1, 2);
	return 0;
}

