#ifndef _YON_CORE_OBJECTPOOL_H_
#define _YON_CORE_OBJECTPOOL_H_

#include "yonTypes.h"
#include "yonList.h"

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
	class CObjectPoolFast : public IObjectPool<Element>{

		struct Chunk
		{
			enum { SIZE = 8 * 1024 - 16 };
			char storage_[SIZE];
			Chunk *next;
		};

		Element* m_pNext;
		Chunk* m_pChunk;

		void extend(){
			Chunk *p = new Chunk;
			p->next = m_pChunk;
			m_pChunk = p;

			const size_t elem = Chunk::SIZE / size_;
			char *start = p->storage_;
			char *last = &start[(elem - 1) * size_];

			for(char *p = start; p < last; p += size_)
				reinterpret_cast<Element*>(p)->next = reinterpret_cast<Element*>(p + size_);

			reinterpret_cast<Element*>(last)->next = 0;
			head_ = reinterpret_cast<Element*>(start);
		}
	public:
		CObjectPoolFast(u32 increment=5)
			:m_uIncrement(increment),m_uCapacity(0){
				extend(increment);
		}
		virtual ~CObjectPoolFast(){
			clear();
		}
		virtual u32 getSize() const {return m_pool.size();}
		virtual u32 getCapacity() const {return m_uCapacity;}
		virtual void clear(){
			Chunk *cur = m_pChunk;
			while(cur)
			{
				Chunk *p = cur;
				cur = cur->next;

				delete p;
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
				printf("Recycle no recyclable class,do nothing!\n");
			}
		}
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
				printf("Not all elements recycled!\n");
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
				printf("Recycle no recyclable class,do nothing!\n");
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