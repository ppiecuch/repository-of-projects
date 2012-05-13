

#include "COGLES1MyGUITexture.h"
#include "CImage.h"

#include "ILogger.h"

namespace yon{
namespace gui{
namespace mygui{
namespace ogles1{

	COGLES1MyGUITexture::COGLES1MyGUITexture(const std::string& name,video::ogles1::COGLES1Driver* driver)
		:m_name(name),m_bLock(false),m_pDriver(driver),m_pTexture(NULL),
		m_usage(MyGUI::TextureUsage::Default),m_numElemBytes(0){
	}
	COGLES1MyGUITexture::~COGLES1MyGUITexture(){
		destroy();
	}

	/*
	void OpenGLTexture::createManual(int _width, int _height, TextureUsage _usage, PixelFormat _format, void* _data)
	{
		MYGUI_PLATFORM_ASSERT(!mTextureID, "Texture already exist");

		//FIXME §á§Ö§â§Ö§ß§Ö§ã§ä§Ú §Ó §Þ§Ö§ä§à§Õ
		mInternalPixelFormat = 0;
		mPixelFormat = 0;
		mNumElemBytes = 0;

		if (_format == PixelFormat::L8)
		{
			//			mInternalPixelFormat = GL_LUMINANCE8;
			//			mPixelFormat = GL_LUMINANCE;
			mInternalPixelFormat = GL_LUMINANCE;
			mPixelFormat = GL_LUMINANCE;
			mNumElemBytes = 1;
		}
		else if (_format == PixelFormat::L8A8)
		{
			//			mInternalPixelFormat = GL_LUMINANCE8_ALPHA8;
			//			mPixelFormat = GL_LUMINANCE_ALPHA;
			mInternalPixelFormat = GL_LUMINANCE_ALPHA;
			mPixelFormat = GL_LUMINANCE_ALPHA;
			mNumElemBytes = 2;
		}
		else if (_format == PixelFormat::R8G8B8)
		{
			//			mInternalPixelFormat = GL_RGB8;
			//			mPixelFormat = GL_BGR;
			mInternalPixelFormat = GL_RGB;
			mPixelFormat = GL_RGB;
			mNumElemBytes = 3;
		}
		else if (_format == PixelFormat::R8G8B8A8)
		{
			//			mInternalPixelFormat = GL_RGBA8;
			//			mPixelFormat = GL_BGRA;
			mInternalPixelFormat = GL_RGBA;
			mPixelFormat = GL_RGBA;
			mNumElemBytes = 4;
		}
		else
		{
			MYGUI_PLATFORM_EXCEPT("format not support");
		}

		mWidth = _width;
		mHeight = _height;
		mDataSize = _width * _height * mNumElemBytes;
		setUsage(_usage);
		//MYGUI_PLATFORM_ASSERT(mUsage, "usage format not support");

		mOriginalFormat = _format;
		mOriginalUsage = _usage;

		// Set unpack alignment to one byte
		int alignment = 0;
		glGetIntegerv( GL_UNPACK_ALIGNMENT, &alignment );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		// §ã§à§Ù§Õ§Ñ§Ö§Þ §ä§å§Ü§ã§ä§å§â§å
		glGenTextures(1, &mTextureID);
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, mInternalPixelFormat, mWidth, mHeight, 0, mPixelFormat, GL_UNSIGNED_BYTE, (GLvoid*)_data);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Restore old unpack alignment
		glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
#if 0
		if (!_data && OpenGLRenderManager::getInstance().isPixelBufferObjectSupported())
		{
			//§ã§à§Ù§Õ§Ñ§Ö§Þ §ä§Ö§Ü§ã§ä§å§â§ß§ß§í§Û §Ò§å§æ§Ö§â
			glGenBuffers(1, &mPboID);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboID);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, 0, mUsage);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
		}
#endif
	}
	*/

	void  COGLES1MyGUITexture::createManual(int _width, int _height, MyGUI::TextureUsage _usage, MyGUI::PixelFormat _format){
		video::ENUM_COLOR_FORMAT format;
		if (_format == MyGUI::PixelFormat::L8){
			//Logger->warn(YON_LOG_WARN_FORMAT,"do not support LUMINACE currently!");
			//return;
			format=video::ENUM_COLOR_FORMAT_L8;
		}else if (_format == MyGUI::PixelFormat::L8A8){
			//Logger->warn(YON_LOG_WARN_FORMAT,"do not support LUMINACE currently!");
			//return;
			format=video::ENUM_COLOR_FORMAT_L8A8;
		}else if (_format == MyGUI::PixelFormat::R8G8B8){
			format=video::ENUM_COLOR_FORMAT_R8G8B8;
		}else if (_format == MyGUI::PixelFormat::R8G8B8A8){
			format=video::ENUM_COLOR_FORMAT_R8G8B8A8;
		}else{
			Logger->warn(YON_LOG_WARN_FORMAT,"do not support PixelFormat currently!");
			return;
		}

		m_pTexture = static_cast<video::ogles1::COGLES1Texture*>(m_pDriver->addTexture(core::dimension2du(_width,_height),io::path(m_name.c_str()),format));
	}
	void  COGLES1MyGUITexture::loadFromFile(const std::string& _filename){
		/*
		destroy();

		if (mImageLoader)
		{
			int width = 0;
			int height = 0;
			PixelFormat format = PixelFormat::Unknow;

			void* data = mImageLoader->loadImage(width, height, format, _filename);
			if (data)
			{
				createManual(width, height, TextureUsage::Static | TextureUsage::Write, format, data);
				delete data;
			}
		}
		*/
		destroy();
		//getTexture
		m_pTexture=static_cast<video::ogles1::COGLES1Texture*>(m_pDriver->getTexture(io::path(_filename.c_str())));
		m_pTexture->upsidedown();
		Logger->debug(YON_LOG_SUCCEED_FORMAT,core::stringc("loadFromFile:%s",_filename.c_str()).c_str());
	}
	void  COGLES1MyGUITexture::saveToFile(const std::string& _filename){
		//TODO
	}


	void* COGLES1MyGUITexture::lock(MyGUI::TextureUsage _access){
		/*
#if 0
		if (_access == TextureUsage::Read)
		{
			glBindTexture(GL_TEXTURE_2D, mTextureID);

			mBuffer = new unsigned char[mDataSize];
			glGetTexImage(GL_TEXTURE_2D, 0, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);

			mLock = false;

			return mBuffer;
		}
#endif
		// bind the texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		if (!OpenGLRenderManager::getInstance().isPixelBufferObjectSupported())
		{
			//Fallback if PBO's are not supported
			mBuffer = new unsigned char[mDataSize];
		}
		else
		{
#if 0
			// bind the PBO
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, mPboID);

			// Note that glMapBuffer() causes sync issue.
			// If GPU is working with this buffer, glMapBuffer() will wait(stall)
			// until GPU to finish its job. To avoid waiting (idle), you can call
			// first glBufferData() with NULL pointer before glMapBuffer().
			// If you do that, the previous data in PBO will be discarded and
			// glMapBuffer() returns a new allocated pointer immediately
			// even if GPU is still working with the previous data.
			glBufferData(GL_PIXEL_UNPACK_BUFFER, mDataSize, 0, mUsage);

			// map the buffer object into client's memory
			mBuffer = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, mAccess);
			if (!mBuffer)
			{
				glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
				MYGUI_PLATFORM_EXCEPT("Error texture lock");
			}
#endif
			MYGUI_PLATFORM_EXCEPT("Error texture lock");
		}

		mLock = true;

		return mBuffer;
		*/
		return m_pTexture->lock();
	}
	void COGLES1MyGUITexture::unlock(){
		/*
		if (!mLock && mBuffer)
		{
			delete mBuffer;
			mBuffer = 0;

			glBindTexture(GL_TEXTURE_2D, 0);

			return;
		}

		MYGUI_PLATFORM_ASSERT(mLock, "Texture is not locked");

		if (!OpenGLRenderManager::getInstance().isPixelBufferObjectSupported())
		{
			//Fallback if PBO's are not supported
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, mBuffer);
			delete mBuffer;
		}
		else
		{
#if 0
			// release the mapped buffer
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
			// copy pixels from PBO to texture object
			// Use offset instead of ponter.
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mWidth, mHeight, mPixelFormat, GL_UNSIGNED_BYTE, 0);

			// it is good idea to release PBOs with ID 0 after use.
			// Once bound with 0, all pixel operations are back to normal ways.
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
#endif
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		mBuffer = 0;
		mLock = false;
		*/
		m_pTexture->unlock();
	}

	void COGLES1MyGUITexture::destroy(){
		/*
#if 0
		if (mRenderTarget != nullptr)
		{
			delete mRenderTarget;
			mRenderTarget = nullptr;
		}
#endif
		if (mTextureID != 0)
		{
			glDeleteTextures(1, &mTextureID);
			mTextureID = 0;
		}
		if (mPboID != 0)
		{
			glDeleteBuffers(1, &mPboID);
			mPboID = 0;
		}

		mWidth = 0;
		mHeight = 0;
		mLock = false;
		mPixelFormat = 0;
		mDataSize = 0;
		mUsage = 0;
		mBuffer = 0;
		mInternalPixelFormat = 0;
		mAccess = 0;
		mNumElemBytes = 0;
		mOriginalFormat = PixelFormat::Unknow;
		mOriginalUsage = TextureUsage::Default;
		*/

		//erase texture
		m_pDriver->removeTexture(m_pTexture);

		m_bLock = false;
		m_numElemBytes = 0;
		m_usage = MyGUI::TextureUsage::Default;
	}

	MyGUI::IRenderTarget* COGLES1MyGUITexture::getRenderTarget(){
		//TODO RTT
		return NULL;
	}

}
}
}
}