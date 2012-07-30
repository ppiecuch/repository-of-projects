#ifndef _YON_SCENE_QUADTREE_H_
#define _YON_SCENE_QUADTREE_H_

namespace yon{
namespace scene{
	
	enum ENUM_QUADTREE_AIRT{
		ENUM_QUADTREE_AIRT_NORTH_EAST = 0,
		ENUM_QUADTREE_AIRT_NORTH_WEST,
		ENUM_QUADTREE_AIRT_SOUTH_WEST,
		ENUM_QUADTREE_AIRT_SOUTH_EAST,
		ENUM_QUADTREE_AIRT_COUNT
	};

	template<class T> class quadtree;

	template<class T>
	class quadtreenode{
	private:
		friend class quadtree<T>;
		quadtreenode<T>* m_pChilds[ENUM_QUADTREE_AIRT_COUNT];
	public:

		void setChild(ENUM_QUADTREE_AIRT airt,quadtreenode<T>* node){
			m_pChilds[airt]=node;
		}
		quadtreenode<T>* getChild(ENUM_QUADTREE_AIRT airt){
			return m_pChilds[airt];
		}

		bool isEmpty(){
			return m_pChilds[ENUM_QUADTREE_AIRT_NORTH_EAST]==NULL&&
				m_pChilds[ENUM_QUADTREE_AIRT_NORTH_WEST]==NULL&&
				m_pChilds[ENUM_QUADTREE_AIRT_SOUTH_WEST]==NULL&&
				m_pChilds[ENUM_QUADTREE_AIRT_SOUTH_EAST]==NULL;
		}
	};
}
}
#endif