#ifndef _MUI_FACTORYMANAGER_H_
#define _MUI_FACTORYMANAGER_H_

#include "MUI_GenericFactory.h"
#include "MUI_Singleton.h"

namespace mui{

	//TODO 单例
	class FactoryManager : public Singleton<FactoryManager>{
	public:
		typedef core::CDelegate1<void,IObject*&> Delegate;
	private:
		//typedef std::map<std::string, Delegate> MapFactoryItem;
		//typedef std::map<std::string, MapFactoryItem> MapRegisterFactoryItem;
		//MapRegisterFactoryItem mRegisterFactoryItems;

		//bool mIsInitialise;

		//TODO string改为enum
		struct GeneratorId{
			core::stringc Category;
			core::stringc Type;

			GeneratorId(){}
			GeneratorId(const core::stringc& category,const core::stringc& type)
				:Category(category),Type(type){}

			bool operator<(const GeneratorId& o) const{
				//TODO 使用compare
				if(Category<o.Category)
					return true;
				if(o.Category<Category)
					return false;
				return Type<o.Type;
			};

			bool operator==(const GeneratorId& o) const{
				return Category==o.Category&&Type==o.Type;
			}
		};

		typedef core::map<GeneratorId,Delegate> GeneratorMap;
		GeneratorMap m_generators;

		bool m_bInitialized;
	public:
		FactoryManager();
		~FactoryManager();

		void initialize();
		void shutdown();
		
		/** Register delegate function that creates object for specified category and type. */
		void registerFactory(const core::stringc& category, const core::stringc& type, Delegate::IDelegate* dg);
		/** Unregister delegate function that creates object for specified category and type. */
		void unregisterFactory(const core::stringc& category, const core::stringc& type);
		/** Unregister all delegate functions that creates object for specified category. */
		void unregisterFactory(const core::stringc& category);

		/** Is factory for specified _category and _type exist. */
		//bool isFactoryExist(const std::string& _category, const std::string& _type);
		bool existFactory(const core::stringc& category, const core::stringc& type);

		/** Register factory for specified _category. */
		template<typename Type>
		void registerFactory(const core::stringc& category)
		{
			registerFactory(category, Type::getClassTypeName(), GenericFactory<Type>::getFactory());
		}

		/** Register factory for specified _category and _type. */
		template<typename Type>
		void registerFactory(const core::stringc& category, const core::stringc& type)
		{
			registerFactory(category, type, GenericFactory<Type>::getFactory());
		}

		/** Unegister factory for specified _category. */
		template<typename Type>
		void unregisterFactory(const core::stringc& category)
		{
			unregisterFactory(category, Type::getClassTypeName());
		}

		/** Create object with given category and type. Return nullptr if there's no registered factory. */
		//TODO UI池在这接口上扩展？
		IObject* createObject(const core::stringc& category, const core::stringc& type);
		/** Destroy object */
		void destroyObject(IObject* object);

	};
}
#endif