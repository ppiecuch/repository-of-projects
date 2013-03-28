#include <stdio.h>
#include <stdlib.h>

//refer to:http://www.boost.org/doc/libs/1_49_0/boost/mpl/if.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/detail/sp_typeinfo.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/current_function.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/ref.hpp
//refer to:http://songyiwei1984.blog.163.com/blog/static/1121277402011026104031978/
//refer to:http://blog.csdn.net/demon__hunter/article/details/5450498
//refer to:http://www.boost.org/doc/libs/1_34_1/boost/type_traits/is_member_pointer.hpp

inline void current_function_helper()
{

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)

# define YON_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__DMC__) && (__DMC__ >= 0x810)

# define YON_CURRENT_FUNCTION __PRETTY_FUNCTION__

#elif defined(__FUNCSIG__)

# define YON_CURRENT_FUNCTION __FUNCSIG__

#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))

# define YON_CURRENT_FUNCTION __FUNCTION__

#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)

# define YON_CURRENT_FUNCTION __FUNC__

#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)

# define YON_CURRENT_FUNCTION __func__

#else

# define YON_CURRENT_FUNCTION "(unknown)"

#endif

}

template<typename T> struct is_pointer{enum{value=0};};
template<typename T> struct is_pointer<T*>{enum{value=1};};

template<bool C, typename T1, typename T2>
struct if_c
{
	typedef T1 type;
};

template<typename T1, typename T2>
struct if_c<false,T1,T2>
{
	typedef T2 type;
};
template<class T> 
struct addr_impl_ref
{
	T & v_;

	inline addr_impl_ref( T & v ): v_( v ) {}
	inline operator T& () const { return v_; }

private:
	addr_impl_ref & operator=(const addr_impl_ref &);
};

template<class T> 
struct addressof_impl
{
	static inline T * f( T & v, long )
	{
		//这是关键的地方，先强制转换成const volatile char &，然后去掉const属性，再取址，避免调用v的重载的operator&  
		return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char &>(v)));
	}

	static inline T * f( T * v, int )
	{
		return v;
	}
};

template<class T> 
T* addressof( T & v )
{
//#if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT( 0x610 ) )
//	return boost::detail::addressof_impl<T>::f( v, 0 );
//#else
	return addressof_impl<T>::f( addr_impl_ref<T>( v ), 0 );
//#endif
}

//以上是模板函数addressof()的实现原理，实际是一个cast trick，c++标准中有如下的规定：
//An lvalue expression of type T1 can be cast to the type “reference to T2” if an expression of type “pointer
//to T1” can be explicitly converted to the type “pointer to T2” using a reinterpret_cast. That is, a
//reference cast reinterpret_cast<T&>(x) has the same effect as the conversion
//*reinterpret_cast<T*>(&x) with the built-in & and * operators. The result is an lvalue that refers
//to the same object as the source lvalue, but with a different type. No temporary is created, no copy is made,
//and constructors (12.1) or conversion functions (12.3) are not called.67)  (ISO/IEC 14882:2003(E)  5.2.10 Reinterpret cast)


template<class T> class reference_wrapper
{ 
public:
	typedef T type;

//#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, < 1300 )
//	explicit reference_wrapper(T& t): t_(&t) {}
//#else
	explicit reference_wrapper(T& t): t_(addressof(t)) {}
//#endif

	operator T& () const { return *t_; }

	T& get() const { return *t_; }

	T* get_pointer() const { return t_; }

private:

	T* t_;
};

//# if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT(0x581) )
//#  define BOOST_REF_CONST
//# else
#  define BOOST_REF_CONST const
//# endif

template<class T> 
inline reference_wrapper<T> BOOST_REF_CONST ref(T & t)
{ 
	return reference_wrapper<T>(t);
}

template<class T> 
inline reference_wrapper<T const> BOOST_REF_CONST cref(T const & t)
{
	return reference_wrapper<T const>(t);
}

# undef BOOST_REF_CONST

//# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

// BOOST_STATIC_CONSTANT workaround --------------------------------------- //
// On compilers which don't allow in-class initialization of static integral
// constant members, we must use enums as a workaround if we want the constants
// to be available at compile-time. This macro gives us a convenient way to
// declare such constants.

#  ifdef BOOST_NO_INCLASS_MEMBER_INITIALIZATION
#       define BOOST_STATIC_CONSTANT(type, assignment) enum { assignment }
#  else
#     define BOOST_STATIC_CONSTANT(type, assignment) static const type assignment
#  endif

template< bool C_ > struct bool_
{
	BOOST_STATIC_CONSTANT(bool, value = C_);
	//typedef integral_c_tag tag;
	typedef bool_ type;
	typedef bool value_type;
	operator bool() const { return this->value; }
};
// shorcuts
typedef bool_<true> true_;
typedef bool_<false> false_;

template<typename T>
class is_reference_wrapper : public false_
{
};

template<typename T>
class unwrap_reference
{
public:
	typedef T type;
};


class sp_typeinfo
{
private:

	sp_typeinfo(const sp_typeinfo & );
	sp_typeinfo& operator=(const sp_typeinfo & );

	const char *name_;

public:

	explicit sp_typeinfo( const char * name ): name_( name )
	{
	}

	bool operator==(const sp_typeinfo & rhs ) const
	{
		return this == &rhs;
	}

	bool operator!=(const sp_typeinfo & rhs ) const
	{
		return this != &rhs;
	}

	bool before(const sp_typeinfo & rhs ) const
	{
		//TODO right?
		//return std::less< sp_typeinfo const* >()( this, &rhs );
		return this<&rhs;
	}

	const char* name() const
	{
		return name_;
	}
};

template<class T> struct sp_typeid_
{
	static sp_typeinfo ti_;

	static const char * name()
	{
		return YON_CURRENT_FUNCTION;
	}
};


class X;

/**
* A buffer used to store small function objects in
* boost::function. It is a union containing function pointers,
* object pointers, and a structure that resembles a bound
* member function pointer.
*/

union function_buffer
{
	// For pointers to function objects
	mutable void* obj_ptr;

	// For pointers to std::type_info objects
	struct type_t {
		// (get_functor_type_tag, check_functor_type_tag).
		const sp_typeinfo* type;

		// Whether the type is const-qualified.
		bool const_qualified;
		// Whether the type is volatile-qualified.
		bool volatile_qualified;
	} type;

	// For function pointers of all kinds
	mutable void (*func_ptr)();

	// For bound member pointers
	struct bound_memfunc_ptr_t {
		void (X::*memfunc_ptr)(int);
		void* obj_ptr;
	} bound_memfunc_ptr;

	// For references to function objects. We explicitly keep
	// track of the cv-qualifiers on the object referenced.
	struct obj_ref_t {
		mutable void* obj_ptr;
		bool is_const_qualified;
		bool is_volatile_qualified;
	} obj_ref;

	// To relax aliasing constraints
	mutable char data;
};

template <typename T>
struct is_mem_fun_pointer_impl
{
	static const bool value = false;
};

// 偏特化版本,匹配无参数的成员函数
template <class R, class T > 
struct is_mem_fun_pointer_impl<R (T::*)() >
{
	static const bool value = true;
};

//匹配一个参数的成员函数
template <class R, class T , class T0>
struct is_mem_fun_pointer_impl<R (T::*)( T0) >
{
	static const bool value = true;
};

template<class T>
struct is_mem_function_pointer
{
	static const bool value = is_mem_fun_pointer_impl<T>::value;
};

template <bool>
struct is_member_pointer_select
	: ::boost::type_traits::false_result
{
};

template <>
struct is_member_pointer_select<false>
{
	template <typename T> struct result_
	{
		static T* make_t();
		BOOST_STATIC_CONSTANT(
			bool, value =
			(::boost::type_traits::ice_or<
			(1 == sizeof(::boost::type_traits::is_mem_fun_pointer_tester(make_t()))),
			(1 == sizeof(is_member_pointer_tester(make_t())))
			>::value) );
	};
};

template <typename T>
struct is_member_pointer_impl
	: is_member_pointer_select<
	::boost::type_traits::ice_or<
	::boost::is_reference<T>::value
	, ::boost::is_array<T>::value
	>::value
		>::template result_<T>
{
};
//bool_trait_def.hpp
template <> 
struct is_member_pointer_impl<void>
{
	BOOST_STATIC_CONSTANT(bool, value = false);
};
template <> 
struct is_member_pointer_impl<const void>
{
	BOOST_STATIC_CONSTANT(bool, value = false);
};
template <> 
struct is_member_pointer_impl<volatile void>
{
	BOOST_STATIC_CONSTANT(bool, value = false);
};
template <> 
struct is_member_pointer_impl<const volatile void>
{
	BOOST_STATIC_CONSTANT(bool, value = false);
};

//refer to: mpl\aux_\lambda_support.hpp

#define BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
	typedef BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE::int_<i> arity; \
	BOOST_PP_LIST_FOR_EACH_I_R( \
	1 \
	, BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC \
	, typedef \
	, BOOST_PP_TUPLE_TO_LIST(i,params) \
	) \
	friend class BOOST_PP_CAT(name,_rebind); \
	typedef BOOST_PP_CAT(name,_rebind) rebind; \
	/**/

#define BOOST_MPL_AUX_LAMBDA_SUPPORT(i, name, params) \
	BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
}; \
	BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
class BOOST_PP_CAT(name,_rebind) \
{ \
public: \
	template< BOOST_MPL_PP_PARAMS(i,typename U) > struct apply \
	: name< BOOST_MPL_PP_PARAMS(i,U) > \
{ \
}; \
/**/

//refer to:type_traits\detail\bool_trait_def.hpp
#define BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
	typedef ::boost::integral_constant<bool,C> base_; \
	using base_::value; \
	/**/

#define BOOST_TT_AUX_BOOL_C_BASE(C) : public ::boost::integral_constant<bool,C>

#define BOOST_TT_AUX_BOOL_TRAIT_DEF1(trait,T,C) \
template< typename T > struct trait \
	BOOST_TT_AUX_BOOL_C_BASE(C) \
{ \
public:\
	BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
	BOOST_MPL_AUX_LAMBDA_SUPPORT(1,trait,(T)) \
}; \
	\
	BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1,trait) \
	/**/

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,::boost::detail::is_member_pointer_impl<T>::value)

/*

template<class T>
struct is_member_pointer : public ::boost::integral_constant<bool,is_member_pointer_impl<T>::value>
{
public:
	typedef int_<1> arity;
	typedef_ param BOOST_PP_CAT(arg,BOOST_PP_INC(i));
	BOOST_PP_LIST_FOR_EACH_I_R( 
		1 
		, BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC 
		, typedef 
		, BOOST_PP_TUPLE_TO_LIST(i,params) 
		) 
	struct rebind 
	{ 
	template< BOOST_MPL_PP_PARAMS(i,typename U) > struct apply 
	: name< BOOST_MPL_PP_PARAMS(i,U) > 
		{ 
		}; 
	};
	//	   BOOST_MPL_AUX_LAMBDA_SUPPORT(1,trait,(T)) \
}; 
BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(1,is_member_pointer) \
*/
// The operation type to perform on the given functor/function pointer
enum functor_manager_operation_type {
	clone_functor_tag,
	move_functor_tag,
	destroy_functor_tag,
	check_functor_type_tag,
	get_functor_type_tag
};

// Tags used to decide between different types of functions
struct function_ptr_tag {};
struct function_obj_tag {};
struct member_ptr_tag {};
struct function_obj_ref_tag {};

template<typename F>
class get_function_tag
{
	typedef typename if_c<(is_pointer<F>::value),
		function_ptr_tag,
		function_obj_tag>::type ptr_or_obj_tag;

	typedef typename if_c<(is_member_pointer<F>::value),
		member_ptr_tag,
		ptr_or_obj_tag>::type ptr_or_obj_or_mem_tag;

	typedef typename if_c<(is_reference_wrapper<F>::value),
		function_obj_ref_tag,
		ptr_or_obj_or_mem_tag>::type or_ref_tag;

public:
	typedef or_ref_tag type;
};

void test0()
{
	printf("test0\r\n");
}

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}

class obj{};

int main(int argc,char ** argv)
{
	EnableMemLeakCheck();

	printf("%s\r\n",YON_CURRENT_FUNCTION);
	
	system("pause");
	return 0;
}