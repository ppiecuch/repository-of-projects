#include "CWaterModel.h"
#include "SUnit.h"
#include "ISceneManager.h"
#include <math.h>
#include <stdlib.h>
#include "ILogger.h"

namespace yon{
namespace scene{
namespace water{

#if 0
	inline float noise(int x,int y)
	{
		int n=x+y*57;
		n=(n<<13)^n;
		return ( 1.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0); 
	}

	inline float smoothedNoise(float x,float y)
	{
		float corners = ( noise(x-1, y-1)+noise(x+1, y-1)+noise(x-1, y+1)+noise(x+1, y+1) ) / 16;
		float sides = ( noise(x-1, y) +noise(x+1, y) +noise(x, y-1) +noise(x, y+1) ) / 8;
		float center = noise(x, y) / 4;
		return corners + sides + center;
	}

	inline float interpolate(float a,float b,float x)
	{
		float f=(1-cos(x*3.141592658))*0.5;
		return a*(1-f) + b*f;
		/*return a*(1-x) + b*x;*/
	}

	inline float interpolatedNoise(float x,float y)
	{
		int intX;
		if(x<0) intX=(int)x-1;
		else intX=(int)x;
		float fractionalX=x-intX;
		int intY;
		if(y<0) intY=(int)y-1;
		else intY=(int)y;
		float fractionalY=y-intY;

		float v1=smoothedNoise(intX,intY);
		float v2=smoothedNoise(intX+1,intY);
		float v3=smoothedNoise(intX,intY+1);
		float v4=smoothedNoise(intX+1,intY+1);

		float i1=interpolate(v1,v2,fractionalX);
		float i2=interpolate(v3,v4,fractionalX);

		return interpolate(i1 , i2 , fractionalY);
	}

	float PerlinNoise_2D(float x,float y)
	{
		static float persistence=0.4;
		static int n=3;
		float frequency;
		float amplitude;
		float total=0;
		for (int i=0;i<5;++i)
		{
			frequency=pow(2.0f,(float)i);
			amplitude=pow(persistence,(float)i);
			total += interpolatedNoise(x * frequency, y * frequency) * amplitude;
		}
		return total;
	}
#elif 1
	int permutation[] = { 151, 160, 137, 91,
		90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103,
		30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120,
		234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57,
		177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68,
		175, 74, 165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83,
		111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245,
		40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76,
		132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86,
		164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47,
		16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2,
		44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39,
		253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218,
		246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162,
		241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31, 181,
		199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150,
		254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128,
		195, 78, 66, 215, 61, 156, 180 };

	int p[512];
	bool initial=false;
	float fade(float t);
	float lerp(float t, float a, float b);
	float grad(int hash, float x, float y, float z);
	float noise(float x, float y, float z) {
		if(initial==false)
		{
			initial=true;
			for (int i = 0; i < 256; ++i)
				p[256 + i] = p[i] = permutation[i];
		}
		int X = (int)floor(x) & 255, // FIND UNIT CUBE THAT
			Y = (int)floor(y) & 255, // CONTAINS POINT.
			Z = (int)floor(z) & 255;
		x -= floor(x);				// FIND RELATIVE X,Y,Z
		y -= floor(y);				// OF POINT IN CUBE.
		z -= floor(z);
		float u = fade(x),			// COMPUTE FADE CURVES
			v = fade(y),			// FOR EACH OF X,Y,Z.
			w = fade(z);
		int A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z,			// HASH COORDINATES OF
			B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;		// THE 8 CUBE CORNERS,

		return lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),	// AND ADD
			grad(p[BA], x - 1, y, z)),							// BLENDED
			lerp(u, grad(p[AB], x, y - 1, z),					// RESULTS
			grad(p[BB], x - 1, y - 1, z))),						// FROM 8
			lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),		// CORNERS
			grad(p[BA + 1], x - 1, y, z - 1)),					// OF CUBE
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1), grad(p[BB + 1], x - 1, y - 1, z - 1))));
	}

	float fade(float t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float lerp(float t, float a, float b) {
		return a + t * (b - a);
	}

	float grad(int hash, float x, float y, float z) {
		int h = hash & 15;						// CONVERT LO 4 BITS OF HASH CODE
		float u = h < 8 ? x : y,				// INTO 12 GRADIENT DIRECTIONS.
			v = h < 4 ? y : h == 12 || h == 14 ? x : z;
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}
#else
	#define		MOD	0xff

	static int		permut[256];
	static const char	gradient[32][4] =
	{
		{ 1,  1,  1,  0}, { 1,  1,  0,  1}, { 1,  0,  1,  1}, { 0,  1,  1,  1},
		{ 1,  1, -1,  0}, { 1,  1,  0, -1}, { 1,  0,  1, -1}, { 0,  1,  1, -1},
		{ 1, -1,  1,  0}, { 1, -1,  0,  1}, { 1,  0, -1,  1}, { 0,  1, -1,  1},
		{ 1, -1, -1,  0}, { 1, -1,  0, -1}, { 1,  0, -1, -1}, { 0,  1, -1, -1},
		{-1,  1,  1,  0}, {-1,  1,  0,  1}, {-1,  0,  1,  1}, { 0, -1,  1,  1},
		{-1,  1, -1,  0}, {-1,  1,  0, -1}, {-1,  0,  1, -1}, { 0, -1,  1, -1},
		{-1, -1,  1,  0}, {-1, -1,  0,  1}, {-1,  0, -1,  1}, { 0, -1, -1,  1},
		{-1, -1, -1,  0}, {-1, -1,  0, -1}, {-1,  0, -1, -1}, { 0, -1, -1, -1},
	};


	void InitNoise (void)
	{
		unsigned int i = 0;
		while (i < 256)
			permut[i++] = rand() & MOD;
	}

	/*
	** Function finding out the gradient corresponding to the coordinates
	*/
	static int	Indice (const int i,
		const int j,
		const int k,
		const int l)
	{
		return (permut[(l + permut[(k + permut[(j + permut[i & MOD])
			& MOD])
			& MOD])
			& MOD]
		& 0x1f);
	}

	/*
	** Functions computing the dot product of the vector and the gradient
	*/
	static float	Prod (const float a, const char b)
	{
		if (b > 0)
			return a;
		if (b < 0)
			return -a;
		return 0;
	}

	static float	Dot_prod (const float x1, const char x2,
		const float y1, const char y2,
		const float z1, const char z2,
		const float t1, const char t2)
	{
		return (Prod (x1, x2) + Prod (y1, y2) + Prod (z1, z2) + Prod (t1, t2));
	}

	/*
	** Functions computing interpolations
	*/
	static float	Spline5 (const float state)
	{
		/*
		** Enhanced spline :
		** (3x^2 + 2x^3) is not as good as (6x^5 - 15x^4 + 10x^3)
		*/
		const float sqr = state * state;
		return state * sqr * (6 * sqr - 15 * state + 10);
	}

	static float	Linear (const float start,
		const float end,
		const float state)
	{
		return start + (end - start) * state;
	}

	/*
	** Noise function, returning the Perlin Noise at a given point
	*/
	float		Noise (const float x,
		const float y,
		const float z,
		const float t)
	{
		/* The unit hypercube containing the point */
		const int x1 = (int) (x > 0 ? x : x - 1);
		const int y1 = (int) (y > 0 ? y : y - 1);
		const int z1 = (int) (z > 0 ? z : z - 1);
		const int t1 = (int) (t > 0 ? t : t - 1);
		const int x2 = x1 + 1;
		const int y2 = y1 + 1;
		const int z2 = z1 + 1;
		const int t2 = t1 + 1;

		/* The 16 corresponding gradients */
		const char * g0000 = gradient[Indice (x1, y1, z1, t1)];
		const char * g0001 = gradient[Indice (x1, y1, z1, t2)];
		const char * g0010 = gradient[Indice (x1, y1, z2, t1)];
		const char * g0011 = gradient[Indice (x1, y1, z2, t2)];
		const char * g0100 = gradient[Indice (x1, y2, z1, t1)];
		const char * g0101 = gradient[Indice (x1, y2, z1, t2)];
		const char * g0110 = gradient[Indice (x1, y2, z2, t1)];
		const char * g0111 = gradient[Indice (x1, y2, z2, t2)];
		const char * g1000 = gradient[Indice (x2, y1, z1, t1)];
		const char * g1001 = gradient[Indice (x2, y1, z1, t2)];
		const char * g1010 = gradient[Indice (x2, y1, z2, t1)];
		const char * g1011 = gradient[Indice (x2, y1, z2, t2)];
		const char * g1100 = gradient[Indice (x2, y2, z1, t1)];
		const char * g1101 = gradient[Indice (x2, y2, z1, t2)];
		const char * g1110 = gradient[Indice (x2, y2, z2, t1)];
		const char * g1111 = gradient[Indice (x2, y2, z2, t2)];

		/* The 16 vectors */
		const float dx1 = x - x1;
		const float dx2 = x - x2;
		const float dy1 = y - y1;
		const float dy2 = y - y2;
		const float dz1 = z - z1;
		const float dz2 = z - z2;
		const float dt1 = t - t1;
		const float dt2 = t - t2;

		/* The 16 dot products */
		const float b0000 = Dot_prod(dx1, g0000[0], dy1, g0000[1],
			dz1, g0000[2], dt1, g0000[3]);
		const float b0001 = Dot_prod(dx1, g0001[0], dy1, g0001[1],
			dz1, g0001[2], dt2, g0001[3]);
		const float b0010 = Dot_prod(dx1, g0010[0], dy1, g0010[1],
			dz2, g0010[2], dt1, g0010[3]);
		const float b0011 = Dot_prod(dx1, g0011[0], dy1, g0011[1],
			dz2, g0011[2], dt2, g0011[3]);
		const float b0100 = Dot_prod(dx1, g0100[0], dy2, g0100[1],
			dz1, g0100[2], dt1, g0100[3]);
		const float b0101 = Dot_prod(dx1, g0101[0], dy2, g0101[1],
			dz1, g0101[2], dt2, g0101[3]);
		const float b0110 = Dot_prod(dx1, g0110[0], dy2, g0110[1],
			dz2, g0110[2], dt1, g0110[3]);
		const float b0111 = Dot_prod(dx1, g0111[0], dy2, g0111[1],
			dz2, g0111[2], dt2, g0111[3]);
		const float b1000 = Dot_prod(dx2, g1000[0], dy1, g1000[1],
			dz1, g1000[2], dt1, g1000[3]);
		const float b1001 = Dot_prod(dx2, g1001[0], dy1, g1001[1],
			dz1, g1001[2], dt2, g1001[3]);
		const float b1010 = Dot_prod(dx2, g1010[0], dy1, g1010[1],
			dz2, g1010[2], dt1, g1010[3]);
		const float b1011 = Dot_prod(dx2, g1011[0], dy1, g1011[1],
			dz2, g1011[2], dt2, g1011[3]);
		const float b1100 = Dot_prod(dx2, g1100[0], dy2, g1100[1],
			dz1, g1100[2], dt1, g1100[3]);
		const float b1101 = Dot_prod(dx2, g1101[0], dy2, g1101[1],
			dz1, g1101[2], dt2, g1101[3]);
		const float b1110 = Dot_prod(dx2, g1110[0], dy2, g1110[1],
			dz2, g1110[2], dt1, g1110[3]);
		const float b1111 = Dot_prod(dx2, g1111[0], dy2, g1111[1],
			dz2, g1111[2], dt2, g1111[3]);

		/* Then the interpolations, down to the result */
		const float idx1 = Spline5 (dx1);
		const float idy1 = Spline5 (dy1);
		const float idz1 = Spline5 (dz1);
		const float idt1 = Spline5 (dt1);

		const float b111 = Linear (b1110, b1111, idt1);
		const float b110 = Linear (b1100, b1101, idt1);
		const float b101 = Linear (b1010, b1011, idt1);
		const float b100 = Linear (b1000, b1001, idt1);
		const float b011 = Linear (b0110, b0111, idt1);
		const float b010 = Linear (b0100, b0101, idt1);
		const float b001 = Linear (b0010, b0011, idt1);
		const float b000 = Linear (b0000, b0001, idt1);

		const float b11 = Linear (b110, b111, idz1);
		const float b10 = Linear (b100, b101, idz1);
		const float b01 = Linear (b010, b011, idz1);
		const float b00 = Linear (b000, b001, idz1);

		const float b1 = Linear (b10, b11, idy1);
		const float b0 = Linear (b00, b01, idy1);

		return Linear (b0, b1, idx1);
	}
#endif

	CWaterModel::CWaterModel(IModel* parent,s32 xsize,s32 zsize,const core::vector3df& pos,
			const core::vector3df& rot,const core::vector3df& scale)
			:IWaterModel(parent,pos,rot,scale),m_iWidth(zsize),m_iHeight(xsize)
	{
		m_pUnit=new Unit3D();
		//m_pUnit->setVertexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_DYNAMIC);
		//m_pUnit->setIndexHardwareBufferUsageType(video::ENUM_HARDWARDBUFFER_USAGE_TYPE_STATIC);
		//m_pUnit->getMaterial()->setPolygonMode(video::ENUM_POLYGON_MODE_LINE);
		m_pUnit->setShap(&m_shap);

#ifdef MOD
		InitNoise();
#endif
		

		const u32 numVertices=(xsize+1)*(zsize+1);
		m_shap.getVertexArray().set_used(numVertices);

		s32 index=0;
		f32 fx=0.f;
		f32 fv=0.f;
		video::SColor color(0.65f,1,1,1);
		for(s32 x = 0; x<=xsize; ++x)
		{
			f32 fz=0.f;
			f32 fu=0.f;
			for(s32 z = 0; z<=zsize; ++z)
			{
				SVertex& v=m_shap.getVertexArray()[index++];
				v.pos.x=fx;
				v.pos.y=0;
				v.pos.z=fz;

				v.texcoords.x=fu;
				v.texcoords.y=fv;

				v.color=color;

				++fz;

				fu+=1.0f;
			}
			++fx;
			fv+=1.0f;
		}

		core::array<u16>& indices=m_shap.getIndexArray();
		bool wireframe=m_pUnit->getMaterial()->getPolygonMode()==video::ENUM_POLYGON_MODE_LINE;

		for(s32 x=0;x<xsize;++x)
		{
			for(s32 z=0;z<zsize;++z)
			{
				s32 index00=getIndex(x,z,0,0);
				s32 index01=getIndex(x,z,0,1);
				s32 index10=getIndex(x,z,1,0);
				s32 index11=getIndex(x,z,1,1);
				//Logger->debug("%d,%d->%d,%d,%d,%d\r\n",x,z,index00,index01,index10,index11);

				if(wireframe)
				{
					indices.push_back(index00);
					indices.push_back(index01);
					indices.push_back(index01);
					indices.push_back(index11);
					indices.push_back(index00);
					indices.push_back(index11);
					indices.push_back(index00);
					indices.push_back(index10);
					indices.push_back(index10);
					indices.push_back(index11);
				}
				else
				{
					indices.push_back(index00);
					indices.push_back(index01);
					indices.push_back(index11);
					indices.push_back(index00);
					indices.push_back(index11);
					indices.push_back(index10);
				}
			}
		}
	}
	CWaterModel::~CWaterModel(){
		m_pUnit->drop();
	}
	void CWaterModel::render(video::IVideoDriver* driver){
		driver->setTransform(video::ENUM_TRANSFORM_WORLD,getAbsoluteTransformation());
		driver->setMaterial(getMaterial(0));

		s32 index=0;
		for(s32 x = 0; x<=m_iHeight; ++x)
		{
			for(s32 z = 0; z<=m_iWidth; ++z)
			{
				SVertex& v=m_shap.getVertexArray()[index++];
				
				f32 ctime=driver->getTimer()->getTime()/1000.f;
				//f32 ctime=driver->getTimer()->getTime()/1000.f;
				//ctime=ctime-lasttime;
#ifdef MOD
				v.pos.y=Noise(x,z,ctime,0);
#else
				//v.pos.y=PerlinNoise_2D(fx+ctime,fz+ctime)/10.f+sinf(ctime+fx+fz)/2.f;
				f32 f1=noise(sqrt((float)(x*x+z*z))/5.f+ctime,0,0);
				f32 f2=noise(sqrt((float)((m_iHeight-x)*(m_iHeight-x)+(m_iWidth-z)*(m_iWidth-z)))/7.f+ctime,0,0);
				f32 f3=noise(sqrt((float)((m_iHeight-x)*(m_iHeight-x)+z*z))/3.f+ctime,0,0);
				v.pos.y=(f1+f2+f3)*3;
				//v.pos.y=noise(sqrt((float)(x*x+z*z))/5.f+ctime,sqrt((float)((m_iHeight-x)*(m_iHeight-x)+(m_iWidth-z)*(m_iWidth-z)))/7.f+ctime,sqrt((float)((m_iHeight-x)*(m_iHeight-x)+z*z))/3.f+ctime)*3;
#endif
				//Logger->debug("%.2f,%.2f\r\n",ctime,v.pos.y);
			}
		}
		m_shap.setVerticesDirty();

		driver->drawUnit(m_pUnit);
	}

	void CWaterModel::onRegisterForRender(){
		if(m_bVisible)
		{
			m_pSceneManager->registerForRender(this);
			IWaterModel::onRegisterForRender();
		}
	}

	s32 CWaterModel::getIndex(s32 px,s32 pz,s32 vx,s32 vz){
		return (px+vx)*(m_iWidth+1)+pz+vz;
	}
#ifdef MOD
	#undef MOD
#endif

}
}
}