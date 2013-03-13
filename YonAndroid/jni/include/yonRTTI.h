#ifndef _YON_CORE_YONRTTI_H_
#define _YON_CORE_YONRTTI_H_

#include "yonString.h"

namespace yon{
namespace core{

#define YON_RTTI_TYPE const core::stringc&
#define YON_RTTI_GET_TYPE(Type) Type::getClassName()

	//VC++ 7.1
#if defined( _MSC_VER ) && _MSC_VER <= 1310
#define YON_DECLARE_TYPE_NAME(Type) \
private: \
	struct TypeNameHolder { const core::stringc& getClassName() { static core::stringc type = #Type; return type; } }; \
public: \
	static const core::stringc& getClassName() { TypeNameHolder type; return type.getClassName(); } \
	/** Get type name as string */ \
	virtual const core::stringc& getTypeName() const { return getClassName(); }
#else
#define YON_DECLARE_TYPE_NAME(Type) \
public: \
	static const core::stringc& getClassName() { static core::stringc type = #Type; return type; } \
	/** Get type name as string */ \
	virtual const core::stringc& getTypeName() const { return getClassName(); }
#endif

#define YON_RTTI_BASE(BaseType) \
public: \
	typedef BaseType RTTIBase; \
	YON_DECLARE_TYPE_NAME(BaseType) \
	/** Compare with selected type */ \
	virtual bool isType(YON_RTTI_TYPE _type) const { return YON_RTTI_GET_TYPE(BaseType) == _type; } \
	/** Compare with selected type */ \
	template<typename Type> bool isType() const { return isType(YON_RTTI_GET_TYPE(Type)); } \
	/** Try to cast pointer to selected type. \
		@param _throw If true throw exception when casting in wrong type, else return NULL \
	*/ \
	template<typename Type> Type* castType(bool _throw = true) \
	{ \
		YON_DEBUG_BREAK_IF(_throw&&this->isType<Type>()==false); \
		if (this->isType<Type>()) return static_cast<Type*>(this); \
		return NULL; \
	} \
	/** Try to cast pointer to selected type. \
		@param _throw If true throw exception when casting in wrong type, else return NULL \
	*/ \
	template<typename Type> const Type* castType(bool _throw = true) const \
	{ \
		YON_DEBUG_BREAK_IF(_throw&&this->isType<Type>()==false); \
		if (this->isType<Type>()) return static_cast<Type*>(this); \
		return NULL; \
	}

#define YON_RTTI_DERIVED(DerivedType) \
public: \
	YON_DECLARE_TYPE_NAME(DerivedType) \
	//TODO 此处的RTTIBase是不是基类？
	typedef RTTIBase Base; \
	//TODO RTTIBase没用到啊
	typedef DerivedType RTTIBase; \
	/** Compare with selected type */ \
	virtual bool isType(YON_RTTI_TYPE _type) const { return YON_RTTI_GET_TYPE(DerivedType) == _type || Base::isType(_type); } \
	/** Compare with selected type */ \
	template<typename Type> bool isType() const { return isType(YON_RTTI_GET_TYPE(Type)); }
}
}
#endif