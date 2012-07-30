#ifndef _YON_SCENE_QUADTREE_H_
#define _YON_SCENE_QUADTREE_H_

namespace yon{
namespace scene{
	///    TopLeft(TL)      TopRight(TR)
	///              0------1
	///              |      |
	///              |      |
	///              2------3
	/// BottomLeft(BL)      BottomRight(BR)
	enum ENUM_QUADTREE_AIRT{
		ENUM_QUADTREE_AIRT_TL = 0,
		ENUM_QUADTREE_AIRT_TR,
		ENUM_QUADTREE_AIRT_BL,
		ENUM_QUADTREE_AIRT_BR,
		ENUM_QUADTREE_AIRT_COUNT
	};

	template<class T> class quadtree;

	template<class T>
	class quadtreenode{
	private:
		friend class quadtree<T>;
	protected:
		quadtreenode<T>* m_pChilds[ENUM_QUADTREE_AIRT_COUNT];
		u32 m_uIndex;
	public:
		virtual ~quadtreenode(){}
		void setChild(ENUM_QUADTREE_AIRT airt,quadtreenode<T>* node){
			m_pChilds[airt]=node;
		}
		quadtreenode<T>* getChild(ENUM_QUADTREE_AIRT airt){
			return m_pChilds[airt];
		}

		bool isEmpty(){
			return m_pChilds[ENUM_QUADTREE_AIRT_TL]==NULL&&
				m_pChilds[ENUM_QUADTREE_AIRT_TR]==NULL&&
				m_pChilds[ENUM_QUADTREE_AIRT_BL]==NULL&&
				m_pChilds[ENUM_QUADTREE_AIRT_BR]==NULL;
		}
	};
}
}
#endif