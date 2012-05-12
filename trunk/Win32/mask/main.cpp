
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <Windows.h>
#include <atlstr.h>

const static char* MATERIAL_STATE_NAMES[]=
{
	"AlphaTest",
	"Blend",
	"ColorMaterial",
	"CullFace",
	"DepthTest",
	"Dither",
	"Fog",
	"Lighting",
	"LineSmooth",
	"Normalize",
	"RescaleNormal",
	"ScissorTest",
	"StencilTest"
};

enum ENUM_MATERIAL_STATE
{
	ENUM_MATERIAL_STATE_ALPHA_TEST = 0,
	ENUM_MATERIAL_STATE_BLEND,
	ENUM_MATERIAL_STATE_COLOR_MATERIAL,
	ENUM_MATERIAL_STATE_CULL_FACE,
	ENUM_MATERIAL_STATE_DEPTH_TEST,
	ENUM_MATERIAL_STATE_DITHER,
	ENUM_MATERIAL_STATE_FOG,
	ENUM_MATERIAL_STATE_LIGHTING,
	ENUM_MATERIAL_STATE_LINESMOOTH,
	ENUM_MATERIAL_STATE_NORMALIZE,
	ENUM_MATERIAL_STATE_RESCALE_NORMALIZE,
	ENUM_MATERIAL_STATE_SCISSOR_TEST,
	ENUM_MATERIAL_STATE_STENCIL_TEST,
	ENUM_MATERIAL_STATE_COUNT
};

class Material
{
public:

	struct State{
		State()
			:AlphaTest(false),
			Blend(false),
			ColorMaterial(false),
			CullFace(false),
			DepthTest(false),
			Dither(true),
			Fog(false),
			Lighting(false),
			LineSmooth(false),
			Normalize(false),
			RescaleNormal(false),
			ScissorTest(false),
			StencilTest(false){}

		bool AlphaTest:1;
		bool Blend:1;
		bool ColorMaterial:1;
		bool CullFace:1;
		bool DepthTest:1;
		bool Dither:1;
		bool Fog:1;
		bool Lighting:1;
		bool LineSmooth:1;
		bool Normalize:1;
		bool RescaleNormal:1;
		bool ScissorTest:1;
		bool StencilTest:1;

		int operator ^(const State& other) const{
			return getInt()^other.getInt();
		}

		bool operator !=(const State& other) const{
			return ((*this)^other)!=0;
		}
		bool operator ==(const State& other) const{
			return !(*this!=other);
		}

		int getInt() const{
			int *a;
			a=reinterpret_cast<int*>((void*)(this));
			return *a;
		}
	};

	State states;
};

int main(int argc, char* argv[])
{
	Material m1;
	Material m2;


	m2.states.AlphaTest=true;
	m2.states.Blend=false;
	m2.states.ColorMaterial=true;
	m2.states.CullFace=true;
	m2.states.DepthTest=true;
	m2.states.Dither=false;
	m2.states.Fog=true;
	m2.states.Lighting=true;
	m2.states.LineSmooth=false;
	m2.states.Normalize=true;
	m2.states.RescaleNormal=true;
	m2.states.ScissorTest=false;
	m2.states.StencilTest=true;

	printf("%d\n",m1.states^(m2.states));

	int xor=m1.states^(m2.states);

	int powOf2=1;
	for(int i=0;i<ENUM_MATERIAL_STATE_COUNT;++i)
	{
		if(xor&powOf2)
			if(m2.states.getInt()&powOf2)
				printf("%s change to true\n",MATERIAL_STATE_NAMES[i]);
			else
				printf("%s change to false\n",MATERIAL_STATE_NAMES[i]);
		powOf2<<=1;
	}

	struct _timeb start;
	_ftime64_s( &start ); 
	
	for(long i=0;i<9000000;++i)
	{
		m1.states==m2.states;
		/*m1.states.counterClockWise==m2.states.counterClockWise&&
			m1.states.light==m1.states.light&&
		m1.states.fog==m1.states.fog&&
		m1.states.cullingBack==m1.states.cullingBack&&
		m1.states.cullingFront==m1.states.cullingFront&&
		m1.states.depthTest==m1.states.depthTest&&
		m1.states.depthWritable==m1.states.depthWritable&&
		m1.states.alphaTest==m1.states.alphaTest;*/
	}

	struct _timeb end;
	_ftime64_s( &end ); 

	/*CString strTime;
	CString strMillTime;
	strTime.Format(_T("%d"), end.time-start.time);
	strMillTime.Format(_T("%d"), end.millitm-start.millitm);
	strTime += strMillTime;

	printf("%s\n",(LPTSTR)(LPCTSTR)strTime);*/


	printf("%d.%d\n",start.time,start.millitm);
	printf("%d.%d",end.time,end.millitm);

	getchar();

	return 0;
}