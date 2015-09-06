/**
 *      ___           ___           ___           ___
 *     /\  \         /\  \         /\  \         /\__\
 *    /::\  \       /::\  \       /::\  \       /:/  /
 *   /:/\:\  \     /:/\ \  \     /:/\:\  \     /:/  /
 *  /::\~\:\  \   _\:\~\ \  \   /::\~\:\  \   /:/  /
 * /:/\:\ \:\__\ /\ \:\ \ \__\ /:/\:\ \:\__\ /:/__/
 * \/__\:\/:/  / \:\ \:\ \/__/ \/_|::\/:/  / \:\  \
 *      \::/  /   \:\ \:\__\      |:|::/  /   \:\  \
 *      /:/  /     \:\/:/  /      |:|\/__/     \:\  \
 *     /:/  /       \::/  /       |:|  |        \:\__\
 *     \/__/         \/__/         \|__|         \/__/
 * -----------------------------------------------------
 * ----------> Autonomous Space Robotics Lab <----------
 * -----------------------------------------------------
 * /name    PNGImage
 * /brief   Implementation file for wrapper around LibPNG. Used for writing rendering to PNG file.
 * /author  Erik E. Beerepoot
 */

#include "GenericTypes.hpp"
#include "PNGImage.hpp"


 
//STL
#include <new>
#include <cstring>
#include <iostream>
 
 //libc
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <png.h>

 /**
  * /name Bitmap
  * /brief Construct for base class. Takes care of bitmap setup.
  */
Bitmap::Bitmap(std::string filePath,int imgHeight,int imgWidth){
	_bitmap.imageData = NULL;
	_bitmap.width = imgWidth;
	_bitmap.height = imgHeight;
	_filePath = filePath;
	
	try {
		AllocateBitmap(_bitmap);
	} catch (std::bad_alloc &ba) {
		//try some other way?
		throw ba;
	}
}

Bitmap::~Bitmap(){
	DeallocateBitmap(_bitmap);
}

/**
 * /name AllocateBitmap
 * /brief Allocates the memory for the bitmap pixel data.
 * /notes Throws an std::bad_alloc exception when out of memory.
 */
int Bitmap::AllocateBitmap(bitmap_t& bitmap){
	bitmap.imageData = new pixel_t[bitmap.height*bitmap.width];
	return SUCCESS;
}

/**
 * /name DeallocateBitmap
 * /brief Deallocates the memory for the bitmap pixel data.
 */
int Bitmap::DeallocateBitmap(bitmap_t& bitmap){
	delete [] bitmap.imageData;
	bitmap.imageData = NULL;
	return SUCCESS;
}

/**
 * /name ImageData
 * /brief Returns pointer to the image data array
 */
pixel_t * Bitmap::GetImageData(){
	return _bitmap.imageData;
}

/**
 * /name SetImageData
 * /brief Sets the contents of the bitmap to "newImageData"
 */
void Bitmap::SetImageData(pixel_t *newImageData){
}

/**
 * /name SetPixel
 * /brief Sets pixel at (x,y) to pix.
 */
void Bitmap::SetPixel(int x,int y, pixel_t pix){
	_bitmap(x,y) = pix;
}
/**
 * /name WriteImage
 * /brief Commits the bitmap to disk by writing it to a PNG file
 */
int PNGImage::Write(){
	if(_bitmap.imageData==NULL) return ERROR;
	
	int status = ERROR;
	int pixel_size = 3;
    int depth = 8;
    
	png_byte ** row_pointers = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	size_t x, y;
	FILE * fp;
	
    fp = fopen (_filePath.c_str(), "wb");
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
	
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    
    /* Set up error handling. */
    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    
    /* Set image attributes. */
    png_set_IHDR (png_ptr,
                  info_ptr,
                  _bitmap.width,
                  _bitmap.height,
                  depth,
                  PNG_COLOR_TYPE_RGB,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    
    /* Initialize rows of PNG. */ 
    row_pointers = static_cast<png_byte**>(png_malloc (png_ptr, _bitmap.height * sizeof (png_byte *)));
    for (y = 0; y < _bitmap.height; ++y) {
        png_byte *row = static_cast<png_byte*>(png_malloc (png_ptr, sizeof (uint8_t) * _bitmap.width * pixel_size));
        row_pointers[y] = row;
		
        for (x = 0; x < _bitmap.width; ++x) {
            pixel_t pixel = _bitmap(x, y);
            *row++ = pixel.red;
            *row++ = pixel.green;
            *row++ = pixel.blue;
        }
    }
    
    /* Write the image data to "fp". */
    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    /* The routine has successfully written the file, so we set
       "status" to a value which indicates success. */
    status = 0;
    
    for (y = 0; y < _bitmap.height; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
	png_failure:
	png_create_info_struct_failed:
		png_destroy_write_struct (&png_ptr, &info_ptr);
	png_create_write_struct_failed:
		fclose (fp);
	fopen_failed:
	return status;
}

/**
 * /name ReadImage
 * /brief Reads a PNG image from disk
 */
int PNGImage::Read(){
	if(_bitmap.imageData==NULL) return ERROR;	
	return SUCCESS;
}