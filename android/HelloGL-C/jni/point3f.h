#include <stl_pair.h>
using namespace std;
class Point3f{
public:
	float x,y,z;
	Point3f();
	Point3f(float,float,float);
	friend ostream & operator <<(ostream&,const Point3f&);
};
