#include "StdAfx.h"
#include "XC3DMeshLoader.h"


//TODO导入部分与构建部分分开
core::stringc XC3DMeshLoader::directory="";
XC3DMeshLoader::XC3DMeshLoader(scene::ISceneManager* smgr)
{
	this->smgr=smgr;
}

XC3DMeshLoader::~XC3DMeshLoader(void)
{

}
void XC3DMeshLoader::release()
{
	irr::core::map<core::stringc,XC3DJoint*>::Iterator iterator=jointMap.getIterator();

	for (;!iterator.atEnd();iterator++)
	{
		XC3DJoint* joint=iterator.getNode()->getValue();
		delete joint;
	}

	jointMap.clear();
}

bool XC3DMeshLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "xc3d" );
}

void XC3DMeshLoader::print(const char* title,core::matrix4& m)
{
	TRACE("%s\r\n",title);
	for(int j=0;j<16;j++)
	{
		TRACE("%.2f",fabs(m[j])<0.0001?0:m[j]);
		if(j%4==3)
			TRACE("\r\n");
		else
			TRACE(",");
	}
}
void XC3DMeshLoader::print(const char* title,const core::quaternion& q)
{
	TRACE("%s:{%g,%g,%g,%g}\r\n",title,q.X,q.Y,q.Z,q.W);
}
void XC3DMeshLoader::print(const char* title,const core::vector3df& v)
{
	TRACE("%s:{%g,%g,%g}\r\n",title,v.X,v.Y,v.Z);
}

void XC3DMeshLoader::importModel(irr::scene::ISkinnedMesh* mesh,CFileReader& reader)
{

	int meshCount=reader.readInt();
	TRACE("mesh count：%d\r\n",meshCount);

	//顶点骨骼交叉克隆法
	core::stringc folder=directory;
	int bufferIndex=0;
	for(int i=0;i<meshCount;i++)
	{
		char* name=reader.readString(';');
		TRACE("mesh.Name:%s\r\n",name);
		free(name);

		int materialCount=reader.readInt();
		TRACE("materialCount:%d\r\n",materialCount);

		core::array<XC3DVertex> vertexs;
		core::array<core::vector3df> normals,texcoords;

		//TODO颜色
		video::SColor white(255,255,255,255);

		//顶点数据
		int verticesCount=reader.readInt();
		TRACE("verticesCount:%d\r\n",verticesCount);
		for(int j=0;j<verticesCount;j+=3)
		{
			vertexs.push_back(XC3DVertex());
			XC3DVertex& vertex=vertexs.getLast();

			vertex.Pos.X=reader.readFloat();
			vertex.Pos.Y=reader.readFloat();
			vertex.Pos.Z=reader.readFloat();

			vertex.WeightCount=reader.readInt();
			for(u32 k=0;k<vertex.WeightCount;k++)
			{
				vertex.BoneIndexs.push_back(reader.readInt());
				vertex.Strengths.push_back(reader.readFloat());
			}
			//TRACE("vertices[%d]:{%g,%g,%g}\r\n",j/3,vertex.Pos.X,vertex.Pos.Y,vertex.Pos.Z);
		}

		//法线数据
		int normalCount=reader.readInt();
		TRACE("normalCount:%d\r\n",normalCount);
		for(int j=0;j<normalCount;j+=3)
		{
			normals.push_back(core::vector3df());
			core::vector3df& n=normals.getLast();

			n.X=reader.readFloat();
			n.Y=reader.readFloat();
			n.Z=reader.readFloat();

			//TRACE("nromals[%d]:{%g,%g,%g}\r\n",j/3,n.X,n.Y,n.Z);
		}

		//纹理坐标数据
		int texcoordCount=reader.readInt();
		TRACE("texcoordCount:%d\r\n",texcoordCount);
		for(int j=0;j<texcoordCount;j+=3)
		{
			texcoords.push_back(core::vector3df());
			core::vector3df& t=texcoords.getLast();

			t.X=reader.readFloat();
			t.Y=1-reader.readFloat();
			t.Z=reader.readFloat();

			//TRACE("%g,%g,%g\r\n",t.X,t.Y,t.Z);
		}
		//三角形数据
		int trianglesCount=reader.readInt();
		for(int j=0;j<trianglesCount;j++)
		{
			//创建MeshBuffer
			scene::SSkinMeshBuffer *meshBuffer=mesh->addMeshBuffer();
			core::array<video::S3DVertex>& Vertices=meshBuffer->Vertices_Standard;
			core::array<u16>& Indices=meshBuffer->Indices;

			//贴图数据
			char* imageName=reader.readString(';');
			TRACE("mesh image name:%s,is NULL:%d\r\n",imageName,imageName==NULL);
			if(core::stringc(imageName).trim().size()>0)
			{
				core::stringc texturePath=folder+imageName;
				video::SMaterial& material=meshBuffer->getMaterial();
				material.setTexture(0,smgr->getVideoDriver()->getTexture(texturePath));
				material.MaterialType=EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				material.Lighting = false;
				material.NormalizeNormals = false;
			}
			free(imageName);

			//索引数据
			int indicesCount=reader.readInt();
			TRACE("indices.size:%d\r\n",indicesCount);
			for(int k=0;k<indicesCount;k++)
			{
				video::S3DVertex v;
				v.Color=white;

				u32 pIndex=reader.readInt();
				u32 nIndex=reader.readInt();
				u32 tIndex=reader.readInt();

				//TRACE("Indices[%d][%d]:[%d,%d,%d]====>",i,k,pIndex,nIndex,tIndex);

				v.Pos=vertexs[pIndex].Pos;
				v.Normal=normals[nIndex];
				v.TCoords.X=texcoords[tIndex].X;
				v.TCoords.Y=texcoords[tIndex].Y;



				s32 index=-1;
				for(u32 l=0;l<Vertices.size();l++)
				{
					if(v==Vertices[l])
					{
						index=l;
						//TRACE("Used:[%d][%d]\r\n",i,index);
						break;
					}
				}
				if (index == -1)
				{
					index = Vertices.size();
					f32 sum=0;
					for(u32 l=0;l<vertexs[pIndex].WeightCount;l++)
					{
						u32 boneIndex=vertexs[pIndex].BoneIndexs[l];
						f32 strength=vertexs[pIndex].Strengths[l];
						sum+=strength;
						if(strength<0.0001f)
							continue;
						irr::scene::ISkinnedMesh::SJoint* joint=mesh->getAllJoints()[boneIndex];
						irr::scene::ISkinnedMesh::SWeight *w=mesh->addWeight(joint);
						w->buffer_id = bufferIndex;
						w->strength=strength;
						w->vertex_id = index;
						//TRACE("vertices[%d]--add weight:(boneIndex:%d(%s)/%d){%d,%g,%d}\r\n",k,boneIndex,joint->Name.c_str(),mesh->getAllJoints().size(),w->buffer_id,w->strength,w->vertex_id);
					}
					Vertices.push_back(v);
					//TRACE("Append vertex:[%d][%d],sum strength:%g\r\n",i,index,sum);
				}
				Indices.push_back(index);
			}

			//bufferIndex+1
			bufferIndex++;
		}


	}
	TRACE("importModel finish\r\n");
}
void XC3DMeshLoader::importSkeleton(irr::scene::ISkinnedMesh* mesh,CFileReader& reader)
{
	XC3DJoint* joint=new XC3DJoint();

	char* nodeName=reader.readString(';');
	TRACE("Joint: %s\r\n",nodeName);
	joint->Name=core::stringc(nodeName);
	jointMap.set(joint->Name,joint);
	free(nodeName);

	char* parentName=reader.readString(';');
	//TRACE("parentName: %s\r\n",parentName);

	irr::core::map<core::stringc,XC3DJoint*>::Node* node=jointMap.find(core::stringc(parentName));
	if(node)
		joint->Parent=node->getValue();
	else
		joint->Parent=NULL;
	free(parentName);


	//绝对索引
	joint->AbsoluteIndex=reader.readInt();
	//TRACE("AbsoluteIndex:%d\r\n",joint->AbsoluteIndex);


	//变换矩阵
	for(u32 i=0;i<16;i++)
	{
		joint->LocalMatrix[i]=reader.readFloat();
	}

	//print("local",joint->LocalMatrix);

	//建立骨骼
	ISkinnedMesh::SJoint *irrJoint;
	irrJoint=mesh->addJoint(joint->Parent?joint->Parent->IrrJoint:NULL);
	irrJoint->Name=joint->Name;
	joint->IrrJoint=irrJoint;

	//初始化变换矩阵
	irrJoint->LocalMatrix=joint->LocalMatrix;
	//print("joint->LocalMatrix:",irrJoint->LocalMatrix);

	//动画数据
	bool b=reader.readBool();
	//TRACE("has animation:%d\r\n",b);
	if(b)
	{
		int frameCount=reader.readInt();
		float frame;
		for(int i=0;i<frameCount;++i)
		{
			frame=reader.readFloat()*frameRate;

			ISkinnedMesh::SRotationKey* rKey=mesh->addRotationKey(irrJoint);
			rKey->frame=frame;
			rKey->rotation.X=reader.readFloat();
			rKey->rotation.Y=reader.readFloat();
			rKey->rotation.Z=reader.readFloat();
			rKey->rotation.W=reader.readFloat();

			ISkinnedMesh::SPositionKey* pKey=mesh->addPositionKey(irrJoint);
			pKey->frame=frame;
			pKey->position.X=reader.readFloat();
			pKey->position.Y=reader.readFloat();
			pKey->position.Z=reader.readFloat();
		}
	}

	int childrenCount=reader.readInt();
	TRACE("children count:%d\r\n",childrenCount);

	for(int i=0;i<childrenCount;i++)
	{
		importSkeleton(mesh,reader);
	}
}

IAnimatedMesh* XC3DMeshLoader::createMesh(io::IReadFile* file)
{
	irr::scene::ISkinnedMesh* mesh=NULL;

	CFileReader reader(file->getFileName().c_str());

	if(reader.exist()==false)
	{
		TRACE("open file:%s fail!\r\n",file->getFileName().c_str());
	}
	else
	{
		TRACE("createMesh\r\n");
		mesh=smgr->createSkinnedMesh();

		TRACE("read has animation\r\n");
		bool hasAnimation=reader.readBool();
		TRACE("%d\r\n",hasAnimation);

		if(hasAnimation)
		{
			//播放速率
			this->frameRate=reader.readInt();

			importSkeleton(mesh,reader);
		}

		importModel(mesh,reader);


		reader.close();
		mesh->finalize();

		release();
	}


	return mesh;
}
