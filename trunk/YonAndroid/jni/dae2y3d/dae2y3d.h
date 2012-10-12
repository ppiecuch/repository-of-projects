#ifndef _DAE2Y3D_H_
#define _DAE2Y3D_H_

#include "vector3d.h"
#include "matrix4.h"
#include "quaternion.h"
#include "fast_atof.h"

#include "CXMLReaderImpl.h"
#include "CReadFileStream.h"
#include "CWriteFileStream.h"

using namespace yon;
using namespace yon::core;
using namespace yon::io;

void dae2y3d(XMLReader* reader,IWriteStream* stream);

/************************************************************************/
/* 共用部分                                                             */
/************************************************************************/
struct DAEUnit{
	f32 meter;
	core::stringc name;
};
struct DAEAsset{
	DAEUnit unit;
};
struct DAEInput{
	core::stringc Semantic;
	core::stringc Source;
	u32 Offset;
	u32 Set;
};
struct DAEAccessor{
	u32 Count;
	u32 Stride;
};
struct DAETechniqueCommon{
	DAEAccessor Accessor;
};
struct DAESource{
	core::stringc Id;
	u32 Count;
	//TODO移到TechniqueCommon中
	core::stringc ParamName;
	core::array<f32> FloatArray;
	core::array<core::stringc> NameArray;
	DAETechniqueCommon TechniqueCommon;

	DAESource():Count(0){}
};
/************************************************************************/
/* 图片部分                                                             */
/************************************************************************/
struct DAEImage{
	core::stringc Id;
	core::stringc Name;
	core::stringc ImageName;
};
struct DAELibraryImages{
	core::array<DAEImage> Images;

	DAEImage* getImage(const char* id){
		for(u32 i=0;i<Images.size();i++)
			if(Images[i].Id==id)
				return &Images[i];
		return NULL;
	}
};
/************************************************************************/
/* 材质部分                                                             */
/************************************************************************/
struct DAESurface{
	core::stringc InitFrom;

	//TODO
};
struct DAENewParam{
	core::stringc Sid;
	DAESurface Surface;

	//TODO
};
struct DAEEffect{
	core::stringc Id;
	core::stringc Name;
	core::array<DAENewParam> NewParams;

	DAENewParam* getSurfaceNewParam(){
		for(u32 i=0;i<NewParams.size();i++)
			if(strstr(NewParams[i].Sid.c_str(),"surface")!=NULL)
				return &NewParams[i];
		return NULL;
	}
};
struct DAELibraryEffects{
	core::array<DAEEffect> Effects;

	DAEEffect* getEffectByName(const char* name){
		for(u32 i=0;i<Effects.size();i++)
			if(Effects[i].Name==name)
				return &Effects[i];
		return NULL;
	}
};
/************************************************************************/
/* 模型部分                                                             */
/************************************************************************/
struct EXTRAVertex{
	u32 Index;
	u32 NormalIndex;
	u32 TexCoordIndex;
};
struct DAETriangles{
	core::stringc Material;
	u32 Count;
	u32 VertexOffset;
	core::array<u32> P;
	core::array<DAEInput> Inputs;

	//加工数据
	//core::array<u32> Indices;
	core::array<EXTRAVertex> Vertexs;

	DAEInput* getVertexInput(){
		for(u32 i=0;i<Inputs.size();i++)
			if(Inputs[i].Semantic=="VERTEX")
				return &Inputs[i];
		return NULL;
	}

	DAEInput* getNormalInput(){
		for(u32 i=0;i<Inputs.size();i++)
			if(Inputs[i].Semantic=="NORMAL")
				return &Inputs[i];
		return NULL;
	}

	DAEInput* getTexCoordInput(){
		for(u32 i=0;i<Inputs.size();i++)
			if(Inputs[i].Semantic=="TEXCOORD")
				return &Inputs[i];
		return NULL;
	}

	//TODO
};
struct DAEMesh{
	DAESource Position;
	DAESource Normal;
	DAESource MapChannel;
	core::array<DAETriangles> Triangles;

	void getRelatedNormalIndexsByVertexIndex(u32 index,core::array<u32>& arr){
		for(u32 i=0;i<Triangles.size();i++)
			for(u32 j=0;j<Triangles[i].Vertexs.size();j++)
				if(Triangles[i].Vertexs[j].Index==index)
				{
					bool has=false;
					for(u32 k=0;k<arr.size();k++)
					{
						if(arr[k]==Triangles[i].Vertexs[j].NormalIndex)
						{
							has=true;
							break;
						}
					}
					if(has==false)
						arr.push_back(Triangles[i].Vertexs[j].NormalIndex);
				}
	}

	//加工数据
	//core::array<EXTRATexcoord> Texcoords;
	//TODO
};
struct DAEGeometry{
	core::stringc Id;
	core::stringc Name;
	DAEMesh Mesh;
};
struct DAELibraryGeometries{
	core::array<DAEGeometry> Geometrys;
};
/************************************************************************/
/* 控制部分                                                             */
/************************************************************************/
struct EXTRAWeightComponent{
	u32 BoneIndex;
	u32 WeightIndex;
};
struct EXTRAVertexWeights{
	u32 VertexIndex;
	u32 Count;
	core::array<EXTRAWeightComponent> WeightComponents;;
};
struct DAEVertexWeights{
	u32 Count;
	core::array<DAEInput> Inputs;
	core::array<u32> Vcount;
	core::array<u32> V;

	//加工数据
	core::array<EXTRAVertexWeights> VertexWeights;

	DAEInput* getJointInput(){
		for(u32 i=0;i<Inputs.size();i++)
			if(Inputs[i].Semantic=="JOINT")
				return &Inputs[i];
		return NULL;
	}
	DAEInput* getWeightInput(){
		for(u32 i=0;i<Inputs.size();i++)
			if(Inputs[i].Semantic=="WEIGHT")
				return &Inputs[i];
		return NULL;
	}
};
struct DAESkin{
	core::stringc Source;
	core::array<DAESource> Sources;
	core::matrix4f BindShapeMatrix;
	DAEVertexWeights VertexWeights;
	//TODO

	//加工数据
	core::array<core::matrix4> InverseBindMatrixs;

	DAESource* getJointsSource(){
		for(u32 i=0;i<Sources.size();i++)
			if(Sources[i].ParamName=="JOINT")
				return &Sources[i];
		return NULL;
	}
	DAESource* getWeightsSource(){
		for(u32 i=0;i<Sources.size();i++)
			if(Sources[i].ParamName=="WEIGHT")
				return &Sources[i];
		return NULL;
	}
	DAESource* getBindPostsSource(){
		for(u32 i=0;i<Sources.size();i++)
			if(Sources[i].ParamName=="TRANSFORM")
				return &Sources[i];
		return NULL;
	}
};
struct DAEController{
	core::stringc Id;
	DAESkin Skin;
};
struct DAELibraryControllers{
	core::array<DAEController> Controllers;
};
/************************************************************************/
/* 动画部分                                                             */
/************************************************************************/
struct DAEAnimation
{
	core::stringc Id;
	DAESource Input;
	DAESource Output;
	core::stringc Target;
};
struct DAELibraryAnimations{
	core::array<DAEAnimation> Animations;
};
/************************************************************************/
/* 场景部分                                                             */
/************************************************************************/
struct DAEInstanceController{
	core::stringc Url;
	core::stringc Skeleton;
};
struct DAENodeNode{
	core::stringc Id;
	core::stringc Name;
	DAEInstanceController InstanceController;
	core::matrix4f Matrix;
	//core::stringc MaterialTarget;
	core::array<core::stringc> MaterialTargets;


	bool operator<(const DAENodeNode& other)
	{
		return Name<other.Name;
	}
};
struct DAEJointNode{
	core::stringc Id;
	core::stringc Name;
	core::stringc Sid;
	core::matrix4 Transform;
	core::array<DAEJointNode> Children;
	DAEJointNode* Parent;

	DAEJointNode():Parent(NULL){}

	//加工数据
	core::matrix4 AbsoluteTransform;
	static u32 Counter;
	u32 AbsoluteIndex;
};
struct DAEVisualScene{
	core::stringc Id;
	core::stringc Name;
	f32 FrameRate;
	core::array<DAENodeNode> Nodes;
	DAEJointNode* RootJoint;



	DAEVisualScene():RootJoint(NULL){}
	~DAEVisualScene(){
		if(RootJoint)
			delete RootJoint;
	}
};
struct DAELibraryVisualScenes{
	DAEVisualScene VisualScene;
};

const core::stringc ELEM_COLLADA = "COLLADA";

const core::stringc ELEM_ASSET = "asset";
const core::stringc ELEM_UNIT = "unit";
const core::stringc ELEM_UP_AXIS = "up_axis";

const core::stringc ELEM_LIBRARY_IMAGES = "library_images";
const core::stringc ELEM_IMAGE = "image";
const core::stringc ELEM_INIT_FROM = "init_from";

const core::stringc ELEM_LIBRARY_EFFECTS = "library_effects";
const core::stringc ELEM_EFFECT = "effect";
const core::stringc ELEM_PROFILE_COMMON = "profile_COMMON";
const core::stringc ELEM_NEWPARAM = "newparam";
const core::stringc ELEM_SURFACE = "surface";

const core::stringc ELEM_LIBRARY_MATERIALS = "library_materials";

const core::stringc ELEM_LIBRARY_GEOMETRIES = "library_geometries";
const core::stringc ELEM_GEOMETRY = "geometry";
const core::stringc ELEM_MESH = "mesh";
const core::stringc ELEM_SOURCE = "source";
const core::stringc ELEM_ACCESSOR = "accessor";
const core::stringc ELEM_VERTICES = "vertices";
const core::stringc ELEM_INPUT =  "input";
const core::stringc ELEM_ARRAY = "array";
const core::stringc ELEM_FLOAT_ARRAY = "float_array";
const core::stringc ELEM_INT_ARRAY = "int_array";
const core::stringc ELEM_TECHNIQUE_COMMON = "technique_common";
const core::stringc ELEM_POLYLIST =  "polylist";
const core::stringc ELEM_TRIANGLES = "triangles";
const core::stringc ELEM_POLYGONS =  "polygons";
const core::stringc ELEM_P = "p";
const core::stringc ELEM_VCOUNT  = "vcount";
const core::stringc ELEM_PARAM = "param";
const core::stringc ELEM_MATRIX = "matrix";

const core::stringc ELEM_LIBRARY_CONTROLLERS = "library_controllers";
const core::stringc ELEM_CONTROLLER = "controller";
const core::stringc ELEM_SKIN = "skin";
const core::stringc ELEM_BIND_SHAPE_MATRIX = "bind_shape_matrix";
const core::stringc ELEM_NAME_ARRAY = "Name_array";
const core::stringc ELEM_JOINTS = "joints";
const core::stringc ELEM_VERTEX_WEIGHTS = "vertex_weights";
const core::stringc ELEM_V = "v";

const core::stringc ELEM_LIBRARY_VISUAL_SCENES = "library_visual_scenes";
const core::stringc ELEM_VISUAL_SCENE = "visual_scene";
const core::stringc ELEM_NODE = "node";
const core::stringc ELEM_INSTANCE_CONTROLLER = "instance_controller";
const core::stringc ELEM_SKELETON = "skeleton";
const core::stringc ELEM_TRANSLATE = "translate";
const core::stringc ELEM_ROTATE = "rotate";
const core::stringc ELEM_SCALE = "scale";
const core::stringc ELEM_INSTANCE_GEOMETRY = "instance_geometry";
const core::stringc ELEM_EXTRA = "extra";
const core::stringc ELEM_FRAME_RATE = "frame_rate";
const core::stringc ELEM_TECHNIQUE = "technique";
const core::stringc ELEM_BIND_MATERIAL = "bind_material";
const core::stringc ELEM_BIND_VERTEX_INPUT = "bind_vertex_input";
const core::stringc ELEM_INSTANCE_MATERIAL = "instance_material";

const core::stringc ELEM_LIBRARY_ANIMATIONS = "library_animations";
const core::stringc ELEM_ANIMATION = "animation";
const core::stringc ELEM_CHANNEL = "channel";

const core::stringc ELEM_SCENE = "scene";

const core::stringc inputSemanticNames[] = {"POSITION", "VERTEX", "NORMAL", "TEXCOORD", "UV", "TANGENT", "IMAGE", "TEXTURE","JOINT","INV_BIND_MATRIX","WEIGHT", 0};
const core::stringc NODE_TYPE_JOINT= "JOINT";
const core::stringc PARAM_NAMES[] = {"TIME", "TRANSFORM", "INTERPOLATION", "JOINT","WEIGHT", "null",0};
const core::stringc PARAM_TYPES[] = {"float", "float2", "float3", "float4", "float4x4", "name", "null",0};
#endif