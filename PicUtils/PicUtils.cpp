// PicUtils.cpp : Defines the entry point for the console application.
//


#include <windows.h>

#ifdef INT32
#undef INT32
#endif // INT32

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include "jpeglib.h"

using namespace std;

#define DIB_HEADER_MARKER   ((WORD) ('M' << 8) | 'B')


void decode_example()
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
}

int main(int argc, char* argv[])
{

	char* filename = "D:\\my_first.jpg";
	ifstream ifs;
	ifs.open("D:\\finger.bmp", ios::binary|ios::in);
	if (ifs.is_open() == false)
	{
		return 1;
	}

	BITMAPFILEHEADER bfh;
	ifs.read((char*)&bfh, sizeof(BITMAPFILEHEADER));

	if (bfh.bfType != DIB_HEADER_MARKER)
	{
		return 1;
	}
	unsigned char* remain_data = (unsigned char*)malloc(bfh.bfSize);
	if (remain_data == NULL)
	{
		return 1;
	}
	memset(remain_data, 0x00, bfh.bfSize);
	ifs.read((char*)remain_data, bfh.bfSize - sizeof(BITMAPFILEHEADER));
	//TODO:Èô¶ÁÊ§°Ü£¬Ôò·µ»Ø99
	ifs.close();

	unsigned char* image_buffer = remain_data - sizeof(BITMAPFILEHEADER) + bfh.bfOffBits;

	struct jpeg_compress_struct cinfo;

	struct jpeg_error_mgr jerr;
	/* More stuff */
	FILE * outfile;		/* target file */
	JSAMPROW row_pointer[1];	/* pointer to JSAMPLE row[s] */
	int row_stride;		/* physical row width in image buffer */

	cinfo.err = jpeg_std_error(&jerr);
	/* Now we can initialize the JPEG compression object. */
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\n", filename);
		exit(1);
	}
	jpeg_stdio_dest(&cinfo, outfile);

	/* Step 3: set parameters for compression */

	/* First we supply a description of the input image.
	* Four fields of the cinfo struct must be filled in:
	*/
	cinfo.image_width = 328; 	/* image width and height, in pixels */
	cinfo.image_height = 356;
	cinfo.input_components = 1;		/* # of color components per pixel */
	cinfo.in_color_space = JCS_GRAYSCALE; 	/* colorspace of input image */
	/* Now use the library's routine to set default compression parameters.
	* (You must set at least cinfo.in_color_space before calling this,
	* since the defaults depend on the source color space.)
	*/
	jpeg_set_defaults(&cinfo);
	/* Now you can set any non-default parameters you wish to.
	* Here we just illustrate the use of quality (quantization table) scaling:
	*/
	jpeg_set_quality(&cinfo, 95, TRUE /* limit to baseline-JPEG values */);

	/* Step 4: Start compressor */

	/* TRUE ensures that we will write a complete interchange-JPEG file.
	* Pass TRUE unless you are very sure of what you're doing.
	*/
	jpeg_start_compress(&cinfo, TRUE);

	/* Step 5: while (scan lines remain to be written) */
	/*           jpeg_write_scanlines(...); */

	/* Here we use the library's state variable cinfo.next_scanline as the
	* loop counter, so that we don't have to keep track ourselves.
	* To keep things simple, we pass one scanline per call; you can pass
	* more if you wish, though.
	*/
	row_stride = 328;	/* JSAMPLEs per row in image_buffer */

	while (cinfo.next_scanline < cinfo.image_height) {
		/* jpeg_write_scanlines expects an array of pointers to scanlines.
		* Here the array is only one element long, but you could pass
		* more than one scanline at a time if that's more convenient.
		*/
		row_pointer[0] = & image_buffer[cinfo.next_scanline * row_stride];
		(void) jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	/* Step 6: Finish compression */

	jpeg_finish_compress(&cinfo);
	/* After finish_compress, we can close the output file. */
	fclose(outfile);

	/* Step 7: release JPEG compression object */

	/* This is an important step since it will release a good deal of memory. */
	jpeg_destroy_compress(&cinfo);

	return 0;
}

