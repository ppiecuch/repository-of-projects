#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "object3d.h"

class Renderer{
private:
	Object3D obj;
public:
	Renderer(Object3D& obj);
	void render();
};

#endif
