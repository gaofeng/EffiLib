// PicUtils.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include "jpeglib.h"


int main(int argc, char* argv[])
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE * infile;
	const char* filename = "cat_grey.jpg";
	JSAMPROW one_row;
	JDIMENSION lines;

	/* Initialize the JPEG decompression object with default error handling. */
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);

	if ((infile = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	jpeg_start_decompress(&cinfo);

	one_row = (JSAMPROW)malloc(sizeof(JSAMPLE) * cinfo.output_width);

	while (cinfo.output_scanline < cinfo.output_height)
	{
		lines = jpeg_read_scanlines(&cinfo, &one_row, 1);
	}

	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	printf("Hello World!\n");
	return 0;
}

