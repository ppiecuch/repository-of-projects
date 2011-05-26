#ifndef _CUBOID_RENDERER_H_
#define _CUBOID_RENDERER_H_
#include "cuboid.h"
#include "renderer.h"

class CuboidRenderer:Renderer{
private:
	Cuboid cuboid;
public:
	CuboidRenderer(Cuboid& cuboid);
	void render();
};

#endif
