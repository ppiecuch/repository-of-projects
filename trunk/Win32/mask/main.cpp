
#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>
#include <Windows.h>
#include <atlstr.h>

class Material
{
public:

	struct State{
		bool counterClockWise:1;
		bool light:1;
		bool fog:1;
		bool cullingBack:1;
		bool cullingFront:1;
		bool depthTest:1;
		bool depthWritable:1;
		bool alphaTest:1;

		bool operator !=(const State& other) const{
			return xor(other)!=0;
		}
		bool operator ==(const State& other) const{
			return !(*this!=other);
		}

		int xor(const State& other) const{
			int *a,*b;
			a=reinterpret_cast<int*>((void*)(this));
			b= reinterpret_cast<int*>((void*)&other);
			return *a^*b;
		}
	};

	State states;
};

int main(int argc, char* argv[])
{
	Material m1;
	Material m2;

	m1.states.counterClockWise=false;
	m1.states.light=false;
	m1.states.fog=false;
	m1.states.cullingBack=false;
	m1.states.cullingFront=false;
	m1.states.depthTest=false;
	m1.states.depthWritable=false;
	m1.states.alphaTest=false;

	m2.states.counterClockWise=false;
	m2.states.light=false;
	m2.states.fog=false;
	m2.states.cullingBack=false;
	m2.states.cullingFront=false;
	m2.states.depthTest=false;
	m2.states.depthWritable=false;
	m2.states.alphaTest=false;

	printf("%d\n",m1.states.xor(m2.states));

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