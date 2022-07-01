#pragma once


// -- LOCAL

// .. REFERENCES

#include "platform_file_reader.h"
#include "math_vector_2d.h"
#include "lodepng.h"

#include <vector>
#include <iostream>

// -- GLOBAL

// .. TYPES

class GRAPHICS_IMAGE_PIXEL_LOADER
{
	// -- PUBLIC
	public:

	// .. CONSTRUCTORS

	GRAPHICS_IMAGE_PIXEL_LOADER( void ) = default;

	// ~~

	virtual ~GRAPHICS_IMAGE_PIXEL_LOADER( void )
	{
		delete[] pixels;
	}

	// ~~

	void set_pixels( const char* other_pixels );


	// ~~

	GRAPHICS_IMAGE_PIXEL_LOADER( const GRAPHICS_IMAGE_PIXEL_LOADER& other ) : width( other.width ), height( other.height )
	{
		if (other.pixels) {
			set_pixels(other.pixels);
		}
	}


	// ~~

	GRAPHICS_IMAGE_PIXEL_LOADER( const char* other_pixels, const int w, const int h ) : width( w ), height( h )
	{
		if (other_pixels) {
			set_pixels(other_pixels);
		}
	}

	// .. OPERATORS

	GRAPHICS_IMAGE_PIXEL_LOADER& operator=( const GRAPHICS_IMAGE_PIXEL_LOADER& other )
	{
		width = other.width;
		height = other.height;

		set_pixels( other.pixels );

		return *this;
	}

	std::vector<float>  GetRawData();

	// .. ACCESSORS

	int GetHeight( void )
	{
		return height;
	}

	// ~~

	int GetWidth( void )
	{
		return width;
	}

	// ~~

	const char* GetPixels( void )
	{
		return pixels;
	}

	// .. FUNCTIONS

	static void LoadBMP( GRAPHICS_IMAGE_PIXEL_LOADER& image, const char* filename );

	static void LoadPNG(GRAPHICS_IMAGE_PIXEL_LOADER& image_pixel_loader, const char* filename) {
		std::vector<unsigned char> png;
		std::vector<unsigned char> image;
		unsigned height;
		unsigned width;

		//load and decode
		unsigned error = lodepng::load_file(png, filename);
		if (!error) error = lodepng::decode(image, width, height, png);

		image_pixel_loader.height = height;
		image_pixel_loader.width = width;

		//if there's an error, display it
		if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

		//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
		for (auto c : image) {
			float i = (float)c / 255.0f;
			image_pixel_loader.m_raw_data.push_back(i);
		}
	}


	// .. ATTRIBUTES

	char* pixels{ nullptr };
	int  width{ 0 };
	int  height{ 0 };
	std::vector<float>			m_raw_data;
};
