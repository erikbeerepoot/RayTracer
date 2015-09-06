#ifndef __PNG_IMAGE_HPP
#define __PNG_IMAGE_HPP
/**
 * Filename:	PNGImage.hpp 
 * Purpose:		Interface file for wrapper around LibPNG. Used for writing rendering to PNG file.
 * Author:		Erik E. Beerepoot
 */
 #include "GeometricTypes.hpp"
 
 #include <string>
 
 class Bitmap {
	public:	
		Bitmap(std::string filePath,int imgHeight,int imgWidth);
		~Bitmap();
		
		pixel_t * GetImageData();
		void SetImageData(pixel_t *data);
		void SetPixel(int x,int y, pixel_t pix);
		
		
		virtual int Write() = 0;
		virtual int Read() = 0;
		
	protected:
		typedef struct {
			int width;
			int height;
			pixel_t *imageData;			
			pixel_t &operator()(size_t x,size_t y) { return imageData[y*width + x]; }
		} bitmap_t;	
		
		int AllocateBitmap(bitmap_t& bitmap);
		int DeallocateBitmap(bitmap_t& bitmap);
		
		bitmap_t _bitmap;
		std::string _filePath;
 };
 
 class PNGImage : public Bitmap {
	public:
		PNGImage(std::string filePath,int imgHeight,int imgWidth) : Bitmap(filePath,imgHeight,imgWidth) {};
		int Write();
		int Read();	
 };
#endif