//
// TGA Image Class 
// Brandon Jones (Toji)
// Last Updated 10/09/2007 
// Version 1.0
//

#if !defined(GL21_TGA_IMAGE_H_)
#define GL21_TGA_IMAGE_H_

#include <fstream>
#include "stdafx.h"

namespace OpenGL21 {

	//******************
	// TGAImage
	//******************
	
	/// Loads an uncompressed or RLE compressed TGA image file 
	class TGAImage {
		public:
										TGAImage();
			virtual						~TGAImage();
			
			void						CreateFromFile( const char* filePath );		/// Create this image using the given file path
			void						CreateFromStream( std::istream &imgStream );	/// Uses the given file handle to load the TGA image data

			int							GetComponents( void );		/// Get the number of color components in this image
			unsigned int				GetFormat( void );			/// Get the OpenGL format for this image (ie: GL_RGBA)
			unsigned int				GetType( void );			/// Get the OpenGL component type for this image (ie: GL_UNSIGNED_BYTE)

			int							GetWidth( void );			/// Get the width, in pixels, of this image
			int							GetHeight( void );			/// Get the height, in pixels, of this image

			virtual void*				GetImageData( void );		/// Get a pointer to the pixel data of this image 

		protected:
			unsigned int				m_type;						/// Bits Per Pixel (Color Depth)
			unsigned int				m_format;					/// The image format
			int							m_components;				/// Components per Pixel
			int							m_width, m_height;			/// Image Dimensions

			unsigned char*				m_imageData;				/// The image data
			unsigned int				m_imageDataSize;			/// Stores the total size in bytes of the image data

		private:
			void						ReadUncompressed( std::istream &imgStream );		/// Read in uncompressed TGA image data
			void						ReadCompressed( std::istream &imgStream );			/// Read in compressed TGA image data
	};

}

#endif