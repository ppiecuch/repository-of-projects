#ifndef _YON_CORE_XMLDATA_H_
#define _YON_CORE_XMLDATA_H_

#include "yonString.h"

namespace yon{
namespace core{

	class xmldata{
	public:
		/**
		* @brief Constructor creating an xmldata object with the given tag and id
		*
		* ie. <Skin id="001"/>
		* @param tag : the tag of the element
		* @param id : the id of the element
		*/
		xmldata(const core::stringc& tag, const core::stringc& id);

		xmldata(xmldata* data, bool cloneChildren=true);
		~xmldata();

		const core::stringc& getTag() const;

		/**
		* @brief Return true if the object contains the given property.
		*/
		bool containProperty(const core::stringc& name);

		/**
		* @brief Set the property of the given name with the given value.
		*/
		template <class T>
		void setProperty(const core::stringc& name, const T& value)
		{
			//TODO
			/*std::map< std::string, Data* >::iterator it = mData.find(propertyName);
			if(it != mData.end())
			{
				delete mData[propertyName];
				mData.erase(it);
			}

			mData[propertyName] = new DataImpl<T>(value);*/
		}

		/**
		* @brief Return the value of the propety by the given name.
		*/
		template <class T>
		T getProperty(const core::stringc& name)
		{
			/*if(mData.find(propertyName) != mData.end())
			{
				return ((DataImpl<T>*)mData[propertyName])->getValue();
			}
			else
			{
				Log::exception("XMLData::getProperty : No data with property name \"" + propertyName + "\" exists in this XMLData instance!");

				mData[propertyName] = new DataImpl<T>();
				return ((DataImpl<T>*)mData[propertyName])->getValue();
			}*/
			//TODO
			T t;
			return t;
		}

		/**
		* @brief Removes a property from this data set.
		*/
		void removeProperty(const core::stringc& name)
		{
		}
	};
}
}
#endif