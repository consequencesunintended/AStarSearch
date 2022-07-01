#pragma once

#include "GLFW/glfw3.h"

#include <string>
#include <sstream> 
#include <iomanip>

class GRAPHICS_CLIPBOARD {
public:
	~GRAPHICS_CLIPBOARD() {
		free(m_data_buffer);
	}

	void grab_screen(const int x, const int y, const int w, const int h, const bool grey_scale = false) {

		m_width = w;
		m_height = h;

		if (m_data_buffer) {
			free(m_data_buffer);
		}
		m_cached_hex.clear();

		if (!grey_scale) {
			m_data_buffer = (unsigned char*)malloc(w * h * 3 * sizeof(unsigned char));
			
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels((GLint)x, (GLint)y, (GLint)m_width, (GLint)m_height, GL_RGB, GL_UNSIGNED_BYTE, m_data_buffer);
		}
		else {
			unsigned char* tempDataBuffer = (unsigned char*)malloc(w * h * 3 * sizeof(unsigned char));

			m_data_buffer = (unsigned char*)malloc(w * h  * sizeof(unsigned char));

			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glReadPixels((GLint)x, (GLint)y, (GLint)m_width, (GLint)m_height, GL_RGB, GL_UNSIGNED_BYTE, tempDataBuffer);

			int					c = 0;
			int					gIndex = 0;
			float				acc = 0;
			std::ostringstream	internalStream;

			for (size_t ic = 0; ic < w * h * 3; ic++) {

				int				value = int(tempDataBuffer[ic]);
				const float		normalised_value = float(value) / 255.0f;

				if (c == 0) {
					acc += 0.299f * normalised_value;
				}
				else if (c == 1) {
					acc += 0.587f * normalised_value;
				}
				else if (c == 2) {
					acc += 0.114f * normalised_value;
				}
				if (++c == 3) {					
					c = 0;
					value = int(acc * 255.0f);
					internalStream << std::setw(2) << std::setfill('0') << std::hex << value;
					m_data_buffer[gIndex++] = value;
					acc = 0;
				}
			}

			if (m_should_cache_hex)
			{
				m_cached_hex = internalStream.str();
			}

			free(tempDataBuffer);
		}
	}
	unsigned char*	m_data_buffer{ nullptr };
	std::string		m_cached_hex;
	int				m_width{ 0 };
	int				m_height{ 0 };
	bool			m_should_cache_hex{ false };
};
