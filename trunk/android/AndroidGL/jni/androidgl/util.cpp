#include "util.h"
#include "log.h"



zip_file* file;

zip* loadAPK (const char* apkPath) {
  LOGI("AndroidGL","Loading APK %s", apkPath);
  zip* APKArchive = zip_open(apkPath, 0, NULL);
  if (APKArchive == NULL) {
    LOGE("AndroidGL","Error loading APK",0);
    return NULL;
  }

  //Just for debug, print APK contents
  int numFiles = zip_get_num_files(APKArchive);
  for (int i=0; i<numFiles; i++) {
    const char* name = zip_get_name(APKArchive, i, 0);
    if (name == NULL) {
      LOGE("AndroidGL","Error reading zip file name at index %i : %s", zip_strerror(APKArchive));
      return NULL;
    }
    LOGI("AndroidGL","File %i : %s\n", i, name);
  }

  return APKArchive;
}


void pngZipRead(png_structp png_ptr, png_bytep data, png_size_t length) {
  zip_fread(file, data, length);
}
/**
 * 从apk中的指定文件读取出byte颜色数据，这些数据可直接用于glTexImage2D,
 * 除此之外还返回图像的width,heigth及colryType(暂支持GL_RGB跟GL_RGBA)
 */
png_byte* readPngData(const char* filename,int& width,int& height,int& colorType,zip* apk)
{
	file=zip_fopen(apk, filename, 0);
	if(!file)
	{
		LOGE("Util","Error opening %s from APK",filename);
		return NULL;
	}

	//header for testing if it is a png
	png_byte header[8];

	//read the header
	zip_fread(file, header, 8);

	//test if png
	int is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		zip_fclose(file);
		LOGE("Utils","Not a png file : %s", filename);
		return NULL;
	}

	//create png struct
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,NULL, NULL);
	if (!png_ptr) {
		zip_fclose(file);
		LOGE("Utils","Unable to create png struct : %s", filename);
		return NULL;
	}

	//create png info struct
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		LOGE("Utils","Unable to create png info : %s", filename);
		zip_fclose(file);
		return NULL;
	}

	//create png info struct
	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		LOGE("Utils","Unable to create png end info : %s", filename);
		zip_fclose(file);
		return NULL;
	}

	//png error stuff, not sure libpng man suggests this.
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		zip_fclose(file);
		LOGE("Utils","Error during setjmp : %s", filename);
		return NULL;
	}

	//init png reading
	//png_init_io(png_ptr, fp);
	png_set_read_fn(png_ptr, NULL, pngZipRead);

	//let libpng know you already read the first 8 bytes
	png_set_sig_bytes(png_ptr, 8);

	// read all the info up to the image data
	png_read_info(png_ptr, info_ptr);

	//variables to pass to get info
	int bit_depth, color_type;
	png_uint_32 twidth, theight;

	// get info about png
	png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,NULL, NULL, NULL);
	LOGI("Util","bitDepth:%d,colorType:%d,width:%d,height:%d",bit_depth,color_type,twidth,theight);

	//update width 、height and colorType based on png info
	width = twidth;
	height = theight;

	switch(color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		LOGI("Util","colorType:PNG_COLOR_TYPE_GRAY",NULL);
		png_set_gray_to_rgb(png_ptr);
		break;
	case PNG_COLOR_TYPE_RGB:
		LOGI("Util","colorType:PNG_COLOR_TYPE_RGB",NULL);
		colorType=GL_RGB;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		LOGI("Util","colorType:PNG_COLOR_TYPE_RGB_ALPHA",NULL);
		colorType=GL_RGBA;
		break;
	case PNG_COLOR_TYPE_PALETTE:
		LOGI("Util","colorType:PNG_COLOR_TYPE_PALETT",NULL);
		png_set_palette_to_rgb (png_ptr);
		break;

	}
	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		LOGI("Util","PNG_INFO_tRNS valid",NULL);
		png_set_tRNS_to_alpha (png_ptr);
	}
	if (bit_depth == 16)
	{
		LOGI("Util","bitDepth==16",NULL);
		png_set_strip_16 (png_ptr);
	}
	else if (bit_depth < 8)
	{
		LOGI("Util","bitDepth<8",NULL);
		png_set_packing (png_ptr);
	}

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Row size in bytes.
	int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

	// Allocate the image_data as a big block, to be given to opengl
	png_byte *image_data = new png_byte[rowbytes * height];
	if (!image_data) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		LOGE("Utils","Unable to allocate image_data while loading %s ", filename);
		zip_fclose(file);
		return NULL;
	}

	//row_pointers is for pointing to image_data for reading the png with libpng
	png_bytep *row_pointers = new png_bytep[height];
	if (!row_pointers) {
		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] image_data;
		LOGE("Utils","Unable to allocate row_pointer while loading %s ", filename);
		zip_fclose(file);
		return NULL;
	}
	// set the individual row_pointers to point at the correct offsets of image_data
	for (int i = 0; i < height; ++i)
		row_pointers[height - 1 - i] = image_data + i * rowbytes;

	//read the png into image_data through row_pointers
	png_read_image(png_ptr, row_pointers);

	//clean up memory and close stuff
	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	delete[] row_pointers;
	zip_fclose(file);

	return image_data;
}

void gluPerspective(GLfloat fovy, GLfloat aspect,
                           GLfloat zNear, GLfloat zFar)
{
    GLfloat xmin, xmax, ymin, ymax;

    ymax = zNear * (GLfloat)tan(fovy * M_PI / 360);
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustumx((GLfixed)(xmin * 65536), (GLfixed)(xmax * 65536),
               (GLfixed)(ymin * 65536), (GLfixed)(ymax * 65536),
               (GLfixed)(zNear * 65536), (GLfixed)(zFar * 65536));
}

/* Following gluLookAt implementation is adapted from the
 * Mesa 3D Graphics library. http://www.mesa3d.org
 */
void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	              GLfloat centerx, GLfloat centery, GLfloat centerz,
	              GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;

    /* Make rotation matrix */

    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = (float)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {			/* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }

    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;

    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];

    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];

    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */

    mag = (float)sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }

    mag = (float)sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }

#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    {
        int a;
        GLfixed fixedM[16];
        for (a = 0; a < 16; ++a)
            fixedM[a] = (GLfixed)(m[a] * 65536);
        glMultMatrixx(fixedM);
    }

    /* Translate Eye to Origin */
    glTranslatex((GLfixed)(-eyex * 65536),
                 (GLfixed)(-eyey * 65536),
                 (GLfixed)(-eyez * 65536));
}

// Capped conversion from float to fixed.
long floatToFixed(float value)
{
    if (value < -32768) value = -32768;
    if (value > 32767) value = 32767;
    return (long)(value * 65536);
}

void printGLInteger(const char *name, GLenum s) {
	GLint result;
	glGetIntegerv(s,&result);
    LOGI("AndroidGL","GL %s = %d\n", name, result);
}

void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("AndroidGL","GL %s = %s\n", name, v);
}

/*
* Register several native methods for one class.
*/

int registerNativeMethods(JNIEnv* env, const char* className,
    JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    fprintf(stderr, "RegisterNatives start for '%s'", className);
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        fprintf(stderr, "Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        fprintf(stderr, "RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}


