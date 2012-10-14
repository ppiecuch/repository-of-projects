#include "dae2y3d.h"
#include <string.h>

class DAE2Y3DConvert{
private:
	core::matrix4f invert;
protected:
	DAEAsset asset;
	DAELibraryImages libraryImages;
	DAELibraryEffects libraryEffects;
	DAELibraryAnimations libraryAnimations;
	DAELibraryGeometries libraryGeometries;
	DAELibraryControllers libraryControllers;
	DAELibraryVisualScenes libraryVisualScenes;


	DAEGeometry* getGeometryByNodeName(const char* name)
	{
		for(u32 i=0;i<libraryGeometries.Geometrys.size();i++)
			if(libraryGeometries.Geometrys[i].Name==name)
				return &libraryGeometries.Geometrys[i];
		return NULL;
	}

	DAEController* getControllerBySkinSource(const char* source)
	{
		for(u32 i=0;i<libraryControllers.Controllers.size();i++)
			if(libraryControllers.Controllers[i].Skin.Source==source)
				return &libraryControllers.Controllers[i];
		return NULL;
	}

	DAEController* getControllerByControllerId(const char* id)
	{
		for(u32 i=0;i<libraryControllers.Controllers.size();i++)
			if(libraryControllers.Controllers[i].Id==id)
				return &libraryControllers.Controllers[i];
		return NULL;
	}

	DAEAnimation* getAnimationByNodeId(const char* id)
	{
		for(u32 i=0;i<libraryAnimations.Animations.size();i++)
			if(libraryAnimations.Animations[i].Target==id)
				return &libraryAnimations.Animations[i];
		return NULL;
	}

	u32 getNodeIndexByControllerId(const char* id)
	{
		libraryVisualScenes.VisualScene.Nodes.sort();
		for(u32 i=0;i<libraryVisualScenes.VisualScene.Nodes.size();i++)
			if(libraryVisualScenes.VisualScene.Nodes[i].InstanceController.Url==id)
				return i;
		return -1;
	}

	DAEJointNode* getJointNodeBySid(const char* sid,DAEJointNode* node)
	{
		if(node==NULL)
			return NULL;
		if(core::stringc(sid)==node->Sid)
			return node;
		for(u32 i=0;i<node->Children.size();i++)
		{
			DAEJointNode* result=getJointNodeBySid(sid,&node->Children[i]);
			if(result)
				return result;
		}
		return NULL;
	}

	const char* getImageNameByMaterialName(const char* target)
	{
		DAEEffect* effect=libraryEffects.getEffectByName(target);
		if(effect==NULL)
			return NULL;
		DAENewParam* param=effect->getSurfaceNewParam();
		if(param==NULL)
			return NULL;
		DAEImage* image=libraryImages.getImage(param->Surface.InitFrom.c_str());
		if(image==NULL)
			return NULL;
		return image->ImageName.c_str();
	}


	void readColladaSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		//读取版本号
		const char* version=xml->getAttributeValue("version");
		Logger->debug("version：%s\r\n",version);

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_ASSET==nodeName)
				{
					readAssetSection(xml,asset);
					//skipSection(xml);
				}
				else if(ELEM_LIBRARY_IMAGES==nodeName)
				{
					readLibraryImagesSection(xml);
				}
				else if(ELEM_LIBRARY_EFFECTS==nodeName)
				{
					readLibraryEffectsSection(xml);
				}
				else if(ELEM_LIBRARY_MATERIALS==nodeName)
				{
					skipSection(xml);
				}
				else if (ELEM_LIBRARY_GEOMETRIES==nodeName)
				{
					readLibraryGeometriesSection(xml);
				}
				else if(ELEM_LIBRARY_CONTROLLERS==nodeName)
				{
					readLibraryControllersSection(xml);
				}
				else if(ELEM_LIBRARY_ANIMATIONS==nodeName)
				{
					readLibraryAnimationsSection(xml);
				}
				else if (ELEM_LIBRARY_VISUAL_SCENES==nodeName)
				{
					readLibraryVisualScenesSection(xml);
				}
				else if(ELEM_SCENE==nodeName)
				{
					skipSection(xml);
				}
				else
				{
					Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("Wrong tag usage found:%s",xml->getNodeName()).c_str());
					skipSection(xml);
				}
			}
			else if(xml->getNodeType()==io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();
			}
		}
	}

	void readAssetSection(XMLReader* xml,DAEAsset& asset)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_UNIT==nodeName)
				{
					asset.unit.meter=core::fastatof(xml->getAttributeValue("meter"));
					asset.unit.name=xml->getAttributeValue("name");
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_ASSET==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}

	void readLibraryImagesSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_IMAGE==nodeName)
				{
					libraryImages.Images.push_back(DAEImage());
					DAEImage& image=libraryImages.Images.getLast();
					readImageSection(xml,image);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_LIBRARY_IMAGES==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readImageSection(XMLReader* xml,DAEImage& image)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());

		image.Id=getId(xml);
		image.Name=getName(xml);

		Logger->debug("id:%s,name:%s\r\n",image.Id.c_str(),image.Name.c_str());


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_INIT_FROM==nodeName)
				{
					if(xml->read())
					{
						image.ImageName=getImageName(xml);
						Logger->debug("imagename:%s\r\n",image.ImageName.c_str());
					}
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_IMAGE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readLibraryEffectsSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_EFFECT==nodeName)
				{
					libraryEffects.Effects.push_back(DAEEffect());
					DAEEffect& effect=libraryEffects.Effects.getLast();
					readEffectSection(xml,effect);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_LIBRARY_EFFECTS==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readEffectSection(XMLReader* xml,DAEEffect& effect)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());

		effect.Id=getId(xml);
		effect.Name=getName(xml);

		Logger->debug("id:%s,name:%s\r\n",effect.Id.c_str(),effect.Name.c_str());


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_PROFILE_COMMON==nodeName){}
				else if (ELEM_NEWPARAM==nodeName)
				{
					effect.NewParams.push_back(DAENewParam());
					DAENewParam& newParam=effect.NewParams.getLast();
					readNewParamSection(xml,newParam);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_EFFECT==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readNewParamSection(XMLReader* xml,DAENewParam& newParam)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());

		newParam.Sid=getSid(xml);

		Logger->debug("sid:%s\r\n",newParam.Sid.c_str());


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_SURFACE==nodeName){}
				else if(ELEM_INIT_FROM==nodeName)
				{
					if(xml->read())
					{
						newParam.Surface.InitFrom=xml->getNodeData();
						Logger->debug("surface.initfrom:%s\r\n",newParam.Surface.InitFrom.c_str());
					}
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_NEWPARAM==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readLibraryVisualScenesSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_VISUAL_SCENE==nodeName)
				{
					readVisualSceneSection(xml,libraryVisualScenes.VisualScene);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_LIBRARY_VISUAL_SCENES==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readVisualSceneSection(XMLReader* xml,DAEVisualScene& visualScene)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		visualScene.Id=getId(xml);
		visualScene.Name=getName(xml);
		Logger->debug("\tid：%s,name：%s\r\n",visualScene.Id.c_str(),visualScene.Name.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_NODE==nodeName)
				{
					if(isJointNode(xml))
					{
						visualScene.RootJoint=new DAEJointNode();
						readJointNodeSection(xml,visualScene.RootJoint);
						//skipSection(xml);
					}
					else
					{
						visualScene.Nodes.push_back(DAENodeNode());
						DAENodeNode& node=visualScene.Nodes.getLast();
						readNodeNodeSection(xml,node);
					}
				}
				else if(ELEM_EXTRA==nodeName){}
				else if(ELEM_TECHNIQUE==nodeName){}
				else if(ELEM_FRAME_RATE==nodeName)
				{
					if(xml->read())
					{
						visualScene.FrameRate=core::fastatof(xml->getNodeData());
						Logger->debug("\tFrameRate：%.2f\r\n",visualScene.FrameRate);
					}
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_VISUAL_SCENE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readNodeNodeSection(io::XMLReader* xml,DAENodeNode& node,bool isRoot=true)
	{
		if (xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		if(isRoot)
		{
			node.Id = getId(xml);
			node.Name= getName(xml);
			Logger->debug("\tid：%s,name：%s\r\n",node.Id.c_str(),node.Name.c_str());
		}
		else
		{
			Logger->debug("\tid：%s,name：%s\r\n",getId(xml).c_str(),getName(xml).c_str());
		}

		while(xml->read())
		{
			if(xml->getNodeType()==io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_INSTANCE_CONTROLLER==nodeName)
				{
					node.InstanceController.Url=getUrl(xml);
					Logger->debug("\turl：%s\r\n",node.InstanceController.Url.c_str());
				}
				else if(ELEM_SKELETON==nodeName)
				{
					//TODO支持多个skeleton
					core::stringc skeleton=xml->getNodeData();
					uriToId(skeleton);
					node.InstanceController.Skeleton=skeleton;
				}
				else if(ELEM_MATRIX==nodeName)
				{
					core::matrix4f temp=readMatrixNode(xml);
					node.Matrix=temp*node.Matrix;
					core::matrix4f& m=node.Matrix;
					//Logger->debug("\tNode matrix：\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n",m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
					Logger->debug("\tNode matrix：\r\n");
					m.print();
				}
				else if(ELEM_NODE==nodeName)
				{
					if(isPivotNode(xml))
					{
						readNodeNodeSection(xml,node,false);
					}
					else
					{
						Logger->warn(YON_LOG_WARN_FORMAT,"Unknow node inside node,will skip it!");
						skipSection(xml);
					}
				}
				else if(ELEM_INSTANCE_GEOMETRY==nodeName){}
				else if(ELEM_BIND_MATERIAL==nodeName){}
				else if(ELEM_TECHNIQUE_COMMON==nodeName){}
				else if(ELEM_INSTANCE_MATERIAL==nodeName)
				{
					//node.MaterialTarget=getMaterialTarget(xml);
					//Trace("Node material target:%s\r\n",node.MaterialTarget.c_str());

					node.MaterialTargets.push_back(getMaterialTarget(xml));
					Logger->debug("Node material target:%s\r\n",node.MaterialTargets.getLast().c_str());
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType()==io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_NODE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}

	void readJointNodeSection(io::XMLReader* xml,DAEJointNode* node)
	{
		if (xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		node->Id = getId(xml);
		node->Name= getName(xml);
		node->Sid=xml->getAttributeValue("sid");

		Logger->debug("\tid：%s,name：%s,sid：%s\r\n",node->Id.c_str(),node->Name.c_str(),node->Sid.c_str());
		Logger->debug("\tid：%s,name：%s,sid：%s\r\n",node->Id.c_str(),node->Name.c_str(),node->Sid.c_str());

		while(xml->read())
		{
			if(xml->getNodeType()==io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_MATRIX==nodeName)
				{
					node->Transform=readMatrixNode(xml);
					printMatrix("Transform",node->Transform);
					//add
					if(node->Parent)
					{
						node->AbsoluteTransform=node->Transform*node->Parent->AbsoluteTransform;
					}
					else
					{
						node->AbsoluteTransform=node->Transform;
					}
					printMatrix("AbsoluteTransform",node->AbsoluteTransform);

					core::matrix4f& m=node->Transform;
					//Logger->debug("\tJoint Transform：\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n",m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
					Logger->debug("\tJoint Transform：\r\n");
					m.print();
				}
				else if(ELEM_NODE==nodeName)
				{
					if(isJointNode(xml))
					{
						node->Children.push_back(DAEJointNode());
						DAEJointNode& child=node->Children.getLast();
						child.Parent=node;
						readJointNodeSection(xml,&child);
						Logger->debug("Add Joint node:%s\r\n",child.Name.c_str());
					}
					else if(isPivotNode(xml))
					{
						//TODO记录进JointNode
						skipSection(xml);
					}
					//暂时连NODE类型也当作JOINT NODE
					else if(isNodeNode(xml))
					{
						node->Children.push_back(DAEJointNode());
						DAEJointNode& child=node->Children.getLast();
						child.Parent=node;
						readJointNodeSection(xml,&child);
						Logger->debug("Add Node as Joint node:%s\r\n",child.Name.c_str());
					}
					else
					{
						skipSection(xml);
					}
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType()==io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_NODE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}

	void readLibraryGeometriesSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		


		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_GEOMETRY==nodeName)
				{
					libraryGeometries.Geometrys.push_back(DAEGeometry());
					DAEGeometry& geometry=libraryGeometries.Geometrys.getLast();
					readGeometrySection(xml,geometry);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_LIBRARY_GEOMETRIES==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readGeometrySection(XMLReader* xml,DAEGeometry& geometry)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		geometry.Id=getId(xml);
		geometry.Name=getName(xml);
		Logger->debug("\tid：%s,name：%s\r\n",geometry.Id.c_str(),geometry.Name.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_MESH==nodeName){}
				else if (ELEM_SOURCE==nodeName)
				{
					if(isPositionSource(xml))
					{
						readGeometryPositionSourceSection(xml,geometry.Mesh.Position);
					}
					else if(isMapChannelSource(xml))
					{
						readGeometryMapChannelSourceSection(xml,geometry.Mesh.MapChannel);
					}
					else if(isNormalSource(xml))
					{
						readGeometryNormalSourceSection(xml,geometry.Mesh.Normal);
					}
					else
					{
						skipSection(xml);
					}
				}
				else if(ELEM_TRIANGLES==nodeName)
				{

					geometry.Mesh.Triangles.push_back(DAETriangles());
					DAETriangles& triangles=geometry.Mesh.Triangles.getLast();
					readTrianglesSection(xml,triangles);

					u32 step=triangles.Inputs.size();
					DAEInput* normalInput=triangles.getNormalInput();
					DAEInput* vertexInput=triangles.getVertexInput();
					DAEInput* texcoordInput=triangles.getTexCoordInput();
					if(normalInput==NULL)
						Logger->warn(YON_LOG_WARN_FORMAT,"normalInput is NULL\r\n");
					if(vertexInput==NULL)
						Logger->warn(YON_LOG_WARN_FORMAT,"vertexInput is NULL\r\n");
					if(texcoordInput==NULL)
						Logger->warn(YON_LOG_WARN_FORMAT,"texcoordInput is NULL\r\n");
					int vIndex=0;
					for(u32 i=0;i<triangles.P.size();i+=step)
					{
						triangles.Vertexs.push_back(EXTRAVertex());
						EXTRAVertex& vertex=triangles.Vertexs.getLast();
						//vertex.Index=triangles.P[i+vertexInput->Offset];
						//vertex.NormalIndex=triangles.P[i+normalInput->Offset];
						//vertex.TexCoordIndex=triangles.P[i+texcoordInput->Offset];

						//对换三角形的1/2索引
						
						if(vIndex%3==1)
						{
							vertex.Index=triangles.P[i+step+vertexInput->Offset];
							vertex.NormalIndex=triangles.P[i+step+normalInput->Offset];
							vertex.TexCoordIndex=triangles.P[i+step+texcoordInput->Offset];
						}
						else if(vIndex%3==2)
						{
							vertex.Index=triangles.P[i-step+vertexInput->Offset];
							vertex.NormalIndex=triangles.P[i-step+normalInput->Offset];
							vertex.TexCoordIndex=triangles.P[i-step+texcoordInput->Offset];
						}
						else
						{
							vertex.Index=triangles.P[i+vertexInput->Offset];
							vertex.NormalIndex=triangles.P[i+normalInput->Offset];
							vertex.TexCoordIndex=triangles.P[i+texcoordInput->Offset];
						}

						vIndex++;
						
					}
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_GEOMETRY==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readTrianglesSection(XMLReader* xml,DAETriangles& triangles)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		triangles.Count=getCount(xml);
		triangles.Material=xml->getAttributeValue("material");
		Logger->debug("\tmaterial：%s,count：%d\r\n",triangles.Material.c_str(),triangles.Count);

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_INPUT==nodeName)
				{
					triangles.Inputs.push_back(DAEInput());
					DAEInput& input=triangles.Inputs.getLast();
					readInputSection(xml,input);
				}
				else if(ELEM_P==nodeName)
				{
					readIntsInsideElement(xml,triangles.P);
					Logger->debug("<p>\r\n\t");
					core::stringc str;
					for(u32 i=0;i<triangles.P.size();i++)
						str.append(core::stringc("%d,",triangles.P[i]));
					str.append("\r\n</p>\r\n");
					Logger->debug("%s",str.c_str());
				}
				else
				{
					skipSection(xml);
				}
			}
			else if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_TRIANGLES==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readGeometryPositionSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.FloatArray.size());
				}
				else if(ELEM_TECHNIQUE_COMMON==nodeName){}
				else if(ELEM_ACCESSOR==nodeName)
				{
					source.TechniqueCommon.Accessor.Count=getCount(xml);
					source.TechniqueCommon.Accessor.Stride=getStride(xml);
					Logger->debug("\taccessor.count:%d\r\n",source.TechniqueCommon.Accessor.Count);
					Logger->debug("\taccessor.stride:%d\r\n",source.TechniqueCommon.Accessor.Stride);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readGeometryNormalSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.FloatArray.size());
				}
				else if(ELEM_TECHNIQUE_COMMON==nodeName){}
				else if(ELEM_ACCESSOR==nodeName)
				{
					source.TechniqueCommon.Accessor.Count=getCount(xml);
					source.TechniqueCommon.Accessor.Stride=getStride(xml);
					Logger->debug("\taccessor.count:%d\r\n",source.TechniqueCommon.Accessor.Count);
					Logger->debug("\taccessor.stride:%d\r\n",source.TechniqueCommon.Accessor.Stride);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readGeometryMapChannelSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.FloatArray.size());
				}
				else if(ELEM_TECHNIQUE_COMMON==nodeName){}
				else if(ELEM_ACCESSOR==nodeName)
				{
					source.TechniqueCommon.Accessor.Count=getCount(xml);
					source.TechniqueCommon.Accessor.Stride=getStride(xml);
					Logger->debug("\taccessor.count:%d\r\n",source.TechniqueCommon.Accessor.Count);
					Logger->debug("\taccessor.stride:%d\r\n",source.TechniqueCommon.Accessor.Stride);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readLibraryControllersSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				//包含一个或多个Controller
				if(ELEM_CONTROLLER==nodeName)
				{
					//新建一个Controller
					libraryControllers.Controllers.push_back(DAEController());
					DAEController& controller=libraryControllers.Controllers.getLast();

					readControllerSection(xml,controller);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_LIBRARY_CONTROLLERS==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readControllerSection(XMLReader* xml,DAEController& controller)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		controller.Id=getId(xml);

		Logger->debug("\tid：%s\r\n",controller.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				//包含<skin>或<morph>，这里暂时只支持<skin>
				if(ELEM_SKIN==nodeName)
				{
					readSkinSection(xml,controller.Skin);
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_CONTROLLER==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readSkinSection(XMLReader* xml,DAESkin& skin)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		skin.Source=getSource(xml);

		Logger->debug("\tsource：%s\r\n",skin.Source.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				//包含<bind_shape_matrix?><source{3,*}><joints{1}><vertex_weights{1}><extra*>，这里暂时只支持<skin>
				if(ELEM_BIND_SHAPE_MATRIX==nodeName)
				{
					skin.BindShapeMatrix=readMatrixNode(xml);

					core::matrix4f& m=skin.BindShapeMatrix;
					//Logger->debug("\tBindShapeMatrix：\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n",m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);
					Logger->debug("\tBindShapeMatrix：\r\n");
					m.print();
				}
				else if(ELEM_SOURCE==nodeName)
				{
					if(isJointsSource(xml))
					{
						skin.Sources.push_back(DAESource());
						DAESource& source=skin.Sources.getLast();
						readJointsSourceSection(xml,source);
						Logger->debug("\tJoints:");
						core::stringc str;
						for(u32 i=0;i<source.Count;i++)
							str.append(core::stringc("%s,",source.NameArray[i].c_str()));
						str.append("\r\n");
						Logger->debug("%s",str.c_str());
					}
					else if(isWeightsSource(xml))
					{
						skin.Sources.push_back(DAESource());
						DAESource& source=skin.Sources.getLast();
						readWeightsSourceSection(xml,source);
						Logger->debug("\tWeights:");
						core::stringc str;
						for(u32 i=0;i<source.Count;i++)
							str.append(core::stringc("%.2f,",source.FloatArray[i]));
						str.append("\r\n");
						Logger->debug("%s",str.c_str());
					}
					else if(isBindPosesSource(xml))
					{
						skin.Sources.push_back(DAESource());
						DAESource& source=skin.Sources.getLast();
						readBindPosesSourceSection(xml,source);
						Logger->debug("\tBind poses:");
						core::stringc str;
						for(u32 i=0;i<source.Count;i++)
							str.append(core::stringc("%.2f,",source.FloatArray[i]));
						str.append("\r\n");
						Logger->debug("%s",str.c_str());

						//加工数据
						u32 count=source.Count/16;
						u32 index=0;
						for(u32 i=0;i<count;i++)
						{
							skin.InverseBindMatrixs.push_back(core::matrix4f(true));
							core::matrix4f& m=skin.InverseBindMatrixs.getLast();
							//for(u32 j=0;j<16;j++)
							//	m[j]=source.FloatArray[index++];
							for(u32 j=0;j<4;++j)
								for(u32 k=0;k<4;++k)
									m.m[j][k]=source.FloatArray[index++];

							printMatrix("m",m);
						}
					}
					else
					{
						skipSection(xml);
					}
				}
				else if(ELEM_VERTEX_WEIGHTS==nodeName)
				{
					readVertexWeightsSourceSection(xml,skin.VertexWeights);

					//加工数据
					DAEVertexWeights& vw=skin.VertexWeights;
					u32 index=0;
					for(u32 i=0;i<vw.Vcount.size();i++)
					{
						vw.VertexWeights.push_back(EXTRAVertexWeights());
						EXTRAVertexWeights& evw=vw.VertexWeights.getLast();
						evw.Count=vw.Vcount[i];
						evw.VertexIndex=i;
						for(u32 j=0;j<evw.Count;j++)
						{
							evw.WeightComponents.push_back(EXTRAWeightComponent());
							EXTRAWeightComponent& ewc=evw.WeightComponents.getLast();
							ewc.BoneIndex=vw.V[index++];
							ewc.WeightIndex=vw.V[index++];
						}
					}

					//查看加工
					Logger->debug("\r\n\tEXTRA:\r\n");
					for(u32 i=0;i<vw.VertexWeights.size();i++)
					{
						core::stringc str;
						str.append(core::stringc("\t,%d,%d:",vw.VertexWeights[i].VertexIndex,vw.VertexWeights[i].Count));
						for(u32 j=0;j<vw.VertexWeights[i].WeightComponents.size();j++)
						{
							u32 wi=vw.VertexWeights[i].WeightComponents[j].WeightIndex;
							str.append(core::stringc("%d->%d(%.2f),",vw.VertexWeights[i].WeightComponents[j].BoneIndex,wi,skin.getWeightsSource()->FloatArray[wi]));
						}
						str.append("\r\n");
						Logger->debug("%s",str.c_str());
					}
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SKIN==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}

		}
	}
	void readVertexWeightsSourceSection(XMLReader* xml,DAEVertexWeights& vertexWeights)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		vertexWeights.Count=getCount(xml);
		Logger->debug("\tcount：%d\r\n",vertexWeights.Count);

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_INPUT==nodeName)
				{
					vertexWeights.Inputs.push_back(DAEInput());
					DAEInput& input=vertexWeights.Inputs.getLast();
					readInputSection(xml,input);
					Logger->debug("\tInput:%s,%s,%d,%d\r\n",input.Semantic.c_str(),input.Source.c_str(),input.Offset,input.Set);
				}
				else if(ELEM_VCOUNT==nodeName)
				{
					readIntsInsideElement(xml,vertexWeights.Vcount);
					core::stringc str;
					str.append("\tVcount:");
					for(u32 i=0;i<vertexWeights.Vcount.size();i++)
						str.append(core::stringc("%d,",vertexWeights.Vcount[i]));
					str.append("\r\n");
					Logger->debug("%s",str.c_str());
				}
				else if(ELEM_V==nodeName)
				{
					readIntsInsideElement(xml,vertexWeights.V);
					core::stringc str("\tV:");
					for(u32 i=0;i<vertexWeights.V.size();i++)
						str.append(core::stringc("%d,",vertexWeights.V[i]));
					str.append("\r\n");
					Logger->debug("%s",str.c_str());
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_VERTEX_WEIGHTS==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readJointsSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_NAME_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readStringsInsideElement(xml,source.NameArray,source.Count);
					Logger->debug("\tname array count:%d\r\n",source.Count);
				}
				else if(ELEM_PARAM==nodeName)
				{
					source.ParamName=getName(xml);
					Logger->debug("\tparam.name:%s\r\n",source.ParamName);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readWeightsSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.Count);
				}
				else if(ELEM_PARAM==nodeName)
				{
					source.ParamName=getName(xml);
					Logger->debug("\tparam.name:%s\r\n",source.ParamName);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readBindPosesSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.Count);
				}
				else if(ELEM_PARAM==nodeName)
				{
					source.ParamName=getName(xml);
					Logger->debug("\tparam.name:%s\r\n",source.ParamName);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readLibraryAnimationsSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if (ELEM_ANIMATION==nodeName)
				{
					libraryAnimations.Animations.push_back(DAEAnimation());
					DAEAnimation& animation=libraryAnimations.Animations.getLast();
					readAnimation(xml,animation);
				}
				else
				{
					skipSection(xml);
				}
			}
			else
				if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
				{
					const char* nodeName=xml->getNodeName();
					if(ELEM_LIBRARY_ANIMATIONS==nodeName)
					{
						Logger->debug("</%s>\r\n",nodeName);
						break;
					}
				}
		}
	}

	void readAnimation(XMLReader* xml,DAEAnimation& animation)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;


		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		animation.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",animation.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					if(isInputSource(xml))
					{
						readAnimationInputSourceSection(xml,animation.Input);
					}
					else if(isOutputSource(xml))
					{
						readAnimationOutputSourceSection(xml,animation.Output);
					}
					else
					{
						skipSection(xml);
					}
				}
				else if(ELEM_CHANNEL==nodeName)
				{
					animation.Target=getAnimationTarget(xml);
					Logger->debug("\ttarget:%s\r\n",animation.Target.c_str());
				}
				else
				{
					skipSection(xml);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();
				if(ELEM_ANIMATION==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readAnimationInputSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.Count);
				}
				else if(ELEM_PARAM==nodeName)
				{
					source.ParamName=getName(xml);
					Logger->debug("\tparam.name:%s\r\n",source.ParamName);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readAnimationOutputSourceSection(XMLReader* xml,DAESource& source)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		

		source.Id=getId(xml);
		Logger->debug("\tid：%s\r\n",source.Id.c_str());

		while(xml->read())
		{
			if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_FLOAT_ARRAY==nodeName)
				{
					source.Count=xml->getAttributeValueAsInt("count");
					readFloatsInsideElement(xml,source.FloatArray,source.Count);
					Logger->debug("\tfloat array count:%d\r\n",source.Count);
				}
				else if(ELEM_PARAM==nodeName)
				{
					source.ParamName=getName(xml);
					Logger->debug("\tparam.name:%s\r\n",source.ParamName);
				}
			}
			else if(xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
			{
				const char* nodeName=xml->getNodeName();

				if(ELEM_SOURCE==nodeName)
				{
					Logger->debug("</%s>\r\n",nodeName);
					break;
				}
			}
		}
	}
	void readInputSection(XMLReader* xml,DAEInput& input)
	{
		if(xml==NULL)
			return;

		Logger->debug("<%s>\r\n",xml->getNodeName());
		
		input.Semantic=xml->getAttributeValue("semantic");
		input.Source=getSource(xml);
		input.Offset=xml->getAttributeValueAsInt("offset");
		input.Set=xml->getAttributeValueAsInt("set");

		Logger->debug("\tsemantic:%s\r\n",input.Semantic);
		Logger->debug("\tsource:%s\r\n",input.Source);
		Logger->debug("\toffset:%d\r\n",input.Offset);
		Logger->debug("\tset:%d\r\n",input.Set);

		Logger->debug("</%s>\r\n",xml->getNodeName());
	}
	void skipSection(XMLReader* xml)
	{
		if(xml==NULL||xml->isEmptyElement())
			return;
		const char* nodeName=xml->getNodeName();
		Logger->debug("(<%s></%s>)\r\n",nodeName,nodeName);

		// read until we've reached the last element in this section
		int tagCounter = 1;

		while(tagCounter && xml->read())
		{
			if (xml->getNodeType() == ENUM_XML_NODE_ELEMENT &&!xml->isEmptyElement())
			{
				++tagCounter;
			}
			else
				if (xml->getNodeType() == ENUM_XML_NODE_ELEMENT_END)
					--tagCounter;
		}
	}
	core::stringc getId(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		if (id.length()==0)
			id = xml->getAttributeValue("name");
		return id;
	}
	core::stringc getSid(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("sid");
		return id;
	}
	core::stringc getName(XMLReader* xml)
	{
		core::stringc name = xml->getAttributeValue("name");
		return name;
	}
	core::stringc getImageName(XMLReader* xml)
	{
		core::stringc path=xml->getNodeData();
		u32 index=path.findLast('/');
		if(index==-1)
			index=path.findLast('\\');
		Logger->debug("index:%d,size:%d\r\n",index,path.length());
		//注意不能path=path.subString(index+1,path.size());否则，打包成DLL会出错（直接运行不会）
		//==>subString(i,length)中的length是指substring的length不是path的length
		core::stringc npath=path.subString(index+1,path.length()-index);
		Logger->debug("npath:%s\r\n",npath.c_str());
		return npath;
	}
	core::stringc getUrl(XMLReader* xml)
	{
		core::stringc url = xml->getAttributeValue("url");
		uriToId(url);
		return url;
	}
	core::stringc getSource(XMLReader* xml)
	{
		core::stringc source = xml->getAttributeValue("source");
		uriToId(source);
		return source;
	}
	core::stringc getAnimationTarget(XMLReader* xml)
	{
		core::stringc target = xml->getAttributeValue("target");
		s32 index=target.findFirst('/');
		target=target.subString(0,index);
		return target;
	}
	core::stringc getMaterialTarget(XMLReader* xml)
	{
		core::stringc target = xml->getAttributeValue("target");
		uriToId(target);
		return target;
	}
	int getCount(XMLReader* xml)
	{
		int count = xml->getAttributeValueAsInt("count");
		return count;
	}
	int getStride(XMLReader* xml)
	{
		int stride = xml->getAttributeValueAsInt("stride");
		return stride;
	}
	core::stringc getNodeType(XMLReader* xml)
	{
		core::stringc type = xml->getAttributeValue("type");
		return type;
	}
	void uriToId(core::stringc& str)
	{
		// currently, we only remove the # from the begin if there
		// because we simply don't support referencing other files.
		if (!str.length())
			return;

		if (str[0] == '#')
			str.erase(0);
	}
	//! reads floats from inside of xml element until end of xml element
	void readFloatsInsideElement(XMLReader* xml, core::array<f32>& floats, u32 count)
	{
		if (xml->isEmptyElement())
			return;

		while(xml->read())
		{
			// TODO: check for comments inside the element
			// and ignore them.

			if (xml->getNodeType() == io::ENUM_XML_NODE_TEXT)
			{
				// parse float data
				core::stringc data = xml->getNodeData();
				data.trim();
				const c8* p = &data[0];

				for (u32 i=0; i<count; ++i)
				{
					findNextNoneWhiteSpace(&p);
					if (*p)
						//floats[i] = readFloat(&p);
						floats.push_back(readFloat(&p));
					else
						//floats[i] = 0.0f;
						floats.push_back(0.0f);
				}
			}
			else
				if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
					break; // end parsing text
		}
	}
	void readFloatsInsideElement(XMLReader* xml, f32* floats, u32 count)
	{
		if (xml->isEmptyElement())
			return;

		while(xml->read())
		{
			// TODO: check for comments inside the element
			// and ignore them.

			if (xml->getNodeType() == io::ENUM_XML_NODE_TEXT)
			{
				// parse float data
				core::stringc data = xml->getNodeData();
				data.trim();
				const c8* p = &data[0];

				for (u32 i=0; i<count; ++i)
				{
					findNextNoneWhiteSpace(&p);
					if (*p)
						floats[i] = readFloat(&p);
					else
						floats[i] = 0.0f;

				}
			}
			else
				if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
					break; // end parsing text
		}
	}
	//! reads string from inside of xml element until end of xml element
	void readStringsInsideElement(XMLReader* xml, core::array<core::stringc>& array, u32 count)
	{
		if (xml->isEmptyElement())
			return;

		while(xml->read())
		{
			// TODO: check for comments inside the element
			// and ignore them.

			if (xml->getNodeType() == io::ENUM_XML_NODE_TEXT)
			{
				// parse float data
				core::stringc data = xml->getNodeData();
				data.trim();
				const c8* p = &data[0];

				for (u32 i=0; i<count; ++i)
				{
					findNextNoneWhiteSpace(&p);
					char buffer[128];
					sscanf_s(p, "%s", buffer,128);
					core::stringc str(buffer);
					str.trim();
					p+=str.length();
					array.push_back(str);
				}
			}
			else
				if (xml->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
					break; // end parsing text
		}
	}
	void readIntsInsideElement(io::XMLReader* reader, core::array<u32>& ints, u32 count)
	{
		if (reader->isEmptyElement())
			return;

		while(reader->read())
		{
			// TODO: check for comments inside the element
			// and ignore them.

			if (reader->getNodeType() == io::ENUM_XML_NODE_TEXT)
			{
				// parse float data
				core::stringc data = reader->getNodeData();
				data.trim();
				const c8* p = &data[0];

				for (u32 i=0; i<count; ++i)
				{
					findNextNoneWhiteSpace(&p);
					if (*p)
						//ints[i] = readInt(&p);
						ints.push_back(readInt(&p));
					else
						//ints[i] = 0;
						ints.push_back(0);

				}
			}
			else
				if (reader->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
					break; // end parsing text
		}
	}
	void readIntsInsideElement(io::XMLReader* reader, core::array<u32>& ints)
	{
		if (reader->isEmptyElement())
			return;

		while(reader->read())
		{
			// TODO: check for comments inside the element
			// and ignore them.

			if (reader->getNodeType() == io::ENUM_XML_NODE_TEXT)
			{
				// parse float data
				core::stringc data = reader->getNodeData();
				data.trim();
				char* p = &data[0];

				char *pOut=NULL;
				char *split=" ";
				char *r=NULL;
				r = strtok_s(p,split,&pOut);
				while(r!=NULL) {
					const c8* temp=r;
					ints.push_back(readInt(&temp));

					r = strtok_s(NULL,split,&pOut);
				}


				/*while(*p!='\0')
				{
				findNextNoneWhiteSpace(&p);
				if(*p=='\0')
				break;
				if (*p)
				ints.push_back(readInt(&p));
				else
				ints.push_back(0);

				}*/
			}
			else
				if (reader->getNodeType() == io::ENUM_XML_NODE_ELEMENT_END)
					break; // end parsing text
		}
	}
	void findNextNoneWhiteSpace(const c8** start)
	{
		const c8* p = *start;

		while(*p && (*p==' ' || *p=='\n' || *p=='\r' || *p=='\t'))
			++p;

		// TODO: skip comments <!-- -->

		*start = p;
	}
	f32 readFloat(const c8** p)
	{
		f32 ftmp;
		*p = core::fast_atof_move(*p, ftmp);
		return ftmp;
	}
	s32 readInt(const c8** p)
	{
		return (s32)readFloat(p);
	}
	core::matrix4f readMatrixNode(io::XMLReader* xml)
	{
		core::matrix4f mat;
		if (xml->isEmptyElement())
			return mat;

		readFloatsInsideElement(xml, mat.pointer(), 16);
		return mat;
	}
	//用于Animation
	bool isInputSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"input")!=NULL;
	}
	bool isOutputSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"output")!=NULL;
	}
	//用于Geometry
	bool isPositionSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"positions")!=NULL;
	}
	bool isNormalSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"normals")!=NULL;
	}
	bool isMapChannelSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"map-channel")!=NULL;
	}
	//用于Controller
	bool isJointsSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"joints")!=NULL;
	}
	bool isWeightsSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"weights")!=NULL;
	}
	bool isBindPosesSource(XMLReader* xml)
	{
		core::stringc id = xml->getAttributeValue("id");
		return strstr(id.c_str(),"bind_poses")!=NULL;
	}
	//用于VisualScene
	bool isJointNode(XMLReader* xml)
	{
		core::stringc type = xml->getAttributeValue("type");
		return type=="JOINT";
	}
	bool isNodeNode(XMLReader* xml)
	{
		core::stringc type = xml->getAttributeValue("type");
		core::stringc name = xml->getAttributeValue("name");
		return type=="NODE"&&strstr(name.c_str(),"_PIVOT")==NULL;
	}
	bool isPivotNode(XMLReader* xml)
	{
		core::stringc type = xml->getAttributeValue("type");
		core::stringc name = xml->getAttributeValue("name");
		return type=="NODE"&&strstr(name.c_str(),"_PIVOT")!=NULL;
	}


	void printMatrix(const char* title,core::matrix4f& m)
	{
		Logger->debug("%s\r\n",title);
		/*for(int j=0;j<16;++j)
		{
#define M(i,j) fabs(m.m[i][j])<0.0001?0:m.m[i][j]
			Logger->debug("%g,%g,%g,%g\r\n",M(0,j),M(1,j),M(2,j),M(3,j));
#undef M(i,j)
		}*/
		m.print();
	}

#if 0

	void exportJointNode(DAEJointNode* parent,DAEJointNode* node,CFileWriter& writer)
	{
		Trace("joint:%s,sid:%s\r\n",node->Name.c_str(),node->Sid.c_str());
		writer.writeString(node->Name.c_str(),';');
		if(parent)
			writer.writeString(parent->Name.c_str(),';');
		else
			writer.writeString("NULL",';');

		//绝对索引
		node->AbsoluteIndex=DAEJointNode::Counter;
		DAEJointNode::Counter++;
		Trace("Counter:%d\r\n",DAEJointNode::Counter);
		writer.writeInt((int)node->AbsoluteIndex);


	
		//将变换转调到转换器中
		core::matrix4 mm=node->Transform.getTransposed();
		if(parent==NULL)
		{
			mm=invert*mm;
			mm[10]=-mm[10];
		}
		else
		{
			mm[2]=-mm[2];
			mm[6]=-mm[6];
			mm[14]=-mm[14];
		}
		mm[8]=-mm[8];
		mm[9]=-mm[9];
		node->Transform=mm;
		for(u32 i=0;i<16;i++)
		{
			writer.writeFloat(node->Transform[i]);
		}
		core::matrix4& m=node->Transform;
		Trace("\ttransform：\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n\t%.2f,%.2f,%.2f,%.2f\r\n",m[0],m[1],m[2],m[3],m[4],m[5],m[6],m[7],m[8],m[9],m[10],m[11],m[12],m[13],m[14],m[15]);


		//动画数据
		DAEAnimation* animation=getAnimationByNodeId(node->Id.c_str());
		writer.writeBool(animation!=NULL);
		if(animation==NULL)
		{
			Trace("\rNo animation\r\n");
		}
		else
		{
			exportAnimation(parent==NULL,animation,writer);
		}

		writer.writeInt((int)node->Children.size());

		for(u32 i=0;i<node->Children.size();i++)
		{
			exportJointNode(node,&node->Children[i],writer);
		}
	}

	void exportAnimation(bool isRoot,DAEAnimation* animation,CFileWriter& writer)
	{
		Trace("\tanimation id:%s\r\n",animation->Id);
		//writer.writeString(animation->Id.c_str(),';');
		
		if(animation->Input.Count!=animation->Output.Count/16)
			Trace("[▲WARN]Input not equal to output in animation:%s\r\n",animation->Id.c_str());
		u32 frameCount=animation->Input.Count;
		writer.writeInt((int)frameCount);
		core::matrix4 temp;
		for(u32 i=0;i<frameCount;i++)
		{
			writer.writeFloat((float)animation->Input.FloatArray[i]);
			Trace("%.2f,",(float)animation->Input.FloatArray[i]);

			for(u32 j=0;j<16;j++)
			{
				temp[j%16]=animation->Output.FloatArray[i*16+j];
			}

			core::matrix4 m=temp.getTransposed();
			if(isRoot)
			{
				m=invert*m;
				m[10]=-m[10];
			}
			else
			{
				m[2]=-m[2];
				m[6]=-m[6];
				m[14]=-m[14];
			}
			m[8]=-m[8];
			m[9]=-m[9];
			
			core::quaternion q(m);
			writer.writeFloat(q.X);
			writer.writeFloat(q.Y);
			writer.writeFloat(q.Z);
			writer.writeFloat(q.W);

			core::vector3df p=m.getTranslation();
			writer.writeFloat(p.X);
			writer.writeFloat(p.Y);
			writer.writeFloat(p.Z);
		}
	}

	void exportModel(CFileWriter& writer)
	{
		int meshCount=0;
		for(u32 i=0;i<libraryVisualScenes.VisualScene.Nodes.size();i++)
		{
			DAENodeNode& node=libraryVisualScenes.VisualScene.Nodes[i];
			DAEGeometry* geometry=getGeometryByNodeName(node.Name.c_str());
			if(geometry==NULL)
			{
				continue;
			}
			meshCount++;
		}
		writer.writeInt(meshCount);

		Trace("\r\n--------Model Data------\r\n");
		Trace("mesh count：%d\r\n",meshCount);

		//按Node.Name进行由小到大排序
		libraryVisualScenes.VisualScene.Nodes.sort();

		for(u32 i=0;i<libraryVisualScenes.VisualScene.Nodes.size();i++)
		{
			DAENodeNode& node=libraryVisualScenes.VisualScene.Nodes[i];
			DAEGeometry* geometry=getGeometryByNodeName(node.Name.c_str());
			if(geometry==NULL)
			{
				Trace("[▲WARN]No geometry found with node name:%s\r\n",node.Name.c_str());
				continue;
			}
			else
			{
				Trace("[DEBUG]node name:%s,geometry->Id:%s\r\n",node.Name.c_str(),geometry->Id.c_str());
			}

			Trace("name:%s,nodeName:%s\r\n",geometry->Name.c_str(),node.Name);
			writer.writeString(node.Name.c_str(),';');
			/*
			const char* imageName=getImageNameFromMaterialTarget(node.MaterialTarget.c_str());
			if(imageName==NULL)
			{
				Trace("[▲WARN]No image found with node material target:%s\r\n",node.MaterialTarget.c_str());
				writer.writeString("",';');
			}
			else
				writer.writeString(imageName,';');
			Trace("image name:%s\r\n",imageName);
			*/
			writer.writeInt(node.MaterialTargets.size());

			DAEController* controller=getControllerBySkinSource(geometry->Id.c_str());
			Trace("controller:%08x\r\n",controller);

			if(controller!=NULL)
			{
				core::matrix4 bsm=controller->Skin.BindShapeMatrix.getTransposed();
				core::matrix4 rsm=bsm;
				rsm[12]=rsm[13]=rsm[14]=0;
				core::array<core::matrix4>& ibms=controller->Skin.InverseBindMatrixs;
				DAESource* joint=controller->Skin.getJointsSource();
				DAESource position=geometry->Mesh.Position;
				DAESource normal=geometry->Mesh.Normal;
				if(joint==NULL)
				{
					Trace("[▲WARN]No joint array found with controller whose node name:%s\r\n",node.Name.c_str());
					continue;
				}

				writer.writeInt((int)position.Count);

				core::array<core::vector3df> bsmPos;
				core::array<core::matrix4> ibmJm;

				core::array<core::vector3df> rsmNolStart;
				core::array<core::vector3df> rsmNolEnd;
				rsmNolStart.set_used(geometry->Mesh.Normal.Count/3);
				rsmNolEnd.set_used(geometry->Mesh.Normal.Count/3);
				core::array<u32> normalIndexs;

				//计算bsmPos
				u32 vertexCount=position.Count/3;
				u32 index=0;
				for(u32 j=0;j<vertexCount;j++)
				{
					bsmPos.push_back(core::vector3df());
					core::vector3df& v=bsmPos.getLast();

					v.X=position.FloatArray[index++];
					v.Y=position.FloatArray[index++];
					v.Z=position.FloatArray[index++];
					bsm.transformVect(v);
					//Trace("bsmpos[%d]:{%g,%g,%g}\r\n",j,v.X,v.Y,v.Z);

					normalIndexs.clear();
					geometry->Mesh.getRelatedNormalIndexsByVertexIndex(j,normalIndexs);

					for(u32 k=0;k<normalIndexs.size();k++)
					{
						core::vector3df& n1=rsmNolStart[normalIndexs[k]];
						core::vector3df& n2=rsmNolEnd[normalIndexs[k]];
						n1.X=n1.Y=n1.Z=0;
						n2.X=geometry->Mesh.Normal.FloatArray[normalIndexs[k]*3];
						n2.Y=geometry->Mesh.Normal.FloatArray[normalIndexs[k]*3+1];
						n2.Z=geometry->Mesh.Normal.FloatArray[normalIndexs[k]*3+2];

						rsm.transformVect(n1);
						rsm.transformVect(n2);

						//Trace("rsmnol[%d]:{%g,%g,%g},{%g,%g,%g}\r\n",normalIndexs[k],n1.X,n1.Y,n1.Z,n2.X,n2.Y,n2.Z);
					}
				}

				//计算ibmJm
				u32 poseCount=ibms.size();
				for(u32 j=0;j<poseCount;j++)
				{
					ibmJm.push_back(core::matrix4());


					DAEJointNode* node=getJointNodeBySid(joint->NameArray[j].c_str(),libraryVisualScenes.VisualScene.RootJoint);
					if(node==NULL)
					{
						Trace("[▲WARN]No joint node found with sid:%s\r\n",joint->NameArray[j].c_str());
						continue;
					}
					else
					{
						Trace("[DEBUG]Find %s joint node sid:%s\r\n",joint->NameArray[j].c_str(),node->Sid.c_str());
					}
					core::matrix4& m=ibmJm.getLast();
					core::matrix4 temp=ibms[j]*node->AbsoluteTransform;
					m=temp.getTransposed();

					//printMatrix("ibm",ibms[j]);
					//printMatrix("jm",node->Transform);
				}

				printMatrix("bsm",bsm);



				core::array<core::vector3df> final;
				core::array<core::vector3df> finalNormalStart;
				core::array<core::vector3df> finalNormalEnd;
				finalNormalStart.set_used(geometry->Mesh.Normal.Count/3);
				finalNormalEnd.set_used(geometry->Mesh.Normal.Count/3);

				index=0;
				core::array<u32>& vcount=controller->Skin.VertexWeights.Vcount;
				core::array<u32>& v=controller->Skin.VertexWeights.V;
				DAESource* weights=controller->Skin.getWeightsSource();

				if(weights==NULL)
				{
					Trace("[▲WARN]No weights array found with controller whose node name:%s\r\n",node.Name.c_str());
					continue;
				}

				
				for(u32 j=0;j<vertexCount;j++)
				{

					final.push_back(core::vector3df());
					core::vector3df& vertex=final.getLast();


					u32 boneIndex;
					u32 weightIndex;
					for(u32 k=0;k<vcount[j];k++)
					{
						boneIndex=v[index++];
						weightIndex=v[index++];

						core::vector3df temp=bsmPos[j];
						ibmJm[boneIndex].transformVect(temp);
						temp*=weights->FloatArray[weightIndex];
						vertex+=temp;
					}

					normalIndexs.clear();
					geometry->Mesh.getRelatedNormalIndexsByVertexIndex(j,normalIndexs);

					for(u32 k=0;k<normalIndexs.size();k++)
					{
						core::vector3df& normal1=finalNormalStart[normalIndexs[k]];
						core::vector3df& normal2=finalNormalEnd[normalIndexs[k]];
						normal1.X=normal1.Y=normal1.Z=0;
						normal2.X=normal2.Y=normal2.Z=0;

						for(u32 l=0;l<vcount[j];l++)
						{
							core::vector3df temp=rsmNolStart[normalIndexs[k]];
							ibmJm[boneIndex].transformVect(temp);
							temp*=weights->FloatArray[weightIndex];
							normal1+=temp;

							temp=rsmNolEnd[normalIndexs[k]];
							ibmJm[boneIndex].transformVect(temp);
							temp*=weights->FloatArray[weightIndex];
							normal2+=temp;
						}
					}




					Trace("[%d]{%g,%g,%g},bind:",j,vertex.X,vertex.Z,vertex.Y);

					writer.writeFloat((float)vertex.X);
					writer.writeFloat((float)vertex.Z);
					writer.writeFloat((float)vertex.Y);

					//追加绑定骨骼信息
					EXTRAVertexWeights& w=controller->Skin.VertexWeights.VertexWeights[j];
					writer.writeInt((int)w.Count);
					for(u32 k=0;k<w.Count;k++)
					{
						u32 boneIndex=w.WeightComponents[k].BoneIndex;
						core::stringc boneName=controller->Skin.getJointsSource()->NameArray[boneIndex];
						DAEJointNode* joint=getJointNodeBySid(boneName.c_str(),libraryVisualScenes.VisualScene.RootJoint);
						boneIndex=joint->AbsoluteIndex;
						writer.writeInt((int)boneIndex);
						//writer.writeInt((int)w.WeightComponents[k].WeightIndex);
						writer.writeFloat((float)controller->Skin.getWeightsSource()->FloatArray[w.WeightComponents[k].WeightIndex]);

						//Trace("{%d(%s),%g},",boneIndex,boneName.c_str(),controller->Skin.getWeightsSource()->FloatArray[w.WeightComponents[k].WeightIndex]);
					}
					//Trace("\r\n");
				}

				//法线数据
				writer.writeInt((int)geometry->Mesh.Normal.Count);
				Trace("normal count:%d,final count:%d\r\n",geometry->Mesh.Normal.Count,finalNormalEnd.size());

				core::vector3df n;
				u32 ni;
				for(u32 j=0;j<geometry->Mesh.Normal.Count;j++)
				{
					if(j%3==0)
					{
						n.X=geometry->Mesh.Normal.FloatArray[j];
					}
					if(j%3==1)
					{
						n.Y=geometry->Mesh.Normal.FloatArray[j];
					}
					else if(j%3==2)
					{
						n.Z=geometry->Mesh.Normal.FloatArray[j];
						Trace("normals[%d]:{%g,%g,%g}-->",j/3,n.X,n.Y,n.Z);

						ni=j/3;
						n.X=finalNormalEnd[ni].X-finalNormalStart[ni].X;
						n.Y=finalNormalEnd[ni].Y-finalNormalStart[ni].Y;
						n.Z=finalNormalEnd[ni].Z-finalNormalStart[ni].Z;
						n.normalize();

						writer.writeFloat((float)n.X);
						writer.writeFloat((float)n.Z);
						writer.writeFloat((float)n.Y);
						Trace("{%g,%g,%g}\r\n",n.X,n.Z,n.Y);
						
					}
				}

			}
			else
			{
				core::matrix4 bsm=node.Matrix;
				printMatrix("bsm",bsm);
				bsm=bsm.getTransposed();

				DAESource& position=geometry->Mesh.Position;
				writer.writeInt((int)position.Count);

				core::vector3df p;
				for(u32 j=0;j<position.Count;j++)
				{
					if(j%3==0)
					{
						p.X=position.FloatArray[j];
					}
					if(j%3==1)
					{
						p.Y=position.FloatArray[j];
					}
					else if(j%3==2)
					{
						p.Z=position.FloatArray[j];
						Trace("vertices[%d]:{%g,%g,%g}-->",j/3,p.X,p.Y,p.Z);
						bsm.transformVect(p);

						writer.writeFloat((float)p.X);
						writer.writeFloat((float)p.Z);
						writer.writeFloat((float)p.Y);
						writer.writeInt(0);
						Trace("{%g,%g,%g}\r\n",p.X,p.Z,p.Y);
					}
				}

				//法线数据
				writer.writeInt((int)geometry->Mesh.Normal.Count);
				Trace("normal count:%d\r\n",geometry->Mesh.Normal.Count);
				core::matrix4 rsm=bsm;
				rsm[12]=0;
				rsm[13]=0;
				rsm[14]=0;
				
				core::vector3df n;
				for(u32 j=0;j<geometry->Mesh.Normal.Count;j++)
				{
					if(j%3==0)
					{
						n.X=geometry->Mesh.Normal.FloatArray[j];
					}
					if(j%3==1)
					{
						n.Y=geometry->Mesh.Normal.FloatArray[j];
					}
					else if(j%3==2)
					{
						n.Z=geometry->Mesh.Normal.FloatArray[j];
						Trace("normals[%d]:{%g,%g,%g}-->",j/3,n.X,n.Y,n.Z);
						rsm.transformVect(n);
						writer.writeFloat((float)n.X);
						writer.writeFloat((float)n.Z);
						writer.writeFloat((float)n.Y);
						Trace("{%g,%g,%g}\r\n",n.X,n.Z,n.Y);
					}
				}
			}

			

			//纹理坐标数据
			writer.writeInt((int)geometry->Mesh.MapChannel.Count);
			Trace("texcoord count:%d\r\n",geometry->Mesh.MapChannel.Count);
			for(u32 j=0;j<geometry->Mesh.MapChannel.Count;j++)
			{
				writer.writeFloat((float)geometry->Mesh.MapChannel.FloatArray[j]);
				if(j%3==2)
					Trace("{%g,%g,%g}\r\n",geometry->Mesh.MapChannel.FloatArray[j-2],geometry->Mesh.MapChannel.FloatArray[j-1],geometry->Mesh.MapChannel.FloatArray[j]);
			}
			//三角形数据
			writer.writeInt((int)geometry->Mesh.Triangles.size());
			for(u32 j=0;j<geometry->Mesh.Triangles.size();j++)
			{
				//使用的贴图
				const char* imageName=getImageNameByMaterialName(geometry->Mesh.Triangles[j].Material.c_str());
				if(imageName==NULL)
				{
					Trace("[▲WARN]No image found with material name:%s\r\n",geometry->Mesh.Triangles[j].Material.c_str());
					writer.writeString("",';');
				}
				else
					writer.writeString(imageName,';');
				Trace("image name:%s\r\n",imageName);

				//索引数据
				writer.writeInt((int)geometry->Mesh.Triangles[j].Vertexs.size());
				Trace("triangle indices count:%d\r\n",geometry->Mesh.Triangles[j].Vertexs.size());
				for(u32 k=0;k<geometry->Mesh.Triangles[j].Vertexs.size();k++)
				{
					writer.writeInt((int)geometry->Mesh.Triangles[j].Vertexs[k].Index);
					writer.writeInt((int)geometry->Mesh.Triangles[j].Vertexs[k].NormalIndex);
					writer.writeInt((int)geometry->Mesh.Triangles[j].Vertexs[k].TexCoordIndex);
					Trace("<%d,%d,%d>,",(int)geometry->Mesh.Triangles[j].Vertexs[k].Index,(int)geometry->Mesh.Triangles[j].Vertexs[k].NormalIndex,(int)geometry->Mesh.Triangles[j].Vertexs[k].TexCoordIndex);
					if(k%3==2)
					{
						Trace("\r\n");
					}
				}
			}
			Trace("\r\n");

		}
	}

#endif

	void exportModel(IWriteStream* writer)
	{
		int meshCount=0;
		for(u32 i=0;i<libraryVisualScenes.VisualScene.Nodes.size();i++)
		{
			DAENodeNode& node=libraryVisualScenes.VisualScene.Nodes[i];
			DAEGeometry* geometry=getGeometryByNodeName(node.Name.c_str());
			if(geometry==NULL)
			{
				continue;
			}
			meshCount++;
		}
		writer->writeInt(meshCount);

		Logger->debug("mesh count：%d\r\n",meshCount);

		//按Node.Name进行由小到大排序
		libraryVisualScenes.VisualScene.Nodes.sort();

		for(u32 i=0;i<libraryVisualScenes.VisualScene.Nodes.size();i++)
		{
			DAENodeNode& node=libraryVisualScenes.VisualScene.Nodes[i];
			DAEGeometry* geometry=getGeometryByNodeName(node.Name.c_str());
			if(geometry==NULL)
			{
				Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("No geometry found with node name:%s",node.Name.c_str()).c_str());
				continue;
			}
			else
			{
				Logger->debug("node name:%s,geometry->Id:%s\r\n",node.Name.c_str(),geometry->Id.c_str());
			}

			Logger->debug("name:%s,nodeName:%s\r\n",geometry->Name.c_str(),node.Name);
			writer->writeString(node.Name.c_str());
			
			writer->writeInt(node.MaterialTargets.size());

			DAEController* controller=getControllerBySkinSource(geometry->Id.c_str());
			Logger->debug("controller:%08x\r\n",controller);

			if(controller!=NULL)
			{
				core::matrix4f bsm=controller->Skin.BindShapeMatrix.getTransposed();
				core::matrix4f rsm=bsm;
				//rsm[12]=rsm[13]=rsm[14]=0;
				rsm.m[3][0]=rsm.m[3][1]=rsm.m[3][2]=0;
				core::array<core::matrix4f>& ibms=controller->Skin.InverseBindMatrixs;
				DAESource* joint=controller->Skin.getJointsSource();
				DAESource position=geometry->Mesh.Position;
				DAESource normal=geometry->Mesh.Normal;
				if(joint==NULL)
				{
					Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("No joint array found with controller whose node name:%s",node.Name.c_str()).c_str());
					continue;
				}

				writer->writeInt((int)position.Count);

				core::array<core::vector3df> bsmPos;
				core::array<core::matrix4f> ibmJm;

				core::array<core::vector3df> rsmNolStart;
				core::array<core::vector3df> rsmNolEnd;
				rsmNolStart.set_used(geometry->Mesh.Normal.Count/3);
				rsmNolEnd.set_used(geometry->Mesh.Normal.Count/3);
				core::array<u32> normalIndexs;

				//计算bsmPos
				u32 vertexCount=position.Count/3;
				u32 index=0;
				for(u32 j=0;j<vertexCount;j++)
				{
					bsmPos.push_back(core::vector3df());
					core::vector3df& v=bsmPos.getLast();

					v.x=position.FloatArray[index++];
					v.y=position.FloatArray[index++];
					v.z=position.FloatArray[index++];
					bsm.transformVect(v);
					//Trace("bsmpos[%d]:{%g,%g,%g}\r\n",j,v.X,v.Y,v.Z);

					normalIndexs.clear();
					geometry->Mesh.getRelatedNormalIndexsByVertexIndex(j,normalIndexs);

					for(u32 k=0;k<normalIndexs.size();k++)
					{
						core::vector3df& n1=rsmNolStart[normalIndexs[k]];
						core::vector3df& n2=rsmNolEnd[normalIndexs[k]];
						n1.x=n1.y=n1.z=0;
						n2.x=geometry->Mesh.Normal.FloatArray[normalIndexs[k]*3];
						n2.y=geometry->Mesh.Normal.FloatArray[normalIndexs[k]*3+1];
						n2.z=geometry->Mesh.Normal.FloatArray[normalIndexs[k]*3+2];

						rsm.transformVect(n1);
						rsm.transformVect(n2);

						//Trace("rsmnol[%d]:{%g,%g,%g},{%g,%g,%g}\r\n",normalIndexs[k],n1.X,n1.Y,n1.Z,n2.X,n2.Y,n2.Z);
					}
				}

				//计算ibmJm
				u32 poseCount=ibms.size();
				for(u32 j=0;j<poseCount;j++)
				{
					ibmJm.push_back(core::matrix4f(true));


					DAEJointNode* node=getJointNodeBySid(joint->NameArray[j].c_str(),libraryVisualScenes.VisualScene.RootJoint);
					if(node==NULL)
					{
						Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("No joint node found with sid:%s",joint->NameArray[j].c_str()).c_str());
						continue;
					}
					else
					{
						Logger->debug("Find %s joint node sid:%s\r\n",joint->NameArray[j].c_str(),node->Sid.c_str());
					}
					core::matrix4f& m=ibmJm.getLast();
					core::matrix4f temp=ibms[j]*node->AbsoluteTransform;
					m=temp.getTransposed();

					//printMatrix("ibm",ibms[j]);
					//printMatrix("jm",node->Transform);
				}

				printMatrix("bsm",bsm);



				core::array<core::vector3df> final;
				core::array<core::vector3df> finalNormalStart;
				core::array<core::vector3df> finalNormalEnd;
				finalNormalStart.set_used(geometry->Mesh.Normal.Count/3);
				finalNormalEnd.set_used(geometry->Mesh.Normal.Count/3);

				index=0;
				core::array<u32>& vcount=controller->Skin.VertexWeights.Vcount;
				core::array<u32>& v=controller->Skin.VertexWeights.V;
				DAESource* weights=controller->Skin.getWeightsSource();

				if(weights==NULL)
				{
					Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("No weights array found with controller whose node name:%s",node.Name.c_str()).c_str());
					continue;
				}

				
				for(u32 j=0;j<vertexCount;j++)
				{

					final.push_back(core::vector3df());
					core::vector3df& vertex=final.getLast();


					u32 boneIndex;
					u32 weightIndex;
					for(u32 k=0;k<vcount[j];k++)
					{
						boneIndex=v[index++];
						weightIndex=v[index++];

						core::vector3df temp=bsmPos[j];
						ibmJm[boneIndex].transformVect(temp);
						temp*=weights->FloatArray[weightIndex];
						vertex+=temp;
					}

					normalIndexs.clear();
					geometry->Mesh.getRelatedNormalIndexsByVertexIndex(j,normalIndexs);

					for(u32 k=0;k<normalIndexs.size();k++)
					{
						core::vector3df& normal1=finalNormalStart[normalIndexs[k]];
						core::vector3df& normal2=finalNormalEnd[normalIndexs[k]];
						normal1.x=normal1.y=normal1.z=0;
						normal2.x=normal2.y=normal2.z=0;

						for(u32 l=0;l<vcount[j];l++)
						{
							core::vector3df temp=rsmNolStart[normalIndexs[k]];
							ibmJm[boneIndex].transformVect(temp);
							temp*=weights->FloatArray[weightIndex];
							normal1+=temp;

							temp=rsmNolEnd[normalIndexs[k]];
							ibmJm[boneIndex].transformVect(temp);
							temp*=weights->FloatArray[weightIndex];
							normal2+=temp;
						}
					}




					Logger->debug("[%d]{%g,%g,%g},bind:",j,vertex.x,vertex.z,vertex.y);

					writer->writeFloat((float)vertex.x);
					writer->writeFloat((float)vertex.z);
					writer->writeFloat((float)vertex.y);

					//追加绑定骨骼信息
					EXTRAVertexWeights& w=controller->Skin.VertexWeights.VertexWeights[j];
					writer->writeInt((int)w.Count);
					for(u32 k=0;k<w.Count;k++)
					{
						u32 boneIndex=w.WeightComponents[k].BoneIndex;
						core::stringc boneName=controller->Skin.getJointsSource()->NameArray[boneIndex];
						DAEJointNode* joint=getJointNodeBySid(boneName.c_str(),libraryVisualScenes.VisualScene.RootJoint);
						boneIndex=joint->AbsoluteIndex;
						writer->writeInt((int)boneIndex);
						//writer.writeInt((int)w.WeightComponents[k].WeightIndex);
						writer->writeFloat((float)controller->Skin.getWeightsSource()->FloatArray[w.WeightComponents[k].WeightIndex]);

						//Trace("{%d(%s),%g},",boneIndex,boneName.c_str(),controller->Skin.getWeightsSource()->FloatArray[w.WeightComponents[k].WeightIndex]);
					}
					//Trace("\r\n");
				}

				//法线数据
				writer->writeInt((int)geometry->Mesh.Normal.Count);
				Logger->debug("normal count:%d,final count:%d\r\n",geometry->Mesh.Normal.Count,finalNormalEnd.size());

				core::vector3df n;
				u32 ni;
				for(u32 j=0;j<geometry->Mesh.Normal.Count;j++)
				{
					if(j%3==0)
					{
						n.x=geometry->Mesh.Normal.FloatArray[j];
					}
					if(j%3==1)
					{
						n.y=geometry->Mesh.Normal.FloatArray[j];
					}
					else if(j%3==2)
					{
						n.z=geometry->Mesh.Normal.FloatArray[j];
						Logger->debug("normals[%d]:{%g,%g,%g}-->",j/3,n.x,n.y,n.z);

						ni=j/3;
						n.x=finalNormalEnd[ni].x-finalNormalStart[ni].x;
						n.y=finalNormalEnd[ni].y-finalNormalStart[ni].y;
						n.z=finalNormalEnd[ni].z-finalNormalStart[ni].z;
						n.normalize();

						writer->writeFloat((float)n.x);
						writer->writeFloat((float)n.z);
						writer->writeFloat((float)n.y);
						Logger->debug("{%g,%g,%g}\r\n",n.x,n.z,n.y);
						
					}
				}

			}
			else
			{
				core::matrix4f bsm=node.Matrix;
				printMatrix("bsm",bsm);
				bsm=bsm.getTransposed();

				DAESource& position=geometry->Mesh.Position;
				writer->writeInt((int)position.Count);

				core::vector3df p;
				for(u32 j=0;j<position.Count;j++)
				{
					if(j%3==0)
					{
						p.x=position.FloatArray[j];
					}
					if(j%3==1)
					{
						p.y=position.FloatArray[j];
					}
					else if(j%3==2)
					{
						p.z=position.FloatArray[j];
						Logger->debug("vertices[%d]:{%g,%g,%g}-->",j/3,p.x,p.y,p.z);
						bsm.transformVect(p);

						writer->writeFloat((float)p.x);
						writer->writeFloat((float)p.z);
						writer->writeFloat((float)p.y);
						writer->writeInt(0);
						Logger->debug("{%g,%g,%g}\r\n",p.x,p.z,p.y);
					}
				}

				//法线数据
				writer->writeInt((int)geometry->Mesh.Normal.Count);
				Logger->debug("normal count:%d\r\n",geometry->Mesh.Normal.Count);
				core::matrix4f rsm=bsm;
				//rsm[12]=0;
				//rsm[13]=0;
				//rsm[14]=0;
				rsm.m[3][0]=0;
				rsm.m[3][1]=0;
				rsm.m[3][2]=0;
				
				core::vector3df n;
				for(u32 j=0;j<geometry->Mesh.Normal.Count;j++)
				{
					if(j%3==0)
					{
						n.x=geometry->Mesh.Normal.FloatArray[j];
					}
					if(j%3==1)
					{
						n.y=geometry->Mesh.Normal.FloatArray[j];
					}
					else if(j%3==2)
					{
						n.z=geometry->Mesh.Normal.FloatArray[j];
						Logger->debug("normals[%d]:{%g,%g,%g}-->",j/3,n.x,n.y,n.z);
						rsm.transformVect(n);
						writer->writeFloat((float)n.x);
						writer->writeFloat((float)n.z);
						writer->writeFloat((float)n.y);
						Logger->debug("{%g,%g,%g}\r\n",n.x,n.z,n.y);
					}
				}
			}

			

			//纹理坐标数据
			writer->writeInt((int)geometry->Mesh.MapChannel.Count);
			Logger->debug("texcoord count:%d\r\n",geometry->Mesh.MapChannel.Count);
			for(u32 j=0;j<geometry->Mesh.MapChannel.Count;j++)
			{
				writer->writeFloat((float)geometry->Mesh.MapChannel.FloatArray[j]);
				if(j%3==2)
					Logger->debug("{%g,%g,%g}\r\n",geometry->Mesh.MapChannel.FloatArray[j-2],geometry->Mesh.MapChannel.FloatArray[j-1],geometry->Mesh.MapChannel.FloatArray[j]);
			}
			//三角形数据
			writer->writeInt((int)geometry->Mesh.Triangles.size());
			for(u32 j=0;j<geometry->Mesh.Triangles.size();j++)
			{
				//使用的贴图
				const char* imageName=getImageNameByMaterialName(geometry->Mesh.Triangles[j].Material.c_str());
				if(imageName==NULL)
				{
					Logger->warn(YON_LOG_WARN_FORMAT,core::stringc("No image found with material name:%s",geometry->Mesh.Triangles[j].Material.c_str()).c_str());
					writer->writeString("");
				}
				else
					writer->writeString(imageName);
				Logger->debug("image name:%s\r\n",imageName);

				//索引数据
				writer->writeInt((int)geometry->Mesh.Triangles[j].Vertexs.size());
				Logger->debug("triangle indices count:%d\r\n",geometry->Mesh.Triangles[j].Vertexs.size());
				core::stringc str;
				for(u32 k=0;k<geometry->Mesh.Triangles[j].Vertexs.size();k++)
				{
					writer->writeInt((int)geometry->Mesh.Triangles[j].Vertexs[k].Index);
					writer->writeInt((int)geometry->Mesh.Triangles[j].Vertexs[k].NormalIndex);
					writer->writeInt((int)geometry->Mesh.Triangles[j].Vertexs[k].TexCoordIndex);

					str.append(core::stringc("<%d,%d,%d>,",(int)geometry->Mesh.Triangles[j].Vertexs[k].Index,(int)geometry->Mesh.Triangles[j].Vertexs[k].NormalIndex,(int)geometry->Mesh.Triangles[j].Vertexs[k].TexCoordIndex));
					//TODO str
					//Trace("<%d,%d,%d>,",(int)geometry->Mesh.Triangles[j].Vertexs[k].Index,(int)geometry->Mesh.Triangles[j].Vertexs[k].NormalIndex,(int)geometry->Mesh.Triangles[j].Vertexs[k].TexCoordIndex);
					if(k%3==2)
					{
						//Trace("\r\n");
						str.append("\r\n");
						Logger->debug("%s",str.c_str());

						str="";
					}
				}
			}
			//Trace("\r\n");
			Logger->debug("\r\n");

		}
	}


public:
	DAE2Y3DConvert(){
		DAEJointNode::Counter=0;

		//for(int i=0;i<16;i++)
		//	invert[i]=0;
		//invert[0]=invert[6]=invert[9]=invert[15]=1;
		for(u32 i=0;i<4;++i)
			for(u32 j=0;j<4;++j)
				invert.m[i][j]=0;
		invert.m[0][0]=invert.m[1][2]=invert.m[2][1]=invert.m[3][3]=1;
	}
	/*void openDAE(const char* filePath)
	{
		IrrXMLReader* xml=NULL;
		try
		{
			xml = createIrrXMLReader(filePath);

			while(xml&&xml->read())
			{
				switch(xml->getNodeType())
				{
				case EXN_ELEMENT:
					{
						const char* nodeName=xml->getNodeName();
						if(ELEM_COLLADA==nodeName)
						{
							readColladaSection(xml);
						}

					}
					break;
				default:
					skipSection(xml);
				}
			}
		}
		catch (const runtime_error& e)
		{
			Trace("【╳】Come around exception:%s\r\n",e.what());
		}

		// delete the xml parser after usage
		if(xml)
			delete xml;
	}

	void exportXC3D(const char* filePath)
	{
		CFileWriter writer(filePath);

		//导出骨骼
		Trace("\r\n--------Skeleton Data------\r\n");
		writer.writeBool(libraryVisualScenes.VisualScene.RootJoint!=NULL);
		if(libraryVisualScenes.VisualScene.RootJoint)
		{
			//导出播放速率
			writer.writeInt((int)libraryVisualScenes.VisualScene.FrameRate);

			DAEJointNode::Counter=0;
			exportJointNode(NULL,libraryVisualScenes.VisualScene.RootJoint,writer);
		}

		//导出模型
		exportModel(writer);

		writer.close();
	}*/

	void exportY3D(IWriteStream* stream)
	{
		//TODO joint
		exportModel(stream);
	}

};

u32 DAEJointNode::Counter=0;

void dae2y3d(XMLReader* reader,IWriteStream* stream){
}