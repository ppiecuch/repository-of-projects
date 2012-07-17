#include <stdio.h>
#include <list>
using namespace std;

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

#include <math.h>

const double PI64= 3.1415926535897932384626433832795028841971693993751;
const double RADTODEG64 = 180.0 / PI64;

int main(int argc, char* argv[])
{
	EnableMemLeakCheck();


	/*double degree=atan2(-1.0,0.0)*RADTODEG64;
	if (degree < 0)
		degree += 360;
	if (degree >= 360)
		degree -= 360;
	printf("%.2f\n",degree);*/

	double pitch,raw;
	double x=1;
	double y=1;
	double z=1;
	raw=atan2(x,z)*RADTODEG64;
	printf("raw:%.2f\n",raw);
	double t=sqrt(x*x+z*z);
	pitch=-atan2(y,t)*RADTODEG64;
	printf("pitch:%.2f\n",pitch);
	getchar();
	return 0;
}