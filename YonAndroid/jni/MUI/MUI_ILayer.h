#ifndef _MUI_ILAYER_H_
#define _MUI_ILAYER_H_

#include "dimension2d.h"
#include "yonString.h"
#include "IEventReceiver.h"

using namespace yon;

namespace mui{

	class ILayerItem;
	class ILayerNode;

	//refer to:http://blog.sina.com.cn/s/blog_be4206db01018wmy.html
	//MyGUI_Layers.xml文件内容也很简单。 
	//<MyGUI type="Layer" version="1.2">
	//<Layer type="SharedLayer" name="Wallpaper">
	//<Property key="Pick" value="false"/>
	//</Layer>
	//<Layer type="OverlappedLayer" name="Overlapped">
	//<Property key="Pick" value="true"/>
	//</Layer>
	//</MyGUI>
	//根节点MyGUI的type属性为Layer，表明当前定义的是Layer，也是界面上每个控件所位于的层次。在绘制时，第一个Layer总是最后一个绘制。
	//所以上面的第一个Layer节点被定义为了Wallpaper，也就是背景。
	//
	//Layer节点的类型为两类：SharedLayer和OverlappedLayer。位于SharedLayer层的控件是不能有重叠的情况发生的，否则会无法正确绘制。
	//在绘制时，整个ShareLayer层的控件被同时绘制出来。如果希望控件可以相互重叠，那么需要使用OverlappedLayer，但是在绘制的时候是按照根节点的控件顺序进行的。
	//
	//每一个Layer节点有一个Pick属性，当设为true时，鼠标可以点击这一层上的所有控件。当设为false时，这一层的所有控件都不会响应鼠标的点击事件。
	//如果有其他Pick属性为true的层的控件在这一层的下面，那么鼠标可以“穿过”当前层，点击那些层的控件。
	class ILayer{
	protected:
		core::stringc m_name;
	public:
		ILayer(){}
		virtual ~ILayer(){}

		const core::stringc& getName() const{
			return m_name;
		}

		//virtual ILayerNode* createChildItemNode() = 0;
		virtual ILayerNode* addChildNode() = 0;
		//virtual void destroyChildItemNode(ILayerNode* _node) = 0;
		virtual void removeChildNode(ILayerNode* node) = 0;

		//virtual void upChildItemNode(ILayerNode* _node) = 0;
		virtual void raiseChildNode(ILayerNode* node) = 0;

		//virtual size_t getLayerNodeCount() const = 0;
		virtual u32 getChildNodeCount() const = 0;

		//virtual ILayerNode* getLayerNodeAt(size_t _index) const = 0;
		virtual ILayerNode* getChildNodeAt(u32 index) const = 0;

		//virtual ILayerItem* getLayerItemByPoint(int _left, int _top) const = 0;
		virtual ILayerItem* getLayerItemByPosition(const core::position2di& pos) const = 0;

		//废弃，无论是OverlappedLayer还是SharedLayer都是直接返回position
		//virtual IntPoint getPosition(int _left, int _top) const = 0;
		//virtual core::position2di getPosition(const core::position2di& pos) const = 0;

		//virtual const IntSize& getSize() const = 0;
		virtual const core::dimension2di& getSize() const = 0;

		//virtual void renderToTarget(IRenderTarget* _target, bool _update) = 0;
		virtual void renderToTarget(IRenderTarget* _target) = 0;

		//virtual void resizeView(const IntSize& _viewSize) = 0;
		virtual void onResize(const core::dimension2di& size) = 0;
	};
}
#endif