///////////////////////////////////////////////////////////////////////////////
//
//	Simple Framework for OpenGL 2.0
//
//	note: requires GLEW (http://glew.sourceforge.net)
//
//	last update : 2005 SEP 24
//
//	author : KIST IMRC Jinwook Kim
//	e-mail : jwkim@imrc.kist.re.kr
//
///////////////////////////////////////////////////////////////////////////////

#include "glsl_common.h"

#define  _USE_MATH_DEFINES

#include <math.h>
#include <assert.h>

#include <fstream>
#include <algorithm>
#include <iostream>

#define _RTD_ 57.29577951308232f

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")

glCamera *glCamera::m_pDefaultCamera = NULL;
vector <glLight *> glLight::m_pLights;

int glTransform::m_iWndSizeWidth = 512;
int glTransform::m_iWndSizeHeight = 512;

glTransform::glTransform()
{
	for ( int i = 0; i < 16; i++ ) m_fTransformMatrix[i] = m_fPrevTransformMatrix[i] = (i % 5 ? 0.0f : 1.0f);
}

void glTransform::multTransform(void) const
{
	glMultMatrixf(m_fTransformMatrix);
}

void glTransform::multInverseTransform(void) const
{
	static float mat[16] = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};

	mat[0] = m_fTransformMatrix[0];
	mat[1] = m_fTransformMatrix[4];
	mat[2] = m_fTransformMatrix[8];
	mat[4] = m_fTransformMatrix[1];
	mat[5] = m_fTransformMatrix[5];
	mat[6] = m_fTransformMatrix[9];
	mat[8] = m_fTransformMatrix[2];
	mat[9] = m_fTransformMatrix[6];
	mat[10] = m_fTransformMatrix[10];
	mat[12] = -m_fTransformMatrix[0] * m_fTransformMatrix[12] - m_fTransformMatrix[1] * m_fTransformMatrix[13] - m_fTransformMatrix[2] * m_fTransformMatrix[14];
	mat[13] = -m_fTransformMatrix[4] * m_fTransformMatrix[12] - m_fTransformMatrix[5] * m_fTransformMatrix[13] - m_fTransformMatrix[6] * m_fTransformMatrix[14];
	mat[14] = -m_fTransformMatrix[8] * m_fTransformMatrix[12] - m_fTransformMatrix[9] * m_fTransformMatrix[13] - m_fTransformMatrix[10] * m_fTransformMatrix[14];

	glMultMatrixf(mat);
}

void glTransform::getInverseEulerHPR(float *hpr, float *xyz) const
{
	hpr[1] = _RTD_ * asin(m_fTransformMatrix[6]);
	if ( hpr[1] < 90.0f )
	{
		if ( hpr[1] > -90.0f )
		{
			hpr[0] = _RTD_ * atan2(-m_fTransformMatrix[4], m_fTransformMatrix[5]);
			hpr[2] = _RTD_ * atan2(-m_fTransformMatrix[2], m_fTransformMatrix[10]);
		} else
		{
			hpr[0] = -_RTD_ * atan2(m_fTransformMatrix[8], m_fTransformMatrix[0]);
			hpr[2] = 0.0f;
		}
	} else
	{
		hpr[0] = _RTD_ * atan2(m_fTransformMatrix[8], m_fTransformMatrix[0]);
		hpr[2] = 0.0f;
	}

	if ( xyz != NULL )
	{
		xyz[0] = m_fTransformMatrix[12];
		xyz[1] = m_fTransformMatrix[13];
		xyz[2] = m_fTransformMatrix[14];
	}
}

void glTransform::lookAt(float x, float y, float z, float h, float p, float r)
{
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, z);
	glRotatef(h, 0.0f, 0.0f, 1.0f);
	glRotatef(p, 1.0f, 0.0f, 0.0f);
	glRotatef(r, 0.0f, 1.0f, 0.0f);

	glGetFloatv(GL_MODELVIEW_MATRIX, m_fTransformMatrix);

	glPopMatrix();
}

void glTransform::setProjectionMatrix(float fovy, float aspect, float zNear,  float zFar)
{
	int mode;
	glGetIntegerv(GL_MATRIX_MODE, &mode);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();  
	gluPerspective(fovy, aspect, zNear, zFar);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_fProjectionMatrix);
	glPopMatrix();
	glMatrixMode(mode);
}

void glTransform::setWindowSize(int x, int y)
{
	m_iWndSizeWidth = x;
	m_iWndSizeHeight = y;
}

glLight::glLight()
{
	if ( m_pLights.empty() )
		m_iID = 0;
	else
	{
		m_iID = -1;
		int i;
		vector <int> ids;
		for ( i = 0; i < (int)(m_pLights.size()); i++ ) ids.push_back(m_pLights[i]->m_iID);
		sort(ids.begin(), ids.end());
		for ( i = 0; i < (int)(ids.size()) - 1; i++ )
			if ( ids[i+1] - ids[i] > 1 )
			{
				m_iID = ids[i] + 1;
				break;
			}

			if ( m_iID == -1 ) m_iID = *(ids.end() - 1) + 1;
	}
	m_pLights.push_back(this);

	enable();

	if ( m_iID )
	{
		setDiffuse(glColor(1.0f, 1.0f, 1.0f));
		setSpecular(glColor(1.0f, 1.0f, 1.0f));
	}
}

glLight *glLight::getLight(unsigned int idx)
{
	return (m_pLights.size() <= idx ? NULL : m_pLights[idx]);
}

int glLight::getNumLights(void)
{
	return (int)(m_pLights.size());
}

bool glLight::isEnabled(void) const
{
	return m_bEnabled;
}

void glLight::getPosition(float position[4]) const
{
	position[0] = m_fTransformMatrix[12];
	position[1] = m_fTransformMatrix[13];
	position[2] = m_fTransformMatrix[14];
	position[3] = 1.0f;
}

int glLight::getID(void) const
{
	return m_iID;
}

glLight::~glLight()
{
	vector <glLight *>::iterator iter = find(m_pLights.begin(), m_pLights.end(), this);

	if ( iter != m_pLights.end() ) m_pLights.erase(iter);
}

void glLight::enable(void)
{
	m_bEnabled = true;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + m_iID);
}

void glLight::disable(void)
{
	m_bEnabled = false;
	glDisable(GL_LIGHT0 + m_iID);

	for ( int i = 0; i < getNumLights(); i++ ) if ( getLight(i)->isEnabled() ) return;

	glDisable(GL_LIGHTING);
}

void glLight::setPosition(float x, float y, float z)
{
	m_fTransformMatrix[12] = x;
	m_fTransformMatrix[13] = y;
	m_fTransformMatrix[14] = z;
}

void glLight::setAmbient(const glColor &color)
{
	glLightfv(GL_LIGHT0 + m_iID, GL_AMBIENT, &color[0]);
}

void glLight::setDiffuse(const glColor &color)
{
	glLightfv(GL_LIGHT0 + m_iID, GL_DIFFUSE, &color[0]);
}

void glLight::setSpecular(const glColor &color)
{
	glLightfv(GL_LIGHT0 + m_iID, GL_SPECULAR, &color[0]);
}

void glLight::getAmbientColor(float color[4]) const
{
	glGetLightfv(GL_LIGHT0 + m_iID, GL_AMBIENT, color);
}

void glLight::getDiffuseColor(float color[4]) const
{
	glGetLightfv(GL_LIGHT0 + m_iID, GL_DIFFUSE, color);
}

void glLight::getSpecularColor(float color[4]) const
{
	glGetLightfv(GL_LIGHT0 + m_iID, GL_SPECULAR, color);
}

void glLight::update(void)
{
	static float zero[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static float minusZ[] = { 0.0f, 0.0f, -1.0f};
	glPushMatrix();
	multTransform();
	glLightfv(GL_LIGHT0 + m_iID, GL_POSITION, zero);
	glLightfv(GL_LIGHT0 + m_iID, GL_SPOT_DIRECTION, minusZ);
	glPopMatrix();	
}

void glLight::loadProjectionMatrix(void) const
{
	glLoadMatrixf(m_fProjectionMatrix);
}

glTexture::glTexture()
{
	m_iTextureID = -1;
	m_eTarget = GL_TEXTURE_2D;
	m_eType = GL_UNSIGNED_BYTE;
	m_pData = NULL;

	_clear();

	m_iParam[WRAP_S] = GL_REPEAT;
	m_iParam[WRAP_T] = GL_REPEAT;
	m_iParam[WRAP_R] = GL_REPEAT;
	m_iParam[MIN_FILTER] = GL_NEAREST_MIPMAP_LINEAR;
	m_iParam[MAG_FILTER] = GL_LINEAR;
	m_iParam[DEPTH_MODE] = GL_LUMINANCE;
	m_iParam[COMPARE_MODE] = GL_NONE;
	m_iParam[COMPARE_FUNC] = GL_LEQUAL;
	m_iParam[GENERATE_MIPMAP] = GL_TRUE;
}

void glTexture::setTarget(GLenum target)
{
	m_eTarget = target;
}

glTexture::~glTexture()
{
	if ( glIsTexture(m_iTextureID) == GL_TRUE ) glDeleteTextures(1, &m_iTextureID);
	delete [] m_pData;
}

void glTexture::_clear()
{
	delete [] m_pData;
	m_pData = NULL;
	m_iWidth = m_iHeight = m_iDepth = m_iMipmapCount = 0;
}

void glTexture::bind(int unit)
{
	if ( unit != -1 ) glActiveTexture(GL_TEXTURE0 + unit);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_3D);
	glEnable(m_eTarget);
	glBindTexture(m_eTarget, m_iTextureID);

	static GLuint _pname[] = { GL_TEXTURE_WRAP_S, GL_TEXTURE_WRAP_T, GL_TEXTURE_WRAP_R, GL_TEXTURE_MIN_FILTER, GL_TEXTURE_MAG_FILTER, GL_DEPTH_TEXTURE_MODE, GL_TEXTURE_COMPARE_MODE, GL_TEXTURE_COMPARE_FUNC ,GL_GENERATE_MIPMAP };
	for ( int i = 0; i < 9; i++ ) glTexParameteri(m_eTarget, _pname[i], m_iParam[i]);
}

void glTexture::disable(int unit)
{
	if ( unit != -1 ) glActiveTexture(GL_TEXTURE0 + unit);
	glDisable(m_eTarget);
}

void glTexture::texGen(int pgen, int penv)
{
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, pgen);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, pgen);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, penv);
}

void genNormalFromHeight(int width, int height, unsigned char **data)
{
	const float sobelX[5][5] = { 1, 2, 0, -2, -1, 4, 8,  0, -8, -4, 6, 12, 0, -12, -6,  4,  8,   0, -8, -4,  1,  2,  0, -2, -1 };
	const float sobelY[5][5] = { 1, 4, 6,  4,  1, 2, 8, 12,  8,  2, 0,  0, 0,   0,  0, -2, -8, -12, -8, -2, -1, -4, -6, -4, -1 };

	// Size of the z component
	float sZ = 128.0f / max(width, height);

	unsigned char *newPixels = new unsigned char[4 * width * height];
	unsigned char *dest = newPixels;
	unsigned char *src = *data;

	for ( int y = 0; y < height; y++ )
	{
		for ( int x = 0; x < width; x++ )
		{
			// Apply a 5 x 5 Sobel filter
			float sX = 0;
			float sY = 0;
			for ( int dy = 0; dy < 5; dy++ )
			{
				int fy = (y + dy - 2 + height) % height;
				for ( int dx = 0; dx < 5; dx++ )
				{
					int fx = (x + dx - 2 + width) % width;
					sX += sobelX[dy][dx] * src[fy * width + fx];
					sY += sobelY[dy][dx] * src[fy * width + fx];
				}
			}
			// Construct the components
			sX *= 1.0f / (48 * 255);
			sY *= 1.0f / (48 * 255);
			float invLen = 1.0f / sqrt(sX * sX + sY * sY + sZ * sZ);

			// Normalize and store
			dest[0] = (unsigned char)(127.5f * (sX * invLen + 1));
			dest[1] = (unsigned char)(127.5f * (sY * invLen + 1));
			dest[2] = (unsigned char)(127.5f * (sZ * invLen + 1));
			dest[3] = src[y * width + x];
			dest += 4;
		}
	}

	delete [] *data;
	*data = newPixels;
}

bool glTexture::_loadBMP(const char file[])
{
	_clear();

	if ( !file ) return false;

	BITMAPFILEHEADER    bmpFileHeader;
	BITMAPINFOHEADER    bmpInfoHeader;

	ifstream fin(file, ios::binary);

	if( !fin.good() )
	{
		// try to find the source file at a parent directory.
		char file_at_parent[1024];
		strcpy(file_at_parent, "../");
		strcat(file_at_parent, file);

		fin.clear();
		fin.open(file_at_parent, ios::binary);
		if( !fin.good() ) return false;
	}

	fin.read((char *)&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	fin.read((char *)&bmpInfoHeader, sizeof(BITMAPINFOHEADER));

	if ( bmpFileHeader.bfType != 'MB' || (bmpInfoHeader.biBitCount != 8 && bmpInfoHeader.biBitCount != 24 && bmpInfoHeader.biBitCount != 32) )
	{
		// unsupported feature
		fin.close();
		return 0;
	}

	m_iWidth  = bmpInfoHeader.biWidth;
	m_iHeight = bmpInfoHeader.biHeight;

	if ( !bmpInfoHeader.biSizeImage ) bmpInfoHeader.biSizeImage = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * bmpInfoHeader.biBitCount / 8;

	m_pData = new unsigned char [bmpInfoHeader.biSizeImage];

	fin.read((char *)m_pData, bmpInfoHeader.biSizeImage);

	fin.close();

	switch ( bmpInfoHeader.biBitCount )
	{
	case 8:
		m_iInternalFormat = GL_LUMINANCE8;
		m_eFormat = GL_LUMINANCE;
		break;
	case 24:
		m_iInternalFormat = GL_RGB8;
		m_eFormat = GL_BGR;
		break;
	case 32:
		m_iInternalFormat = GL_RGBA8;
		m_eFormat = GL_BGRA;
		break;
	}
	m_iDepth = 1;
	m_iMipmapCount = 1;

	return true;
}

#pragma pack (push, 1)
struct DDSHeader
{
	unsigned int dwMagic, dwSize, dwFlags, dwHeight, dwWidth, dwPitchOrLinearSize, dwDepth, dwMipMapCount, dwReserved[11];
	struct { unsigned int dwSize, dwFlags, dwFourCC, dwRGBBitCount, dwRBitMask, dwGBitMask, dwBBitMask, dwRGBAlphaBitMask; } ddpfPixelFormat;
	struct { unsigned int dwCaps1, dwCaps2, Reserved[2]; } ddsCaps;
	unsigned int dwReserved2;
};
#pragma pack (pop)

#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))

inline int get_bytes(int format)
{
	switch ( format )
	{
	case GL_R3_G3_B2:
	case GL_LUMINANCE8:
		return 1;
	case GL_LUMINANCE8_ALPHA8:
	case GL_LUMINANCE16:
		return 2;
	case GL_RGB8:
		return 3;
	case GL_RGBA8:
	case GL_LUMINANCE16_ALPHA16:
	case GL_LUMINANCE32F_ARB:
		return 4;
	case GL_RGBA16:
	case GL_RGBA16F_ARB:
	case GL_LUMINANCE_ALPHA32F_ARB:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
		return 8;
	case GL_RGBA32F_ARB:
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		return 16;
	}
	return 0;
}

inline int get_channels(int format)
{
	switch ( format )
	{
	case GL_LUMINANCE8:
	case GL_LUMINANCE16:
	case GL_LUMINANCE32F_ARB:
		return 1;
	case GL_LUMINANCE8_ALPHA8:
	case GL_LUMINANCE16_ALPHA16:
	case GL_LUMINANCE_ALPHA32F_ARB:
		return 2;
	case GL_R3_G3_B2:
	case GL_RGB8:
		return 3;
	case GL_RGBA8:
	case GL_RGBA16:
	case GL_RGBA16F_ARB:
	case GL_RGBA32F_ARB:
	case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
	case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
		return 4;
	}
	return 0;
}

int getImageSize(int format, int w, int h, int d, int nMipMapLevels, bool isCube)
{
	int size = 0;
	while ( nMipMapLevels )
	{
		if ( format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT ) size += ((w + 3) >> 2) * ((h + 3) >> 2) * d;
		else size += w * h * d;

		if ( w == 1 && h == 1 && d == 1 ) break;
		if ( w > 1 ) w >>= 1;
		if ( h > 1 ) h >>= 1;
		if ( d > 1 ) d >>= 1;
		nMipMapLevels--;
	}

	size *= get_bytes(format);
	if ( isCube ) size *= 6;

	return size;
}

inline int _sz(int x, int w) { return max(1, x >> w); }

bool glTexture::_loadDDS(const char file_name[])
{
	_clear();

	DDSHeader header;
	FILE *file;

	if ( (file = fopen(file_name, "rb")) == NULL ) return false;

	fread(&header, sizeof(header), 1, file);
	if ( header.dwMagic != FOURCC('D', 'D', 'S', ' ') )
	{
		fclose(file);
		return false;
	}

	m_iWidth    = header.dwWidth;
	m_iHeight   = header.dwHeight;
	m_iDepth    = header.dwDepth;
	m_iMipmapCount = header.dwMipMapCount;

	if ( header.ddsCaps.dwCaps2 & 0x00000200 ) m_eTarget = GL_TEXTURE_CUBE_MAP;
	if ( m_iDepth == 0 ) m_iDepth = 1;
	if ( m_iDepth > 1 ) m_eTarget = GL_TEXTURE_3D;

	if ( m_iMipmapCount <= 0 ) m_iMipmapCount = 1;

	switch ( header.ddpfPixelFormat.dwFourCC )
	{
	case FOURCC('D', 'X', 'T', '1'):
		m_iInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		m_eFormat = GL_RGBA;
		break;
	case FOURCC('D', 'X', 'T', '3'):
		m_iInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		m_eFormat = GL_RGBA;
		break;
	case FOURCC('D', 'X', 'T', '5'):
		m_iInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		m_eFormat = GL_RGBA;
		break;
	case FOURCC('A', 'T', 'I', '2'):
		// unsupported feature
		break;
	case FOURCC('A', 'T', 'I', '1'):
		// unsupported feature
		break;
	case 34:
		m_iInternalFormat = GL_LUMINANCE16_ALPHA16;
		m_eFormat = GL_LUMINANCE_ALPHA;
		m_eType = GL_UNSIGNED_SHORT;
		break;
	case 36:
		m_iInternalFormat = GL_RGBA16;
		m_eFormat = GL_RGBA;
		m_eType = GL_UNSIGNED_SHORT;
		break;
	case 113:
		m_iInternalFormat = GL_RGBA16F_ARB;
		m_eFormat = GL_RGBA;
		m_eType = GL_HALF_FLOAT_ARB;
		break;
	case 114:
		m_iInternalFormat = GL_LUMINANCE32F_ARB;
		m_eFormat = GL_LUMINANCE;
		m_eType = GL_FLOAT;
		break;
	case 115:
		m_iInternalFormat = GL_LUMINANCE_ALPHA32F_ARB;
		m_eFormat = GL_LUMINANCE_ALPHA;
		break;
	case 116:
		m_iInternalFormat = GL_RGBA32F_ARB;
		m_eFormat = GL_RGBA;
		m_eType = GL_FLOAT;
		break;
	default:
		switch ( header.ddpfPixelFormat.dwRGBBitCount )
		{
		case 8:
			m_iInternalFormat = header.ddpfPixelFormat.dwRBitMask == 0xE0 ? GL_R3_G3_B2 : GL_LUMINANCE8;
			m_eFormat = header.ddpfPixelFormat.dwRBitMask == 0xE0 ? GL_RGB : GL_LUMINANCE;
			m_eType = header.ddpfPixelFormat.dwRBitMask == 0xE0 ? GL_UNSIGNED_BYTE_3_3_2 : GL_UNSIGNED_BYTE;
		break;
		case 16:
			m_iInternalFormat = header.ddpfPixelFormat.dwRGBAlphaBitMask ? GL_LUMINANCE8_ALPHA8 : GL_LUMINANCE16;
			m_eFormat = header.ddpfPixelFormat.dwRGBAlphaBitMask ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
			m_eType = header.ddpfPixelFormat.dwRGBAlphaBitMask ? GL_UNSIGNED_BYTE: GL_UNSIGNED_SHORT;
			break;
		case 24:
			m_iInternalFormat = GL_RGB8;
			m_eFormat = GL_RGB;
			break;
		case 32:
			m_iInternalFormat = GL_RGBA8;
			m_eFormat = GL_RGBA;
			break;
		default:
			fclose(file);
			return false;
		}
	}

	// Load the image
	int size = getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, m_iMipmapCount, m_eTarget == GL_TEXTURE_CUBE_MAP);
	m_pData = new unsigned char [size];
	
	if ( m_eTarget == GL_TEXTURE_CUBE_MAP )
	{
		for (int face = 0; face < 6; face++)
		{
			for (int level = 0; level < m_iMipmapCount; level++)
			{
				int faceSize = getImageSize(m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), _sz(m_iDepth, level), 1, false);
				unsigned char *src = (unsigned char *)m_pData + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, true) + face * faceSize;
				fread(src, 1, faceSize, file);
			}
		}
	} else
		fread(m_pData, 1, size, file);

	int m_iNumChannel = get_channels(m_iInternalFormat);

	// Swap channels for formats stored in BGR order
	if ( (m_iInternalFormat == GL_RGB8 || m_iInternalFormat == GL_RGBA8) && header.ddpfPixelFormat.dwBBitMask == 0xFF )
	{
		unsigned char *pixels = (unsigned char *)m_pData;
		int nPixels = size / m_iNumChannel; 
		do
		{
			unsigned char tmp = pixels[2];
			pixels[2] = pixels[0];
			pixels[0] = tmp;
			pixels += m_iNumChannel;
		} while ( --nPixels );
	}

	fclose(file);
	return true;
}

enum IMG_FORMAT { BMP, DDS, UNKNOWN };

IMG_FORMAT get_extension(const char file[])
{
	char buf[128], ext[128];
	strcpy(buf, file);
	char *token = strtok(buf, ".");

	while( token != NULL )
	{
		token = strtok( NULL, ".");
		if ( token ) strcpy(ext, token);
	}

	if ( strcmp(strlwr(ext), "bmp") == 0 ) return BMP;
	if ( strcmp(strlwr(ext), "dds") == 0 ) return DDS;

	cerr << "unknown texture format" << endl;
	return UNKNOWN;
}

void glTexture::load(const char file[], bool generate_normal)
{
	if ( glIsTexture(m_iTextureID) == GL_TRUE ) glDeleteTextures(1, &m_iTextureID);
	glGenTextures(1, &m_iTextureID);

	switch ( get_extension(file) )
	{
	case BMP:
		if ( !_loadBMP(file) )
		{
			cerr << "can not load texture file " << file << endl;
			return;
		}

		if ( generate_normal && m_iInternalFormat == GL_LUMINANCE )
		{
			genNormalFromHeight(m_iWidth, m_iHeight, (unsigned char **)&m_pData);
			m_iInternalFormat = GL_RGBA;
			m_eFormat = GL_RGBA;
		}

		bind(-1);
		glTexImage2D(m_eTarget, 0, m_iInternalFormat, m_iWidth, m_iHeight, 0, m_eFormat, GL_UNSIGNED_BYTE, m_pData);
		break;
	case DDS:
		if ( !_loadDDS(file) )
		{
			cerr << "can not load texture file " << file << endl;
			return;
		}

		bind(-1);
		for ( int level = 0; level < m_iMipmapCount; level++ )
		{
			if ( m_eTarget == GL_TEXTURE_CUBE_MAP )
			{
				int size = getImageSize(m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), _sz(m_iDepth, level), 1, false);
				for ( int face = 0; face < 6; face++ )
				{
					switch ( m_iInternalFormat )
					{
					case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
					case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
					case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
						glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), 0, size, (unsigned char *)m_pData + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, true) + face * size);
						break;
					default:
						glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), 0, m_eFormat, m_eType, (unsigned char *)m_pData + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, true) + face * size);
						break;
					}
				}
			} else if ( m_iDepth > 1 )
			{
				switch ( m_iInternalFormat )
				{
				case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
				case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
					cerr << "S3TC Texture compression does not support 3D texture" << endl;
					break;
				default:
					glTexImage3D(m_eTarget, level, m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), _sz(m_iDepth, level), 0, m_eFormat, m_eType, (unsigned char *)m_pData + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, false));
					break;
				}
			} else
			{
				switch ( m_iInternalFormat )
				{
				case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
				case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
				case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
					glCompressedTexImage2D(m_eTarget, level, m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), 0, getImageSize(m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), _sz(m_iDepth, level), 1, false), (unsigned char *)m_pData + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, false));
					break;
				default:
					glTexImage2D(m_eTarget, level, m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), 0, m_eFormat, m_eType, (unsigned char *)m_pData + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, false));
					break;
				}
			}
		}
		break;
	case UNKNOWN:
		cerr << "unknown format " << file << endl;
		break;
	}
}

void glTexture::init(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, void *buf)
{
	if ( glIsTexture(m_iTextureID) == GL_TRUE ) glDeleteTextures(1, &m_iTextureID);
	glGenTextures(1, &m_iTextureID);

	m_iWidth  = width;
	m_iHeight = height;
	m_iDepth  = depth;
	m_iInternalFormat = internalformat;
	m_eFormat = format;
	m_iMipmapCount = 1;
	m_eType = type;
	m_eTarget = target;

	bind(-1);

	switch ( m_eTarget )
	{
	case GL_TEXTURE_1D:
		assert(m_iHeight == 1 && m_iDepth == 1);
		glTexImage1D(m_eTarget, level, m_iInternalFormat, m_iWidth, border, m_eFormat, m_eType, buf);
		break;
	case GL_TEXTURE_2D:
		assert(m_iDepth == 1);
		glTexImage2D(m_eTarget, level, m_iInternalFormat, m_iWidth, m_iHeight, border, m_eFormat, m_eType, buf);
		break;
	case GL_TEXTURE_3D:
		glTexImage3D(m_eTarget, level, m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, border, m_eFormat, m_eType, buf);
		break;
	case GL_TEXTURE_CUBE_MAP:
		cerr << "does not support initialization of cubemap texture" << endl;
		break;
	}
}

int glTexture::getWidth(void) const
{
	return m_iWidth;
}

int glTexture::getHeight(void) const
{
	return m_iHeight;
}

int glTexture::getDepth(void) const
{
	return m_iDepth;
}

bool glTexture::isCubeMap(void) const
{
	return m_eTarget == GL_TEXTURE_CUBE_MAP;
}

GLenum glTexture::getTarget(void) const
{
	return m_eTarget;
}

int glTexture::getInternalFormat(void) const
{
	return m_iInternalFormat;
}

GLenum glTexture::getFormat(void) const
{
	return m_eFormat;
}

GLenum glTexture::getType(void) const
{
	return m_eType;
}

GLint glTexture::getParameter(int idx) const
{
	return m_iParam[idx];
}

void glTexture::setParameter(GLenum pname, GLint param)
{
	switch ( pname )
	{
	case GL_TEXTURE_WRAP_S:
		m_iParam[WRAP_S] = param;
		break;
	case GL_TEXTURE_WRAP_T:
		m_iParam[WRAP_T] = param;
		break;
	case GL_TEXTURE_WRAP_R:
		m_iParam[WRAP_R] = param;
		break;
	case GL_TEXTURE_MIN_FILTER:
		m_iParam[MIN_FILTER] = param;
		break;
	case GL_TEXTURE_MAG_FILTER:
		m_iParam[MAG_FILTER] = param;
		break;
	case GL_DEPTH_TEXTURE_MODE:
		m_iParam[DEPTH_MODE] = param;
		break;
	case GL_TEXTURE_COMPARE_MODE:
		m_iParam[COMPARE_MODE] = param;
		break;
	case GL_TEXTURE_COMPARE_FUNC:
		m_iParam[COMPARE_FUNC] = param;
		break;
	case GL_GENERATE_MIPMAP:
		m_iParam[GENERATE_MIPMAP] = param;
		break;
	}
}

GLuint glTexture::getID(void) const
{
	return m_iTextureID;
}

bool glTexture::save(const char file_name[], bool overwrite)
{
	if ( m_iInternalFormat == GL_DEPTH_COMPONENT16 || m_iInternalFormat == GL_DEPTH_COMPONENT24 || m_iInternalFormat == GL_DEPTH_COMPONENT32 )
	{
		GLuint			fb;
		glTexture		depthTex;

		depthTex.setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		depthTex.init(GL_TEXTURE_2D, 0, GL_RGB8, getWidth(), getHeight(), 1, 0, GL_RGB, GL_UNSIGNED_BYTE);
	
		glGenFramebuffersEXT(1, &fb);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);

		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, depthTex.getID(), 0);

		glPushAttrib(GL_VIEWPORT_BIT);
		glViewport(0, 0, getWidth(), getHeight());

		GLboolean lighting;
		glGetBooleanv(GL_LIGHTING, &lighting);
		if ( lighting ) glDisable(GL_LIGHTING);

		GLint mode;
		glGetIntegerv(GL_MATRIX_MODE, &mode);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		
		setParameter(GL_TEXTURE_COMPARE_MODE, GL_NONE);
		bind(-1);

		GLboolean depth_test;
		glGetBooleanv(GL_DEPTH_TEST, &depth_test);
		if ( depth_test ) glDisable(GL_DEPTH_TEST);
		float data[][5] = {	{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f } };
		glInterleavedArrays(GL_T2F_V3F, 0, data);
		glDrawArrays(GL_QUADS, 0, 4);
		if ( depth_test ) glEnable(GL_DEPTH_TEST);

		if ( lighting ) glEnable(GL_LIGHTING);

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(mode);

		glPopAttrib();
	   	
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

		glDeleteFramebuffersEXT(1, &fb);

		depthTex.save(file_name, overwrite);
		return true;
	}

	int nChannels = get_channels(m_iInternalFormat);

	if ( m_iInternalFormat == GL_R3_G3_B2 || m_iInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || m_iInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || m_iInternalFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT ) return false;

	// Set up the header
	DDSHeader header;
	header.dwMagic = FOURCC('D', 'D', 'S', ' ');
	header.dwSize = 124;
	header.dwFlags = 0x00001007 | (m_iMipmapCount > 1 ? 0x00020000 : 0) | (m_iDepth > 1 ? 0x00800000 : 0);
	header.dwHeight = m_iHeight;
	header.dwWidth  = m_iWidth;
	header.dwPitchOrLinearSize = 0;
	header.dwDepth = (m_iDepth > 1 ? m_iDepth : 0);
	header.dwMipMapCount = (m_iMipmapCount > 1 ? m_iMipmapCount : 0);
	memset(header.dwReserved, 0, sizeof(header.dwReserved));

	header.ddpfPixelFormat.dwSize = 32;

	switch ( m_iInternalFormat )
	{
	case GL_LUMINANCE8: case GL_LUMINANCE8_ALPHA8: case GL_RGB8: case GL_RGBA8 : case GL_LUMINANCE16:
		header.ddpfPixelFormat.dwFlags = ((nChannels < 3 ? 0x00020000 : 0x00000040) | (nChannels & 1 ? 0 : 0x00000001));
		header.ddpfPixelFormat.dwFourCC = 0;
		header.ddpfPixelFormat.dwRGBBitCount = 8 * get_bytes(m_iInternalFormat);
		switch ( m_iInternalFormat )
		{
		case GL_LUMINANCE8: case GL_LUMINANCE8_ALPHA8: case GL_RGB8: case GL_RGBA8:
			header.ddpfPixelFormat.dwRBitMask = (nChannels > 2 ? 0x00FF0000 : 0xFF);
			break;
		default:
            header.ddpfPixelFormat.dwRBitMask = 0xFFFF;
			break;
		}
		header.ddpfPixelFormat.dwGBitMask = (nChannels > 1 ? 0x0000FF00 : 0);
		header.ddpfPixelFormat.dwBBitMask = (nChannels > 1 ? 0x000000FF : 0);
		header.ddpfPixelFormat.dwRGBAlphaBitMask = (nChannels == 4 ? 0xFF000000 : (nChannels == 2 ? 0xFF00 : 0));
		break;
	default:
		header.ddpfPixelFormat.dwFlags = 0x00000004;
		switch ( m_iInternalFormat )
		{
			case GL_LUMINANCE16_ALPHA16:
				header.ddpfPixelFormat.dwFourCC = 34;
				break;
			case GL_LUMINANCE32F_ARB:
				header.ddpfPixelFormat.dwFourCC = 114;
				break;
			case GL_RGBA16:
				header.ddpfPixelFormat.dwFourCC = 36;
				break;
			case GL_RGBA16F_ARB:
				header.ddpfPixelFormat.dwFourCC = 113;
				break;
			case GL_LUMINANCE_ALPHA32F_ARB:
				header.ddpfPixelFormat.dwFourCC = 115;
				break;
			case GL_RGBA32F_ARB:
				header.ddpfPixelFormat.dwFourCC = 116;
				break;
		}
		header.ddpfPixelFormat.dwRGBBitCount = 0;
		header.ddpfPixelFormat.dwRBitMask = 0;
		header.ddpfPixelFormat.dwGBitMask = 0;
		header.ddpfPixelFormat.dwBBitMask = 0;
		header.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
		break;
	}

	header.ddsCaps.dwCaps1 = 0x00001000 | (m_iMipmapCount > 1 ? 0x00400008 : 0) | (m_iDepth != 1 ? 0x00000008 : 0);
	header.ddsCaps.dwCaps2 = m_iDepth > 1 ? 0x00200000 : (m_eTarget == GL_TEXTURE_CUBE_MAP ? 0x0000FE00 : 0);
	header.ddsCaps.Reserved[0] = 0;
	header.ddsCaps.Reserved[1] = 0;
	header.dwReserved2 = 0;

	FILE *file;
	if ( (file = fopen(file_name, "r+b")) == NULL )
		if ( (file = fopen(file_name, "wb")) == NULL ) return false;
	else
		if ( !overwrite ) return false;
	
	fwrite(&header, sizeof(header), 1, file);

	int size = getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, m_iMipmapCount, m_eTarget == GL_TEXTURE_CUBE_MAP);
	unsigned char *data = new unsigned char [size];

	bind(-1);
	if ( m_eTarget == GL_TEXTURE_CUBE_MAP )
	{
		for ( int face = 0; face < 6; face++ )
		{
			unsigned char *sub_data = data + face * size / 6;
			for ( int level = 0; level < m_iMipmapCount; level++ )
			{
				int mipmap_sz = getImageSize(m_iInternalFormat, _sz(m_iWidth, level), _sz(m_iHeight, level), _sz(m_iDepth, level), 1, false);
				glGetTexImage(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, level, m_eFormat, m_eType, sub_data);
				sub_data += mipmap_sz;
			}
		}
	} else
	{		
		for ( int level = 0; level < m_iMipmapCount; level++ )
		{
			unsigned char *sub_data = data + getImageSize(m_iInternalFormat, m_iWidth, m_iHeight, m_iDepth, level, false);
			glGetTexImage(m_eTarget, level, m_eFormat, m_eType, sub_data);
		}
	}

	if ( m_iInternalFormat == GL_RGB8 || m_iInternalFormat == GL_RGBA8)
	{
		if ( (m_iInternalFormat == GL_RGB8 || m_iInternalFormat == GL_RGBA8) && header.ddpfPixelFormat.dwBBitMask == 0xFF )
		{
			unsigned char *pixels = (unsigned char *)data;
			int nPixels = size / nChannels;
			do
			{
				unsigned char tmp = pixels[2];
				pixels[2] = pixels[0];
				pixels[0] = tmp;
				pixels += nChannels;
			} while ( --nPixels );
		}
	}
	
	fwrite(data, size, 1, file);
	delete [] data;

	fclose(file);

	return true;
}

glFramebuffer::glFramebuffer()
{
	m_iFBID = -1;
	m_iRBID = -1;
	m_pTexture = NULL;
	m_bBound = false;
}

glFramebuffer::~glFramebuffer()
{
	if ( glIsFramebufferEXT(m_iFBID) ) glDeleteFramebuffersEXT(1, &m_iFBID);
	if ( glIsRenderbufferEXT(m_iRBID) ) glDeleteRenderbuffersEXT(1, &m_iFBID);
}

void glFramebuffer::setTexture(const glTexture &tex, GLenum attachment)
{
	if ( !glIsFramebufferEXT(m_iFBID) ) glGenFramebuffersEXT(1, &m_iFBID);
	m_pTexture = &tex;
	
	if ( !m_bBound ) glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_iFBID);

	switch ( tex.getTarget() )
	{
	case GL_TEXTURE_1D:
		glFramebufferTexture1DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_1D, tex.getID(), 0);
		break;
	case GL_TEXTURE_2D:
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_2D, tex.getID(), 0);
		break;
	case GL_TEXTURE_3D:
		glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, attachment, GL_TEXTURE_3D, tex.getID(), 0, 0);
		break;
	}
	
	glDrawBuffer(attachment == GL_DEPTH_ATTACHMENT_EXT ? GL_NONE : attachment);
	glReadBuffer(attachment == GL_DEPTH_ATTACHMENT_EXT ? GL_NONE : attachment);

	if ( !m_bBound ) glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void glFramebuffer::enableDepthRenderbuffer(void)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_iFBID);

	if ( !glIsRenderbufferEXT(m_iRBID) ) glGenRenderbuffersEXT(1, &m_iRBID);

	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_iRBID);
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_pTexture->getWidth(), m_pTexture->getHeight());
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_iRBID);
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void glFramebuffer::bind(void)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_iFBID);
	m_bBound = true;
}

void glFramebuffer::release(void)
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	m_bBound = false;
}

glDisplayList::glDisplayList() : m_iID(-1)
{
}

glDisplayList::~glDisplayList()
{
	if ( glIsList(m_iID) == GL_TRUE ) glDeleteLists(m_iID, 1);
}

void glDisplayList::call(void) const
{
	glCallList(m_iID);
}

void glDisplayList::newList(GLenum mode)
{
	if ( glIsList(m_iID) == GL_FALSE ) m_iID = glGenLists(1);
	else glDeleteLists(m_iID, 0);

	glNewList(m_iID, mode);
}

void glDisplayList::endList(void)
{
	glEndList();
}

glCamera::glCamera()
{
	_init(0.1f, 128.0f, 60.0f);
}

glCamera::glCamera(float tnear, float tfar, float fov)
{
	_init(tnear, tfar, fov);
}

void glCamera::setNearFar(float tnear, float tfar)
{
	m_fNear = tnear;
	m_fFar = tfar;
}

void glCamera::setFOV(float fov)
{
	m_fFOV = fov;
	m_eProjectionType = GLSL_COMMON::PERSPECTIVE;

	m_fTop = m_fDistance * tan(0.5f * fov);
	m_fBottom = -m_fTop;
	m_fLeft = -m_fTop;
	m_fRight = m_fTop;
}

void glCamera::setOrthoSize(float left, float right, float bottom, float top)
{
	m_fLeft = left;
	m_fRight = right;
	m_fBottom = bottom;
	m_fTop = top;
	m_eProjectionType = GLSL_COMMON::ORTHOGONAL;
}

float glCamera::getNear(void) const
{
	return m_fNear;
}

float glCamera::getFar(void) const
{
	return m_fFar;
}

float glCamera::getFOV(void) const
{
	return m_fFOV;
}

int glTransform::getWindowWidth(void) const
{
	return m_iWndSizeWidth;
}

int glTransform::getWindowHeight(void) const
{
	return m_iWndSizeHeight;
}

void glCamera::lookAt(float x, float y, float z, float h, float p, float r)
{
	glTransform::lookAt(x, y, z, h, p, r);
	m_fDistance = sqrt(x * x + y * y + z * z);
	setFOV(m_fFOV);
}

glCamera *glCamera::getDefaultCamera(void)
{
	if ( !m_pDefaultCamera ) m_pDefaultCamera = new glCamera();
	return m_pDefaultCamera;
}

void glCamera::applyProjection(GLSL_COMMON::ProjectionType type) const
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if ( type == GLSL_COMMON::PREDEFINED ) type = m_eProjectionType;

	switch ( type )
	{
	case GLSL_COMMON::PERSPECTIVE :
		gluPerspective((double)m_fFOV, (double)m_iWndSizeWidth / (double)m_iWndSizeHeight, (double)m_fNear, (double)m_fFar);
		break;
	case GLSL_COMMON::ORTHOGONAL :
		if ( m_iWndSizeWidth > m_iWndSizeHeight )
		{
			double center = 0.5 * (double)(m_fRight + m_fLeft);
			double width = (double)m_iWndSizeWidth / (double)m_iWndSizeHeight * (double)(m_fRight - m_fLeft);
			glOrtho(center - 0.5 * width, center + 0.5 * width, (double)m_fBottom, (double)m_fTop, (double)m_fNear, (double)m_fFar);
		} else
		{
			double center = 0.5 * (double)(m_fTop + m_fBottom);
			double height = (double)m_iWndSizeHeight / (double)m_iWndSizeWidth * (double)(m_fTop - m_fBottom);
			glOrtho((double)m_fLeft, (double)m_fRight, center - 0.5 * height, center + 0.5 * height, (double)m_fNear, (double)m_fFar);
		}
		break;
	}
	glViewport(0, 0, m_iWndSizeWidth, m_iWndSizeHeight);
	glMatrixMode(GL_MODELVIEW);
}

void glLight::applyProjection(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective((double)m_fFOV, (double)m_fAspectRatio, (double)m_fNear, (double)m_fFar);

	setProjectionMatrix(m_fFOV, m_fAspectRatio, m_fNear, m_fFar);
}

void glLight::setAspectRatio(float r)
{
	m_fAspectRatio = r;
}

void glCamera::_init(float tnear, float tfar, float fov)
{
	m_iWndSizeWidth = 640;
	m_iWndSizeHeight = 480;
	m_fNear = tnear;
	m_fFar = tfar;
	setFOV(fov);
	m_fLeft = tfar;
	m_fRight = tfar;
	m_fBottom = tfar;
	m_fTop = tfar;
}

void glCamera::_pos2spoint(float pt[3], int mx, int my)
{
	float p[3];

	p[0] = 2.0f * float(mx) / float(m_iWndSizeWidth) - 1.0f;
	p[1] = 1.0f - 2.0f * float(my) / float(m_iWndSizeHeight);
	p[2] = p[0] * p[0] + p[1] * p[1];

	if ( p[2] < 1.0f ) 
		p[2] = sqrt(1.0f - p[2]);
	else
	{
		p[2] = sqrt(p[2]);
		p[0] /= p[2];
		p[1] /= p[2];
		p[2] = 0.0f;
	}

	pt[0] = m_fPrevTransformMatrix[0] * p[0] + m_fPrevTransformMatrix[4] * p[1] + m_fPrevTransformMatrix[8] * p[2];
	pt[1] = m_fPrevTransformMatrix[1] * p[0] + m_fPrevTransformMatrix[5] * p[1] + m_fPrevTransformMatrix[9] * p[2];
	pt[2] = m_fPrevTransformMatrix[2] * p[0] + m_fPrevTransformMatrix[6] * p[1] + m_fPrevTransformMatrix[10] * p[2];
}

void glCamera::_pos2mv_point(float mv[3], int mx, int my, int z)
{
	mv[0] = (2.0f * (float)mx / (float)m_iWndSizeWidth - 1.0f) * (m_fRight - m_fLeft) * 0.5f;
	mv[1] = (2.0f * (float)my / (float)m_iWndSizeHeight - 1.0f) * (m_fTop - m_fBottom) * -0.5f;
	mv[2] = (2.0f * (float)z / (float)m_iWndSizeWidth - 1.0f) * (m_fRight - m_fLeft) * 0.5f;
}

void glCamera::mouseFunc(GLSL_COMMON::MouseButtonType button, int state, int x, int y)
{
	memcpy(m_fPrevTransformMatrix, m_fTransformMatrix, sizeof(m_fTransformMatrix));
	_pos2spoint(m_fPrevSPoint, x, y);
	_pos2mv_point(m_fPrevTransformPoint, x, y, 0);
	m_iZoom = y;
	m_eMouseButton = button;
}

void glCamera::motionFunc(int x, int y)
{
	static float _cur_spoint[3];
	static float _cur_mv_point[3];

	switch ( m_eMouseButton )
	{
	case GLSL_COMMON::LEFT :
		_pos2spoint(_cur_spoint, x, y);
		glPushMatrix();
		glLoadIdentity();
		glRotatef(-_RTD_ * acos( m_fPrevSPoint[0] * _cur_spoint[0] + m_fPrevSPoint[1] * _cur_spoint[1] + m_fPrevSPoint[2] * _cur_spoint[2] ), m_fPrevSPoint[1] * _cur_spoint[2] - m_fPrevSPoint[2] * _cur_spoint[1], m_fPrevSPoint[2] * _cur_spoint[0] - m_fPrevSPoint[0] * _cur_spoint[2], m_fPrevSPoint[0] * _cur_spoint[1] - m_fPrevSPoint[1] * _cur_spoint[0]);
		glMultMatrixf(m_fPrevTransformMatrix);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_fTransformMatrix);
		glPopMatrix();
		break;
	case GLSL_COMMON::RIGHT :
		_pos2mv_point(_cur_mv_point, x, y, 0);
		memcpy(m_fTransformMatrix, m_fPrevTransformMatrix, sizeof(m_fTransformMatrix));
		_cur_mv_point[0] -= m_fPrevTransformPoint[0];
		_cur_mv_point[1] -= m_fPrevTransformPoint[1];
		m_fTransformMatrix[12] -= m_fPrevTransformMatrix[0] * _cur_mv_point[0] + m_fPrevTransformMatrix[4] * _cur_mv_point[1];
		m_fTransformMatrix[13] -= m_fPrevTransformMatrix[1] * _cur_mv_point[0] + m_fPrevTransformMatrix[5] * _cur_mv_point[1];
		m_fTransformMatrix[14] -= m_fPrevTransformMatrix[2] * _cur_mv_point[0] + m_fPrevTransformMatrix[6] * _cur_mv_point[1];
		break;
	case GLSL_COMMON::MIDDLE :
		_pos2mv_point(_cur_mv_point, 0, 0, m_iZoom - y);
		memcpy(m_fTransformMatrix, m_fPrevTransformMatrix, sizeof(m_fTransformMatrix));
		m_fTransformMatrix[12] -= m_fPrevTransformMatrix[8] * (_cur_mv_point[2] - m_fPrevTransformPoint[2]);
		m_fTransformMatrix[13] -= m_fPrevTransformMatrix[9] * (_cur_mv_point[2] - m_fPrevTransformPoint[2]);
		m_fTransformMatrix[14] -= m_fPrevTransformMatrix[10] * (_cur_mv_point[2] - m_fPrevTransformPoint[2]);
		break;
	}
}

void glCamera::reshapeFunc(int w, int h)
{
	setWindowSize(w, h);
	applyProjection();
}

glShader::glShader()
{
}

inline unsigned int _pow_two(unsigned int x)
{
	unsigned int y = 1;
	while ( x-- ) y <<= 1;
	return y;
}

glShader::~glShader()
{
	list <glProgram *>::iterator itor = m_pProgramList.begin();
	while ( itor != m_pProgramList.end() ) (*(itor++))->_detach(this);
	for ( unsigned int i = 0; i < m_iShaderIDs.size(); i++ ) glDeleteShader(m_iShaderIDs[i]);
}

GLuint glShader::addDefinition(const GLchar *definition)
{
	m_szDefinitions.push_back(string(definition));
	return _pow_two(m_szDefinitions.size() - 1);
}

GLuint glShader::getID(GLuint mask) const
{
	return m_iShaderIDs.size() > mask ? m_iShaderIDs[mask] : 0;
}

void glShader::_loadShader(void)
{
	unsigned int i, j, slot, num = _pow_two(m_szDefinitions.size());
	m_iShaderIDs.resize(num);

	for ( i = 0; i < num; i++ )
	{
		string buf;
		m_iShaderIDs[i] = glCreateShader(m_eShaderType);
		slot = i;

		for ( j = 0; j < m_szDefinitions.size(); j++ )
		{
			if ( _pow_two(j) & slot )
			{
				buf += m_szDefinitions[j];
				buf += "\n";
			}
		}
		buf += m_szShaderSrc;

		const char *tmp = buf.c_str();

		glShaderSource(m_iShaderIDs[i], 1, (const GLchar **)(&tmp), NULL);
		glCompileShader(m_iShaderIDs[i]);
	}
}


void glShader::loadShaderSource(GLenum shaderType, const GLchar *shader_src, bool isFile)
{
	m_eShaderType = shaderType;

	if ( isFile )
	{
		ifstream fin;

		fin.open(shader_src, ios::binary);

		if( !fin.good() )
		{
			// try to find the source file at a parent directory.
			GLchar shader_src_at_parent[1024];
			strcpy(shader_src_at_parent, "../");
			strcat(shader_src_at_parent, shader_src);

			fin.clear();
			fin.open(shader_src_at_parent, ios::binary);
			if( !fin.good() )
			{
				cerr << "can not open shader source file " << shader_src << endl;
				return;
			}
		}

		while ( fin.good() )
		{
			char buf[1024];
			fin.getline(buf, 1024);
			m_szShaderSrc += buf;
			m_szShaderSrc += "\n";
		}

		fin.close();
	} else
		m_szShaderSrc = shader_src;

	_loadShader();
}

const char *glShader::getInfoLog(void)
{
	static char szLog[4096];
	GLsizei slen, last = 0;

	for ( unsigned int i = 0; i < m_iShaderIDs.size(); i++ )
	{
		if ( m_iShaderIDs[i] )
		{
			glGetShaderInfoLog(m_iShaderIDs[i], 4096, &slen, szLog + last);
			last += slen;
		}
	}
	szLog[last] = NULL;
	return szLog;
}

glProgram::glProgram() : m_bLinked(false), m_bEnabled(false), m_iProgramID(0)
{
}

bool glProgram::isEnabled(void) const
{
	return m_bEnabled;
}

glProgram::~glProgram()
{
	list <SHADER_PAIR>::iterator it = m_pShaderList.begin();
	while ( it != m_pShaderList.end() ) (it++)->pShader->m_pProgramList.remove(this);

	glDeleteProgram(m_iProgramID);

	list <UNIFORM_PAIR *>::iterator itor = m_pUniformList.begin();
	while ( itor != m_pUniformList.end() ) delete [] (*(itor++))->val;
}

void glProgram::_detach(glShader *pShader)
{
	list <SHADER_PAIR>::iterator itor = m_pShaderList.begin();
	while ( itor != m_pShaderList.end() )
	{
		if ( itor->pShader == pShader ) glDetachShader(m_iProgramID, pShader->m_iShaderIDs[itor->mask]);
		itor++;
	}
}

void glProgram::attach(glShader *pShaderA, glShader *pShaderB)
{
	if ( !m_iProgramID ) m_iProgramID = glCreateProgram();

	SHADER_PAIR pair;
	if ( pShaderA && pShaderA->getID(0) )
	{
		m_bLinked = false;
		pShaderA->m_pProgramList.push_back(this);
		pair.pShader = pShaderA;
		pair.mask = 0;
		m_pShaderList.push_back(pair);
		glAttachShader(m_iProgramID, pShaderA->getID(0));
	}

	if ( pShaderB && pShaderB->getID(0) )
	{
		m_bLinked = false;
		pShaderB->m_pProgramList.push_back(this);
		pair.pShader = pShaderB;
		pair.mask = 0;
		m_pShaderList.push_back(pair);
		glAttachShader(m_iProgramID, pShaderB->getID(0));
	}
}

void glProgram::setMask(glShader *pShader, GLuint mask)
{
	if ( pShader && pShader->getID(mask) )
	{
		m_bLinked = false;

		list <SHADER_PAIR>::iterator itor = m_pShaderList.begin();
		while ( itor != m_pShaderList.end() )
		{
			if ( itor->pShader == pShader )
			{
				glDetachShader(m_iProgramID, pShader->m_iShaderIDs[itor->mask]);
				itor->mask = mask;
				glAttachShader(m_iProgramID, pShader->m_iShaderIDs[itor->mask]);
			}
			itor++;
		}
	}
}

void glProgram::_link(void)
{
	if ( m_iProgramID && !m_bLinked )
	{
		glLinkProgram(m_iProgramID);
		m_bLinked = true;
	}
}

const char *glProgram::getInfoLog(void)
{
	_link();

	static char szLog[4096];
	GLsizei slen;

	if ( m_iProgramID )
	{
		glGetProgramInfoLog(m_iProgramID, 4096, &slen, szLog);
	} else szLog[0] = NULL;

	return szLog;
}

void glProgram::enable(bool flag)
{
	if ( flag )
	{
		_link();
		glUseProgram(m_iProgramID);
		_applyUniform();

		m_bEnabled = true;
	} else
		disable();
}

void glProgram::disable(void)
{
	glUseProgram(NULL);
	m_bEnabled = false;
}

void glProgram::_setUniform(const GLchar name[], UNIFORM_PAIR::UNIFORM_TYPE type, int count, int dim, const void *val)
{
	list <UNIFORM_PAIR *>::iterator itor = m_pUniformList.begin();
	int size = count * dim;
	
	while ( itor != m_pUniformList.end() )
	{
		if ( (*itor)->name == string(name) ) 
		{
			(*itor)->type = type;
			(*itor)->count = count;
			(*itor)->dim = dim;
			switch ( type )
			{
			case UNIFORM_PAIR::UNIFORM_TYPE::INTEGER:
				delete [] (*itor)->val;
				(*itor)->val = new int [size];
				memcpy((*itor)->val, val, size * sizeof(int));
				break;
			case UNIFORM_PAIR::UNIFORM_TYPE::FLOAT:
				delete [] (*itor)->val;
				(*itor)->val = new float [size];
				memcpy((*itor)->val, val, size * sizeof(float));
				break;
			case UNIFORM_PAIR::UNIFORM_TYPE::MATRIX:
				delete [] (*itor)->val;
				(*itor)->val = new float [size * size];
				memcpy((*itor)->val, val, size * size * sizeof(float));
				break;
			}
			if ( m_bEnabled ) _applyUniform(*itor);
			return;
		}
		itor++;
	}

	UNIFORM_PAIR *pPair = new UNIFORM_PAIR;
	pPair->name = string(name);
	pPair->type = type;
	pPair->count = count;
	pPair->dim = dim;
	switch ( type )
	{
	case UNIFORM_PAIR::UNIFORM_TYPE::INTEGER:
		pPair->val = new int [size];
		memcpy(pPair->val, val, size * sizeof(int));
		break;
	case UNIFORM_PAIR::UNIFORM_TYPE::FLOAT:
		pPair->val = new float [size];
		memcpy(pPair->val, val, size * sizeof(float));
		break;
	case UNIFORM_PAIR::UNIFORM_TYPE::MATRIX:
		pPair->val = new float [size * size];
		memcpy(pPair->val, val, size * size * sizeof(float));
		break;
	}

	m_pUniformList.push_back(pPair);
	if ( m_bEnabled ) _applyUniform(pPair);
}

void glProgram::_applyUniform(const UNIFORM_PAIR *pPair) const
{
	if ( !pPair )
	{
		list <UNIFORM_PAIR *>::const_iterator itor = m_pUniformList.begin();
		while ( itor != m_pUniformList.end() ) _applyUniform(*itor++);
		return;
	}

	switch ( pPair->type )
	{
	case UNIFORM_PAIR::UNIFORM_TYPE::INTEGER:
		switch ( pPair->dim )
		{
		case 1:
			glUniform1iv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const int *)(pPair->val));
			break;
		case 2:
			glUniform2iv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const int *)(pPair->val));
			break;
		case 3:
			glUniform3iv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const int *)(pPair->val));
			break;
		case 4:
			glUniform4iv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const int *)(pPair->val));
			break;
		}
		break;
	case UNIFORM_PAIR::UNIFORM_TYPE::FLOAT:
		switch ( pPair->dim )
		{
		case 1:
			glUniform1fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const float *)(pPair->val));
			break;
		case 2:
			glUniform2fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const float *)(pPair->val));
			break;
		case 3:
			glUniform3fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const float *)(pPair->val));
			break;
		case 4:
			glUniform4fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, (const float *)(pPair->val));
			break;
		}
		break;
	case UNIFORM_PAIR::UNIFORM_TYPE::MATRIX:
		switch ( pPair->dim )
		{
		case 2:
			glUniformMatrix2fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, GL_FALSE, (const float *)(pPair->val));
			break;
		case 3:
			glUniformMatrix3fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, GL_FALSE, (const float *)(pPair->val));
			break;
		case 4:
			glUniformMatrix4fv(glGetUniformLocation(m_iProgramID, pPair->name.c_str()), pPair->count, GL_FALSE, (const float *)(pPair->val));
			break;
		}
		break;
	}
}

void glProgram::setUniform(const GLchar name[], int v0)
{
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::INTEGER, 1, 1, &v0);
}

void glProgram::setUniform(const GLchar name[], int v0, int v1)
{
	int v[] = { v0, v1 };
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::INTEGER, 1, 2, v);
}

void glProgram::setUniform(const GLchar name[], int v0, int v1, int v2)
{
	int v[] = { v0, v1, v2 };
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::INTEGER, 1, 3, v);
}

void glProgram::setUniform(const GLchar name[], int v0, int v1, int v2, int v3)
{
	int v[] = { v0, v1, v2, v3 };
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::INTEGER, 1, 4, v);
}

void glProgram::setUniform(const GLchar name[], int count, int dim, const int *v)
{
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::INTEGER, count, dim, v);
}

void glProgram::setUniform(const GLchar name[], float v0)
{
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::FLOAT, 1, 1, &v0);
}

void glProgram::setUniform(const GLchar name[], float v0, float v1)
{
	float v[] = { v0, v1 };
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::FLOAT, 1, 2, v);
}

void glProgram::setUniform(const GLchar name[], float v0, float v1, float v2)
{
	float v[] = { v0, v1, v2 };
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::FLOAT, 1, 3, v);
}

void glProgram::setUniform(const GLchar name[], float v0, float v1, float v2, float v3)
{
	float v[] = { v0, v1, v2, v3 };
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::FLOAT, 1, 4, v);
}

void glProgram::setUniform(const GLchar name[], int count, int dim, const float *v)
{
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::FLOAT, count, dim, v);
}

void glProgram::setUniformMatrix(const GLchar name[], int dim, const float *v)
{
	_setUniform(name, UNIFORM_PAIR::UNIFORM_TYPE::MATRIX, 1, dim, v);
}

void glProgram::getUniform(const GLchar name[], GLfloat values[])
{	
	glGetUniformfv(m_iProgramID, glGetUniformLocation(m_iProgramID, name), values);
}

void glProgram::getUniform(const GLchar name[], GLint values[])
{	
	glGetUniformiv(m_iProgramID, glGetUniformLocation(m_iProgramID, name), values);
}

GLuint glText::base = 0;

void glText::print(int x, int y, const char * string, ...)
{
	static GLuint base = 0;

	if ( !base )
	{
		base = glGenLists(255);

		HFONT font = CreateFont(20,								// Height Of Font
			0,								// Width Of Font
			0,								// Angle Of Escapement
			0,								// Orientation Angle
			FW_BOLD,						// Font Weight
			FALSE,							// Italic
			FALSE,							// Underline
			FALSE,							// Strikeout
			ANSI_CHARSET,					// Character Set Identifier
			OUT_TT_PRECIS,					// Output Precision
			CLIP_DEFAULT_PRECIS,			// Clipping Precision
			ANTIALIASED_QUALITY,			// Output Quality
			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
			L"Arial");						// Font Name

		HFONT oldfont = (HFONT)SelectObject(wglGetCurrentDC(), font);
		wglUseFontBitmaps(wglGetCurrentDC(), 0, 255, base);
		SelectObject(wglGetCurrentDC(), oldfont);
		DeleteObject(font);
	}

	char text[1024];
	va_list va;

	if ( !string ) return;

	va_start(va, string);
	vsprintf(text, string, va);
	va_end(va);

	glWindowPos2i(x, y);

	glPushAttrib(GL_LIST_BIT);
	glListBase(base);
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);
	glPopAttrib();
}

void glText::setFont(const wchar_t font_name[], int font_height, int font_width)
{
	if ( base ) glDeleteLists(base, 255);
	base = glGenLists(255);

	HFONT font = CreateFont(font_height, font_width, 0, 0, FW_BOLD, 0, 0, 0, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, font_name);
	HFONT oldfont = (HFONT)SelectObject(wglGetCurrentDC(), font);
	wglUseFontBitmaps(wglGetCurrentDC(), 0, 255, base);
	SelectObject(wglGetCurrentDC(), oldfont);
	DeleteObject(font);
}

float		glTimer::m_fResolution = 0.0f;
int			glTimer::m_nLowshift = 0;
LONGLONG	glTimer::m_sStart;
glTimer		glTimer::m_sDefaultTimer;
bool		glTimer::m_bSuspend = false;
LONGLONG	glTimer::m_sSuspend;

inline LONGLONG __clock()
{
	LARGE_INTEGER Count;
	QueryPerformanceCounter(&Count);
	return Count.QuadPart;
}

glTimer::glTimer()
{
	if ( m_fResolution == 0 || m_nLowshift == 0 )
	{
		LARGE_INTEGER m_Frequency;
		QueryPerformanceFrequency(&m_Frequency);
		LONGLONG nShift = m_Frequency.QuadPart;
		m_nLowshift = 0;
		while ( nShift > 1000000 )
		{
			m_nLowshift++;
			nShift >>= 1;
		}
		m_fResolution = 1.0f / (float)nShift;
	}

	m_sStart = __clock();
}

void glTimer::tic(void)
{
	m_sStart = __clock();
}

float glTimer::toc(void )
{
	if ( m_bSuspend ) return (float)((m_sSuspend - m_sStart) >> m_nLowshift) * m_fResolution;
	return (float)((__clock() - m_sStart) >> m_nLowshift) * m_fResolution;
}

float glTimer::getFPS(void)
{
	const int QSIZE = 30;
	static LONGLONG Qsec[QSIZE];
	static int Qidx = 0;
	Qsec[Qidx] = __clock();
	float fps = (float)QSIZE / ((float)((Qsec[Qidx] - Qsec[(Qidx + 1) % QSIZE]) >> m_nLowshift) * m_fResolution);
	++Qidx %= QSIZE;
	return fps;
}

void glTimer::suspend(void)
{
	if ( !m_bSuspend )
	{
		m_sSuspend = __clock();
		m_bSuspend = true;
	}
}

void glTimer::resume(void)
{
	if ( m_bSuspend ) 
	{
		m_sStart += (__clock() - m_sSuspend);
		m_bSuspend = false;
	}
}

glColor::glColor()
{
	m_fVal[0] = m_fVal[1] = m_fVal[2] = m_fVal[3] = 1.0f;
}

glColor::glColor(float r, float g, float b, float a)
{
	m_fVal[0] = r;
	m_fVal[1] = g;
	m_fVal[2] = b;
	m_fVal[3] = a;
}

const float &glColor::operator [] (int i) const
{
	return m_fVal[i];
}

float &glColor::operator [] (int i)
{
	return m_fVal[i];
}

static float _Hue2RGB(float m1, float m2, float h)
{
	if ( h < 0.0f ) h += 1.0f;
	else if ( h > 1.0f ) h -= 1.0f;
	
	if ( h < 0.1666667f ) return (m1 + (m2 - m1) * h * 6.0f);
	else if ( h < 0.5f ) return m2;
	else if ( h < 0.6666667f) return (m1 + (m2 - m1) * (4.0f - 6.0f * h));
	
	return m1;
}

glColor HSL2RGB(const glColor &HSL)
{
	if ( HSL[1] == 0.0f ) return glColor(HSL[2], HSL[2], HSL[2]);
	
	float m1, m2;
	if ( HSL[2] <= 0.5f ) m2 = HSL[2] * (1.0f + HSL[1]);
	else m2 = HSL[2] + HSL[1] - HSL[2] * HSL[1];
	m1 = 2.0f * HSL[2] - m2;
	return glColor(_Hue2RGB(m1, m2, HSL[0] + 0.33333333f), _Hue2RGB(m1, m2, HSL[0]), _Hue2RGB(m1, m2, HSL[0] - 0.33333333f));
}

glColor RGB2HSL(const glColor &rgb)
{
	glColor hsl;

	float *HSL = &hsl.m_fVal[0];
	const float *RGB = &rgb.m_fVal[0];

	float cmax = max(RGB[0], max(RGB[1], RGB[2]));
	float cmin = min(RGB[0], min(RGB[1], RGB[2]));

	HSL[2] = (cmax + cmin) / 2.0f;

	if ( cmax == cmin )
	{
		HSL[1] = 0.0f;
		HSL[0] = 0.0f;
	} else 
	{
		float delta = cmax - cmin;

		if ( HSL[2] < 0.5f ) HSL[1] = delta / (cmax + cmin);
		else HSL[1] = delta / (2.0f - cmax - cmin);

		if ( RGB[0] == cmax ) HSL[0] = (RGB[1] - RGB[2]) / delta;
		else if ( RGB[1] == cmax ) HSL[0] = 2.0f + (RGB[2] - RGB[0]) / delta;
		else HSL[0] = 4.0f + (RGB[0] - RGB[1]) / delta;

		HSL[0] /= 6.0f;
		if ( HSL[0] < 0.0f ) HSL[0] += 1.0f;
	}
	return hsl;
}

void drawBox(float x, float y, float z, float texc)
{
	x *= 0.5f;
	y *= 0.5f;
	z *= 0.5f;

	float data[][8] = {	{  0.0f,  0.0f,  0.0f,  0.0f,  1.0f, -x, -y,  z },
	{  texc,  0.0f,  0.0f,  0.0f,  1.0f,  x, -y,  z },
	{  texc,  texc,  0.0f,  0.0f,  1.0f,  x,  y,  z },
	{  0.0f,  texc,  0.0f,  0.0f,  1.0f, -x,  y,  z },
	{  texc,  0.0f,  0.0f,  0.0f, -1.0f, -x, -y, -z },
	{  texc,  texc,  0.0f,  0.0f, -1.0f, -x,  y, -z },
	{  0.0f,  texc,  0.0f,  0.0f, -1.0f,  x,  y, -z },
	{  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  x, -y, -z },
	{  0.0f,  texc,  0.0f,  1.0f,  0.0f, -x,  y, -z },
	{  0.0f,  0.0f,  0.0f,  1.0f,  0.0f, -x,  y,  z },
	{  texc,  0.0f,  0.0f,  1.0f,  0.0f,  x,  y,  z },
	{  texc,  texc,  0.0f,  1.0f,  0.0f,  x,  y, -z },
	{  texc,  texc,  0.0f, -1.0f,  0.0f, -x, -y, -z },
	{  0.0f,  texc,  0.0f, -1.0f,  0.0f,  x, -y, -z },
	{  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  x, -y,  z },
	{  texc,  0.0f,  0.0f, -1.0f,  0.0f, -x, -y,  z },
	{  texc,  0.0f,  1.0f,  0.0f,  0.0f,  x, -y, -z },
	{  texc,  texc,  1.0f,  0.0f,  0.0f,  x,  y, -z },
	{  0.0f,  texc,  1.0f,  0.0f,  0.0f,  x,  y,  z },
	{  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  x, -y,  z },
	{  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -x, -y, -z },
	{  texc,  0.0f, -1.0f,  0.0f,  0.0f, -x, -y,  z },
	{  texc,  texc, -1.0f,  0.0f,  0.0f, -x,  y,  z },
	{  0.0f,  texc, -1.0f,  0.0f,  0.0f, -x,  y, -z }	};

	glInterleavedArrays(GL_T2F_N3F_V3F, 0, data);
	glDrawArrays(GL_QUADS, 0, 24);
}

void drawTorus(float ro, float ri, int longitude, int latitude)
{
	longitude = max(7, longitude) + 1;
	latitude = max(7, latitude) + 1;

	int i, j;
	float *normal = new float [longitude * latitude * 3];
	float *vertex = new float [longitude * latitude * 3];
	float *tex = new float [longitude * latitude * 2];
	float ct, st, cp, sp;

	for ( i = 0; i < longitude; i++ )
	{
		ct = cos(2.0f * (float)M_PI * (float)i / (float)(longitude - 1));
		st = sin(2.0f * (float)M_PI * (float)i / (float)(longitude - 1));

		for ( j = 0; j < latitude; j++ )
		{
			cp = cos(2.0f * (float)M_PI * (float)j / (float)(latitude - 1));
			sp = sin(2.0f * (float)M_PI * (float)j / (float)(latitude - 1));

			normal[3 * (i + longitude * j)    ] = cp * ct;
			normal[3 * (i + longitude * j) + 1] = sp * ct;
			normal[3 * (i + longitude * j) + 2] = st;

			tex[2 * (i + longitude * j)    ] = 1.0f * (float)i / (float)(longitude - 1);
			tex[2 * (i + longitude * j) + 1] = 5.0f * (float)j / (float)(latitude - 1);

			vertex[3 * (i + longitude * j)    ] = cp * (ro  + ri * ct);
			vertex[3 * (i + longitude * j) + 1] = sp * (ro  + ri * ct);
			vertex[3 * (i + longitude * j) + 2] = ri * st;
		}
	}

	glBegin(GL_QUADS);
	for ( i = 0; i < longitude - 1; i++ )
		for ( j = 0; j < latitude - 1; j++ )
		{
			glNormal3fv(normal + 3 * (i + longitude * j));
			glMultiTexCoord2fv(GL_TEXTURE0, tex +  2 * (i + longitude * j));
			glVertex3fv(vertex + 3 * (i + longitude * j));

			glNormal3fv(normal + 3 * (i + 1 + longitude * j));
			glMultiTexCoord2fv(GL_TEXTURE0, tex +  2 * (i + 1 + longitude * j));
			glVertex3fv(vertex + 3 * (i + 1 + longitude * j));

			glNormal3fv(normal + 3 * (i + 1 + longitude * (j + 1)));
			glMultiTexCoord2fv(GL_TEXTURE0, tex +  2 * (i + 1 + longitude * (j + 1)));
			glVertex3fv(vertex + 3 * (i + 1 + longitude * (j + 1)));

			glNormal3fv(normal + 3 * (i + longitude * (j + 1)));
			glMultiTexCoord2fv(GL_TEXTURE0, tex +  2 * (i + longitude * (j + 1)));
			glVertex3fv(vertex + 3 * (i + longitude * (j + 1)));
		}
		glEnd();

		delete [] normal;
		delete [] tex;
		delete [] vertex;
}


glMaterial::glMaterial()
{
	m_sAmbient = glColor(0.2f, 0.2f, 0.2f, 1.0f);
	m_sDiffuse = glColor(0.8f, 0.8f, 0.8f, 1.0f);
	m_sSpecular = glColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_sEmission = glColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_eFace = GL_FRONT;
	m_fShininess = 0.0f;
}

void glMaterial::setFace(GLenum face)
{
	m_eFace = face;
}

void glMaterial::setAmbient(const glColor &color)
{
	m_sAmbient = color;
}

void glMaterial::setDiffuse(const glColor &color)
{
	m_sDiffuse = color;
}

void glMaterial::setSpecular(const glColor &color)
{
	m_sSpecular = color;
}

void glMaterial::setEmission(const glColor &color)
{
	m_sEmission = color;
}

void glMaterial::setShininess(float shininess)
{
	m_fShininess = shininess;
}

void glMaterial::apply(void) const
{
	glMaterialfv(m_eFace, GL_AMBIENT, &m_sAmbient[0]);
	glMaterialfv(m_eFace, GL_DIFFUSE, &m_sDiffuse[0]);
	glMaterialfv(m_eFace, GL_SPECULAR, &m_sSpecular[0]);
	glMaterialfv(m_eFace, GL_EMISSION, &m_sEmission[0]);
	glMaterialfv(m_eFace, GL_SHININESS, &m_fShininess);
}

glModel::glModel()
{
	m_pProgram = NULL;
	m_pMaterial = NULL;
	for ( int i = 0; i < 8; i++ ) m_pTexture[i] = NULL;
}

void glModel::setProgram(glProgram *program)
{
	m_pProgram = program;
}

void glModel::setTexture(glTexture *texture, int unit)
{
	m_pTexture[unit] = texture;
}

void glModel::setMaterial(glMaterial *material)
{
	m_pMaterial = material;
}

void glModel::draw(bool enTex) const
{
	if ( m_pMaterial )
	{
		glPushAttrib(GL_LIGHTING_BIT);
		m_pMaterial->apply();
	}

	if ( enTex )
	{
		for ( int i = 0; i < MAX_TEXTURE; i++ )
		{
			if ( m_pTexture[i] ) m_pTexture[i]->bind(i);
			else
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glDisable(GL_TEXTURE_1D);
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_TEXTURE_3D);
			}
		}
	}

	if ( m_pProgram ) m_pProgram->enable();
	call();
	if ( m_pProgram ) m_pProgram->disable();

	if ( m_pMaterial ) glPopAttrib();
}
