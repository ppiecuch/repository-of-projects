#ifndef _YON_CORE_OBJECTPOOL_H_
#define _YON_CORE_OBJECTPOOL_H_

#include "yonTypes.h"
#include "yonList.h"
#include "ILogger.h"

namespace yon{
namespace core{

	class IRecyclable{
	public:
		virtual void reset() = 0;
	};

	template<class Element>
	class IObjectPool{
	public:
		virtual ~IObjectPool(){}
		virtual u32 getSize() const = 0;
		virtual u32 getCapacity() const = 0;
		virtual void clear() = 0;
		virtual Element* get() = 0;
		virtual void recycle(Element* ele) = 0;
	};

	template<class Element>
	class CObjectPoolNill : public IObjectPool<Element>{
	public:
		virtual u32 getSize() const{return 0;}
		virtual u32 getCapacity() const{return 0;}
		virtual void clear(){}
		virtual Element* get(){return new Element;}
		virtual void recycle(Element* ele){delete ele;}
	};

	template<class Element>
	class CObjectPool : public IObjectPool<Element>{
	protected:
		u32 m_uCapacity;
		u32 m_uIncrement;
		core::list<Element*> m_pool;

		virtual void extend(u32 num){
			for(u32 i=0;i<num;++i)
				m_pool.push_back(new Element());
			m_uCapacity+=num;
			Logger->debug("Extend element->%d\n",m_uCapacity);
		}
	public:
		CObjectPool(u32 increment=5)
			:m_uIncrement(increment),m_uCapacity(0){
				extend(increment);
		}
		virtual ~CObjectPool(){
			clear();
		}
		virtual u32 getSize() const {return m_pool.size();}
		virtual u32 getCapacity() const {return m_uCapacity;}
		virtual void clear(){
			u32 count=getSize();
			core::list<Element*>::Iterator it=m_pool.begin();
			while(it!=m_pool.end()){
				delete *it;
				++it;
			}
			m_pool.clear();
			m_uCapacity-=count;
			if(m_uCapacity!=0){
				Logger->warn(YON_LOG_WARN_FORMAT,"Not all elements recycled!");
			}
		}
		virtual Element* get(){
			if(getSize()==0)
				extend(m_uIncrement);
			Element* ele=*(m_pool.begin());
			//m_pool.pop_front();
			m_pool.erase(m_pool.begin());
			return ele;
		}
		virtual void recycle(Element* ele){
			IRecyclable* r=dynamic_cast<IRecyclable*>(ele);
			if(r){
				r->reset();
				m_pool.push_back(ele);
			}else{
				Logger->warn(YON_LOG_WARN_FORMAT,"Recycle no recyclable class,do nothing!");
			}
		}
	};
	template<class Element>
	class CObjectPoolSecure : public IObjectPool<Element>{
		//TODO
	};
}
}
#endif