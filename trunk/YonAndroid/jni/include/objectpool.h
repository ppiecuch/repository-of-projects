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
		CObjectPoolNill(u32 num){}
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
			YON_DEBUG("Extend element->%d\n",m_uCapacity);
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
			//This because std::vector<A<T> *> is dependent on the template parameter (T) of the class template (A). 
			//To enable correct parsing of the template without having to make any assumptions about possible specializations of any other templates, 
			//the language rules require you to indicate which dependent names denote types by using the typename keyword.
			//reference: http://stackoverflow.com/questions/2841757/c-template-is-not-derived-from-type
			typename core::list<Element*>::Iterator it=m_pool.begin();
			while(it!=m_pool.end()){
				delete *it;
				++it;
			}
			m_pool.clear();
			m_uCapacity-=count;
			if(m_uCapacity!=0){
				YON_WARN(YON_LOG_WARN_FORMAT,"Not all elements recycled!");
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
				YON_WARN(YON_LOG_WARN_FORMAT,"Recycle no recyclable class,do nothing!");
			}
		}
	};

	template<class Element>
	class CObjectPoolFast : public IObjectPool<Element>{

		struct Link
		{
			Element* ele;
			Link* next;

			Link():ele(NULL),next(NULL){}
		};


		Link m_link;
		Element m_ele;
		Link* m_pFree;
		Link* m_pUsed;
		u32 m_uIncrement;
		u32 m_uSize;
		u32 m_uCapacity;
		yonAllocator<Link> linkAllocator;
		yonAllocator<Element> elementAllocator;

		void extend(u32 num){
			//Link* ns=linkAllocator.allocate(num);
			//Element* es=elementAllocator.allocate(num);
			Link* tmpL=new Link;
			for(u32 i=0;i<num;++i)
			{
				Element* tmpE=new Element;
				tmpL->ele=tmpE;

				if(i==0)
					m_pFree=tmpL;
				if(i<num-1)
					tmpL->next=new Link;
				else
					tmpL->next=NULL;
				tmpL=tmpL->next;
			}
			m_uCapacity+=num;
			m_uSize+=num;
			YON_DEBUG("Extend element->%d\n",m_uCapacity);
		}
	public:
		CObjectPoolFast(u32 increment=16)
			:m_uIncrement(increment),m_uSize(0),m_uCapacity(0),m_pFree(NULL),m_pUsed(NULL){
				extend(increment);
		}
		virtual ~CObjectPoolFast(){
			clear();
		}
		virtual u32 getSize() const {return m_uSize;}
		virtual u32 getCapacity() const {return m_uCapacity;}
		virtual void clear(){
			if(m_uSize!=m_uCapacity)
				YON_WARN(YON_LOG_WARN_FORMAT,"Not all elements cleared!");
			while(m_pFree)
			{
				Link* tmp=m_pFree->next;
				/*elementAllocator.destruct(m_pFree->ele);
				linkAllocator.destruct(m_pFree);
				elementAllocator.deallocate(m_pFree->ele);
				linkAllocator.deallocate(m_pFree);*/
				delete m_pFree->ele;
				delete m_pFree;
				m_pFree=tmp;
				--m_uCapacity;
				--m_uSize;
			}
		}
		virtual Element* get(){
			if(getSize()==0)
				extend(m_uIncrement);
			Link* tmp=m_pFree;
			m_pFree=m_pFree->next;
			tmp->next=m_pUsed;
			m_pUsed=tmp;
			--m_uSize;
			return tmp->ele;
		}
		virtual void recycle(Element* ele){
			IRecyclable* r=dynamic_cast<IRecyclable*>(ele);
			if(r){
				r->reset();
				Link* tmp=m_pUsed;
				m_pUsed=m_pUsed->next;
				tmp->next=m_pFree;
				tmp->ele=ele;
				m_pFree=tmp;
				++m_uSize;
			}else{
				YON_WARN(YON_LOG_WARN_FORMAT,"Recycle no recyclable class,do nothing!");
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