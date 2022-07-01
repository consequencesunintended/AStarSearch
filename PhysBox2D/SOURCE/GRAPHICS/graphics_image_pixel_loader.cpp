#include "graphics_image_pixel_loader.h"

// -- LOCAL

// .. REFERENCES

#include "platform_file_reader.h"
#include "lodepng.h"

// -- PUBLIC

// .. FUNCTIONS

void GRAPHICS_IMAGE_PIXEL_LOADER::LoadBMP( GRAPHICS_IMAGE_PIXEL_LOADER & image, const char * filename ) 
{
	int		dataOffset;
	int		headerSize;
	int		width;
	int		height;
	int		bytesPerRow;
	int		size;
	char	*pixels3;
	char	*pixels2;
	int		index_x;
	int		index_y;
	int		index_c;

	PLATFORM_FILE_READER  file_reader_engine;

	file_reader_engine.LoadFile( filename );
	file_reader_engine.SkipCharacters( 10 );

	dataOffset = file_reader_engine.readInt();
	headerSize = file_reader_engine.readInt();

	switch( headerSize ) 
	{
		case 40:
			width = file_reader_engine.readInt();
			height = file_reader_engine.readInt();
			break;
		case 12:
			width = file_reader_engine.readShort();
			height = file_reader_engine.readShort();
			break;
	}
	bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	size = bytesPerRow * height;
	pixels3 = new char[ size ];
	file_reader_engine.SeekInBeginning(dataOffset);
	file_reader_engine.ReadCharacters( pixels3 , size);
	pixels2 = new char[ width * height * 3 ];

	for ( index_y = 0; index_y <= height - 1; index_y++ )
	{
		for ( index_x = 0; index_x <= width - 1; index_x++ )
		{
			for ( index_c = 0; index_c < 3; index_c++ )
			{
				pixels2[ 3 * ( width * index_y + index_x ) + index_c ] = pixels3[ bytesPerRow * index_y + 3 * index_x + ( 2 - index_c ) ];
			}
		}
	}	
	file_reader_engine.CloseTheFile();
	image = GRAPHICS_IMAGE_PIXEL_LOADER( pixels2, width, height);
	delete [] pixels2;
	delete [] pixels3;
}

void GRAPHICS_IMAGE_PIXEL_LOADER::set_pixels( const char* other_pixels )
{
	pixels = new char[ width * height * 3 ];

	for ( int index_y = 0; index_y <= height - 1; index_y++ )
	{
		for ( int index_x = 0; index_x <= width - 1; index_x++ )
		{
			for ( int index_c = 0; index_c < 3; index_c++ )
			{
				pixels[ 3 * (width * index_y + index_x) + index_c ] = other_pixels[ 3 * (width * index_y + index_x) + index_c ];
			}
		}
	}
}

std::vector<float> GRAPHICS_IMAGE_PIXEL_LOADER::GetRawData()
{
	int size = GetWidth() * GetHeight() * 3;

	if (m_raw_data.empty()) {
		m_raw_data.resize(size, 0.0f);

		for (size_t index_y = 0; index_y < GetHeight(); index_y++)
		{
			for (size_t index_x = 0; index_x < GetWidth(); index_x++)
			{
				m_raw_data[3 * (index_y * GetWidth() + index_x)] = 114.0f / 255.0f;
				m_raw_data[3 * (index_y * GetWidth() + index_x) + 1] = 0.0f;
				m_raw_data[3 * (index_y * GetWidth() + index_x) + 2] = 138.0f / 255.0f;
			}
		}

		for (size_t index_y = 0; index_y < GetHeight(); index_y++)
		{
			for (size_t index_x = 0; index_x < GetWidth(); index_x++)
			{
				unsigned int colour_r = GetPixels()[3 * (index_y * GetWidth() + index_x)];
				unsigned int colour_g = GetPixels()[3 * (index_y * GetWidth() + index_x) + 1];
				unsigned int colour_b = GetPixels()[3 * (index_y * GetWidth() + index_x) + 2];

				if (colour_r != -1)
				{
					m_raw_data[3 * (index_y * GetWidth() + index_x)] = float(colour_r) / 255.0f;
					m_raw_data[3 * (index_y * GetWidth() + index_x) + 1] = float(colour_g) / 255.0f;
					m_raw_data[3 * (index_y * GetWidth() + index_x) + 2] = float(colour_b) / 255.0f;
				}
			}
		}
	}

	return m_raw_data;
}









