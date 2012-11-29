#pragma once

#include "FileReader.h"
#include <map>


struct XC3DJoint{
	core::stringc Name;
	XC3DJoint* Parent;
	core::matrix4 LocalMatrix;

	core::array<f32> Times;
	core::array<core::matrix4> Animations;

	ISkinnedMesh::SJoint *IrrJoint;

	//Temp
	u32 AbsoluteIndex;
};

struct XC3DVertex{
	core::vector3df Pos;
	u32 WeightCount;
	core::array<u32> BoneIndexs;
	core::array<f32> Strengths;
};

class XC3DMeshLoader:public IMeshLoader
{
protected:
	scene::ISceneManager* smgr;
	irr::core::map<core::stringc,XC3DJoint*> jointMap;
	int frameRate;

	void release();
public:
	static core::stringc directory;
	XC3DMeshLoader(scene::ISceneManager* smgr);
	~XC3DMeshLoader(void);

	virtual bool isALoadableFileExtension(const io::path& filename) const;

	virtual IAnimatedMesh* createMesh(io::IReadFile* file);

	void importModel(irr::scene::ISkinnedMesh* mesh,CFileReader& reader);
	void importSkeleton(irr::scene::ISkinnedMesh* mesh,CFileReader& reader);

	void print(const char* title,core::matrix4& m);
	void print(const char* title,const core::quaternion& q);
	void print(const char* title,const core::vector3df& v);
};
