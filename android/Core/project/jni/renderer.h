#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "object3d.h"

class Renderer{
public:
	Renderer(Object3D& obj);
	void render();
};

#endif
