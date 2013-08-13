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
	//MyGUI_Layers.xml�ļ�����Ҳ�ܼ򵥡� 
	//<MyGUI type="Layer" version="1.2">
	//<Layer type="SharedLayer" name="Wallpaper">
	//<Property key="Pick" value="false"/>
	//</Layer>
	//<Layer type="OverlappedLayer" name="Overlapped">
	//<Property key="Pick" value="true"/>
	//</Layer>
	//</MyGUI>
	//���ڵ�MyGUI��type����ΪLayer��������ǰ�������Layer��Ҳ�ǽ�����ÿ���ؼ���λ�ڵĲ�Ρ��ڻ���ʱ����һ��Layer�������һ�����ơ�
	//��������ĵ�һ��Layer�ڵ㱻����Ϊ��Wallpaper��Ҳ���Ǳ�����
	//
	//Layer�ڵ������Ϊ���ࣺSharedLayer��OverlappedLayer��λ��SharedLayer��Ŀؼ��ǲ������ص�����������ģ�������޷���ȷ���ơ�
	//�ڻ���ʱ������ShareLayer��Ŀؼ���ͬʱ���Ƴ��������ϣ���ؼ������໥�ص�����ô��Ҫʹ��OverlappedLayer�������ڻ��Ƶ�ʱ���ǰ��ո��ڵ�Ŀؼ�˳����еġ�
	//
	//ÿһ��Layer�ڵ���һ��Pick���ԣ�����Ϊtrueʱ�������Ե����һ���ϵ����пؼ�������Ϊfalseʱ����һ������пؼ���������Ӧ���ĵ���¼���
	//���������Pick����Ϊtrue�Ĳ�Ŀؼ�����һ������棬��ô�����ԡ���������ǰ�㣬�����Щ��Ŀؼ���
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

		//������������OverlappedLayer����SharedLayer����ֱ�ӷ���position
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