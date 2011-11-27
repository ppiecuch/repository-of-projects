/** Example 023 SMeshBufferHandling

A tutorial by geoff.

In this tutorial we'll learn how to create custom meshes and deal with them
with Irrlicht. We'll create an interesting heightmap with some lighting effects.
With keys 1,2,3 you can choose a different mesh layout, which is put into the
mesh buffers as desired. All positions, normals, etc. are updated accordingly.

Ok, let's start with the headers (I think there's nothing to say about it)
*/

#include <irrlicht.h>
#include "driverChoice.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//Namespaces for the engine
using namespace irr;
using namespace video;
using namespace core;
using namespace scene;
using namespace io;
using namespace gui;

class MyShaderCallBack : public video::IShaderConstantSetCallBack
{
public:

	virtual void OnSetConstants(video::IMaterialRendererServices* services,
		s32 userData)
	{
		video::IVideoDriver* driver = services->getVideoDriver();

		irr::s32 t[] = {0,1,2,3};

		services->setVertexShaderConstant("Texture0", (irr::f32*)&t[0], 1);
		services->setVertexShaderConstant("Texture1", (irr::f32*)&t[1], 1);
		services->setVertexShaderConstant("Texture2", (irr::f32*)&t[2], 1);
		services->setVertexShaderConstant("Texture3", (irr::f32*)&t[3], 1);

		irr::core::matrix4 mat = driver->getTransform(irr::video::ETS_WORLD);

		services->setVertexShaderConstant("WORLD", (irr::f32*)mat.pointer(), 16);
	}
};

irr::core::stringc vertexshader =   "varying vec4 pos;"
									"uniform mat4 WORLD;"
									"void main()"
									"{"
									"   gl_TexCoord[0] = gl_MultiTexCoord0;"
									"   pos = WORLD * gl_Vertex;"
									"   gl_Position = ftransform();"
									"}"
									"";

irr::core::stringc pixelshader =   "varying vec4 pos;"
									"uniform sampler2D Texture0;"
									"uniform sampler2D Texture1;"
									"uniform sampler2D Texture2;"
									"uniform sampler2D Texture3;"
									"void main()"
									"{"
									"   vec4 col0 = texture2D(Texture0, vec2(gl_TexCoord[0]));"
									"   vec4 col1 = texture2D(Texture1, vec2(gl_TexCoord[0]));"
									"   vec4 col2 = texture2D(Texture2, vec2(gl_TexCoord[0]));"
									"   vec4 col3 = texture2D(Texture3, vec2(gl_TexCoord[0]));"
									"   if (pos.y >= 0 && pos.y < 30)"
									"       gl_FragColor = col0;"
									"   else if (pos.y >= 30 && pos.y < 90)"
									"       gl_FragColor = col1;"
									"   else if (pos.y >= 90 && pos.y < 160)"
									"       gl_FragColor = col2;"
									"   else if (pos.y >= 160)"
									"       gl_FragColor = col3;"
									"}"
									"";
int main(void)
{
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL);

	irr::scene::ISceneManager* smgr = device->getSceneManager();

	irr::video::IVideoDriver* driver = device->getVideoDriver();

	irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();

	irr::scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode("../../media/terrain-heightmap.bmp");

	irr::s32 Shader = driver->getGPUProgrammingServices()->addHighLevelShaderMaterial(
		vertexshader.c_str(), "main", irr::video::EVST_VS_1_1,
		pixelshader.c_str(), "main", irr::video::EPST_PS_1_1,
		new MyShaderCallBack,
		irr::video::EMT_SOLID,
		0);

	terrain->setMaterialType((irr::video::E_MATERIAL_TYPE)Shader);

	terrain->setMaterialTexture(0, driver->getTexture("../../media/water.png"));
	terrain->setMaterialTexture(1, driver->getTexture("../../media/grass.png"));
	terrain->setMaterialTexture(2, driver->getTexture("../../media/dirt.png"));
	terrain->setMaterialTexture(3, driver->getTexture("../../media/rock.png"));

	terrain->setScale(irr::core::vector3df(5,1,5));

	printf("Low: %f\n", terrain->getTransformedBoundingBox().MinEdge.Y);
	printf("High: %f\n", terrain->getTransformedBoundingBox().MaxEdge.Y);

	while (device->run())
	{
		driver->beginScene();
		smgr->drawAll();
		driver->endScene();
	}
	device->drop();
	return 0;
}

