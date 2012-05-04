//
// TGA Image Class
// Brandon Jones (Toji)
// Last Updated 10/09/2007 
// Version 1.0
//

#include "TGA.h"
#include "stdafx.h"
#include <exception>

namespace OpenGL21 {
	unsigned const char uTGAHeader[12] = {0,0,02,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	unsigned const char cTGAHeader[12] = {0,0,10,0,0,0,0,0,0,0,0,0};	// RLE Compressed TGA Header

	// Corresponds with GL_RGB, GL_RGBA, and GL_UNSIGNED_BYTE
	unsigned const int TGA_RGB				= 0x1907;
	unsigned const int TGA_RGBA				= 0x1908;
	unsigned const int TGA_UNSIGNED_BYTE	= 0x1401;

	//******************
	// TGAImage
	//******************

	TGAImage::TGAImage() :
		m_width(0), m_height(0),
		m_type(0), m_format(0),
		m_components(0),
		m_imageData(NULL), m_imageDataSize(0) {
		
	}

	TGAImage::~TGAImage() {
		if(m_imageData != NULL) {
			delete[] m_imageData;
			m_imageData = NULL;
		}
	}

	int TGAImage::GetWidth( void ) {
		return m_width;
	}

	int TGAImage::GetHeight( void ) {
		return m_height;
	}

	int TGAImage::GetComponents( void ) {
		return m_components;
	}

	unsigned int TGAImage::GetFormat( void ) {
		return m_format;
	}

	unsigned int TGAImage::GetType( void ) {
		return m_type;
	}

	void* TGAImage::GetImageData( void ) {
		return m_imageData;
	}
	
	void TGAImage::CreateFromFile( const char *filePath ) {
		std::ifstream fileHandle(filePath, std::ios::binary);

		if(!fileHandle.is_open()) {
			throw std::exception("Could not open image file");
		}
		
		try {
			CreateFromStream( fileHandle );
		} catch( std::exception ex ) {
			fileHandle.close();
			throw ex;
		}		
		
		fileHandle.close();
	}

	void TGAImage::CreateFromStream( std::istream &imgStream ) {
		// Read the header information of the TGA file 
		unsigned char tgaHeader[12];
		unsigned char header[6];

		if(imgStream.bad()) {
			throw std::exception("Input Stream Error");
		}

		imgStream.read((char *)&tgaHeader, sizeof(tgaHeader));
		if(imgStream.bad()) {
			throw std::exception("File header could not be read");
		}
		
		imgStream.read((char *)&header, sizeof(header));
		if(imgStream.bad()) {
			throw std::exception("TGA header could not be read");
		}

		m_width  = (header[1] * 256) + header[0];
		m_height = (header[3] * 256) + header[2];
		int bpp	 = header[4];

		if((m_width <= 0) || (m_height <= 0) || ((bpp != 24) && (bpp !=32))) {
			throw std::exception("Unsupported TGA format");
		}

		m_components	= bpp/8;
		m_format		= (m_components == 4 ? TGA_RGBA : TGA_RGB);
		m_type			= TGA_UNSIGNED_BYTE;
		
		m_imageDataSize	= m_components * m_width * m_height;
		m_imageData		= new unsigned char[m_imageDataSize];

		if(m_imageData == NULL) {
			throw std::exception("Could not allocate memory for image");
		}

		if(memcmp(uTGAHeader, &tgaHeader, sizeof(tgaHeader)) == 0) { // Uncompressed
			ReadUncompressed(imgStream);						
		} else if(memcmp(cTGAHeader, &tgaHeader, sizeof(tgaHeader)) == 0) { // Compressed
			ReadCompressed(imgStream);
		} else {
			throw std::exception("TGA Type unsupported. Must be type 2 or 10");
		}
	}

	void TGAImage::ReadUncompressed( std::istream &imgStream ) {
		imgStream.read((char *)m_imageData, m_imageDataSize);
		if(imgStream.gcount() != m_imageDataSize) {
			if(m_imageData != NULL) {
				delete[] m_imageData;
			}
			throw std::exception("Could not read image data");
		}

		for(unsigned int cswap = 0; cswap < m_imageDataSize; cswap += m_components) {
			m_imageData[cswap] ^= m_imageData[cswap+2] ^= m_imageData[cswap] ^= m_imageData[cswap+2];
		}
	}
	
	void TGAImage::ReadCompressed( std::istream &imgStream ) {
		unsigned int pixelCount	= m_height * m_width;
		unsigned int currentPixel = 0;
		unsigned int currentByte = 0;								
		unsigned char chunkHeader;
		unsigned int counter;
		unsigned char* colorBuffer = m_imageData;			

		do {
			chunkHeader = 0;
			
			imgStream.read((char*)&chunkHeader, sizeof(unsigned char));
			if(imgStream.gcount() == 0) {
				if(m_imageData != NULL) {
					delete[] m_imageData;
				}
				throw std::exception("Could not read RLE header");
			}

			if(chunkHeader < 128) {
				chunkHeader++;
				for(counter = 0; counter < chunkHeader; counter++) {
					imgStream.read((char *)colorBuffer, m_components);
					if(imgStream.gcount() != m_components) {
						if(m_imageData != NULL) {
							delete[] m_imageData;
						}
						throw std::exception("Could not read image data");
					}

					colorBuffer[0] ^= colorBuffer[2] ^= colorBuffer[0] ^= colorBuffer[2];
					colorBuffer += m_components;
					currentPixel++;

					if(currentPixel > pixelCount) {
						if(m_imageData != NULL) {
							delete[] m_imageData;
						}
						throw std::exception("Too many pixels read");
					}
				}
			} else {
				chunkHeader -= 127;
				imgStream.read((char *)colorBuffer, m_components);
				if(imgStream.gcount() != m_components) {	
					if(m_imageData != NULL) {
						delete[] m_imageData;
					}
					throw std::exception("Could not read from file");
				}

				colorBuffer[0] ^= colorBuffer[2] ^= colorBuffer[0] ^= colorBuffer[2];

				for(counter = m_components; counter < (unsigned int)(chunkHeader * m_components); counter++) {
					colorBuffer[m_components] = colorBuffer[0];
					colorBuffer++;
				}

				colorBuffer += m_components;

				currentPixel += chunkHeader;
				if(currentPixel > pixelCount) {
					if(m_imageData != NULL) {
						delete[] m_imageData;
					}
					throw std::exception("Too many pixels read");
				}
			}
		} while(currentPixel < pixelCount);
	}

}