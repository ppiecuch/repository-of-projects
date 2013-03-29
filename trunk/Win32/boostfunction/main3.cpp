#include <stdio.h>
#include <stdlib.h>

//refer to:http://www.boost.org/doc/libs/1_49_0/boost/mpl/if.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/detail/sp_typeinfo.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/current_function.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/ref.hpp
//refer to:http://songyiwei1984.blog.163.com/blog/static/1121277402011026104031978/
//refer to:http://blog.csdn.net/demon__hunter/article/details/5450498
//refer to:http://blog.csdn.net/pongba/article/details/83828
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

//refer to:type_traits\detail\false_result.hpp
// Utility class which always "returns" false
struct false_result
{
	template <typename T> struct result_
	{
		BOOST_STATIC_CONSTANT(bool, value = false);
	};
};

//refer to:type_traits\detail\ice_or.hpp
//相当于逻辑或
template <bool b1, bool b2, bool b3 = false, bool b4 = false, bool b5 = false, bool b6 = false, bool b7 = false>
struct ice_or;

template <bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, bool b7>
struct ice_or
{
	BOOST_STATIC_CONSTANT(bool, value = true);
};

template <>
struct ice_or<false, false, false, false, false, false, false>
{
	BOOST_STATIC_CONSTANT(bool, value = false);
};


template <bool>
struct is_member_pointer_select : false_result
{
};

//refer to:container\detail\type_traits.hpp
template<class T>
struct is_reference
{
	enum {  value = false   };
};

template<class T>
struct is_reference<T&>
{
	enum {  value = true   };
};

template <>
struct is_member_pointer_select<false>
{
	template <typename T> struct result_
	{
		static T* make_t();
		BOOST_STATIC_CONSTANT(
			bool, value =
			(ice_or<
			(1 == sizeof(is_mem_fun_pointer_tester(make_t()))),
			(1 == sizeof(is_member_pointer_tester(make_t())))
			>::value) );
	};
};

// 缺省
template<typename T>
struct is_array
{
	static const bool value=false;
};

// 偏特化
template<typename T,size_t N>
struct is_array<T[N]>
{
	static const bool value=true;
};

template <typename T>
struct is_member_pointer_impl
	: is_member_pointer_select<
	ice_or<
	is_reference<T>::value
	, is_array<T>::value
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

//refer to:preprocessor/arithmetic/inc.hpp
#    define BOOST_PP_INC(x) BOOST_PP_INC_I(x)
# define BOOST_PP_INC_I(x) BOOST_PP_INC_ ## x
# define BOOST_PP_INC_0 1
# define BOOST_PP_INC_1 2
# define BOOST_PP_INC_2 3
# define BOOST_PP_INC_3 4
# define BOOST_PP_INC_4 5

//refer to:preprocessor/control/iif.hpp
#    define BOOST_PP_IIF(bit, t, f) BOOST_PP_IIF_OO((bit, t, f))
#    define BOOST_PP_IIF_OO(par) BOOST_PP_IIF_I ## par
#    define BOOST_PP_IIF_I(bit, t, f) BOOST_PP_IIF_II(BOOST_PP_IIF_ ## bit(t, f))
#    define BOOST_PP_IIF_II(id) id

# define BOOST_PP_IIF_0(t, f) f
# define BOOST_PP_IIF_1(t, f) t

//refer to:preprocessor/detail/dmc/auto_rec.hpp
# define BOOST_PP_AUTO_REC(pred, n) BOOST_PP_NODE_ENTRY_ ## n(pred)

# define BOOST_PP_NODE_ENTRY_256(p) BOOST_PP_NODE_128(p)(p)(p)(p)(p)(p)(p)(p)
# define BOOST_PP_NODE_ENTRY_128(p) BOOST_PP_NODE_64(p)(p)(p)(p)(p)(p)(p)
# define BOOST_PP_NODE_ENTRY_64(p) BOOST_PP_NODE_32(p)(p)(p)(p)(p)(p)
# define BOOST_PP_NODE_ENTRY_32(p) BOOST_PP_NODE_16(p)(p)(p)(p)(p)
# define BOOST_PP_NODE_ENTRY_16(p) BOOST_PP_NODE_8(p)(p)(p)(p)
# define BOOST_PP_NODE_ENTRY_8(p) BOOST_PP_NODE_4(p)(p)(p)
# define BOOST_PP_NODE_ENTRY_4(p) BOOST_PP_NODE_2(p)(p)
# define BOOST_PP_NODE_ENTRY_2(p) BOOST_PP_NODE_1(p)

# define BOOST_PP_NODE_128(p) BOOST_PP_IIF(p##(128), BOOST_PP_NODE_64, BOOST_PP_NODE_192)
#    define BOOST_PP_NODE_64(p) BOOST_PP_IIF(p##(64), BOOST_PP_NODE_32, BOOST_PP_NODE_96)
#        define BOOST_PP_NODE_32(p) BOOST_PP_IIF(p##(32), BOOST_PP_NODE_16, BOOST_PP_NODE_48)
#            define BOOST_PP_NODE_16(p) BOOST_PP_IIF(p##(16), BOOST_PP_NODE_8, BOOST_PP_NODE_24)
#                define BOOST_PP_NODE_8(p) BOOST_PP_IIF(p##(8), BOOST_PP_NODE_4, BOOST_PP_NODE_12)
#                    define BOOST_PP_NODE_4(p) BOOST_PP_IIF(p##(4), BOOST_PP_NODE_2, BOOST_PP_NODE_6)
#                        define BOOST_PP_NODE_2(p) BOOST_PP_IIF(p##(2), BOOST_PP_NODE_1, BOOST_PP_NODE_3)
#                            define BOOST_PP_NODE_1(p) BOOST_PP_IIF(p##(1), 1, 2)
#                            define BOOST_PP_NODE_3(p) BOOST_PP_IIF(p##(3), 3, 4)
#                        define BOOST_PP_NODE_6(p) BOOST_PP_IIF(p##(6), BOOST_PP_NODE_5, BOOST_PP_NODE_7)
#                            define BOOST_PP_NODE_5(p) BOOST_PP_IIF(p##(5), 5, 6)
#                            define BOOST_PP_NODE_7(p) BOOST_PP_IIF(p##(7), 7, 8)
#                    define BOOST_PP_NODE_12(p) BOOST_PP_IIF(p##(12), BOOST_PP_NODE_10, BOOST_PP_NODE_14)
#                        define BOOST_PP_NODE_10(p) BOOST_PP_IIF(p##(10), BOOST_PP_NODE_9, BOOST_PP_NODE_11)
#                            define BOOST_PP_NODE_9(p) BOOST_PP_IIF(p##(9), 9, 10)
#                            define BOOST_PP_NODE_11(p) BOOST_PP_IIF(p##(11), 11, 12)
#                        define BOOST_PP_NODE_14(p) BOOST_PP_IIF(p##(14), BOOST_PP_NODE_13, BOOST_PP_NODE_15)
#                            define BOOST_PP_NODE_13(p) BOOST_PP_IIF(p##(13), 13, 14)
#                            define BOOST_PP_NODE_15(p) BOOST_PP_IIF(p##(15), 15, 16)
#                define BOOST_PP_NODE_24(p) BOOST_PP_IIF(p##(24), BOOST_PP_NODE_20, BOOST_PP_NODE_28)
#                    define BOOST_PP_NODE_20(p) BOOST_PP_IIF(p##(20), BOOST_PP_NODE_18, BOOST_PP_NODE_22)
#                        define BOOST_PP_NODE_18(p) BOOST_PP_IIF(p##(18), BOOST_PP_NODE_17, BOOST_PP_NODE_19)
#                            define BOOST_PP_NODE_17(p) BOOST_PP_IIF(p##(17), 17, 18)
#                            define BOOST_PP_NODE_19(p) BOOST_PP_IIF(p##(19), 19, 20)
#                        define BOOST_PP_NODE_22(p) BOOST_PP_IIF(p##(22), BOOST_PP_NODE_21, BOOST_PP_NODE_23)
#                            define BOOST_PP_NODE_21(p) BOOST_PP_IIF(p##(21), 21, 22)
#                            define BOOST_PP_NODE_23(p) BOOST_PP_IIF(p##(23), 23, 24)
#                    define BOOST_PP_NODE_28(p) BOOST_PP_IIF(p##(28), BOOST_PP_NODE_26, BOOST_PP_NODE_30)
#                        define BOOST_PP_NODE_26(p) BOOST_PP_IIF(p##(26), BOOST_PP_NODE_25, BOOST_PP_NODE_27)
#                            define BOOST_PP_NODE_25(p) BOOST_PP_IIF(p##(25), 25, 26)
#                            define BOOST_PP_NODE_27(p) BOOST_PP_IIF(p##(27), 27, 28)
#                        define BOOST_PP_NODE_30(p) BOOST_PP_IIF(p##(30), BOOST_PP_NODE_29, BOOST_PP_NODE_31)
#                            define BOOST_PP_NODE_29(p) BOOST_PP_IIF(p##(29), 29, 30)
#                            define BOOST_PP_NODE_31(p) BOOST_PP_IIF(p##(31), 31, 32)
#            define BOOST_PP_NODE_48(p) BOOST_PP_IIF(p##(48), BOOST_PP_NODE_40, BOOST_PP_NODE_56)
#                define BOOST_PP_NODE_40(p) BOOST_PP_IIF(p##(40), BOOST_PP_NODE_36, BOOST_PP_NODE_44)
#                    define BOOST_PP_NODE_36(p) BOOST_PP_IIF(p##(36), BOOST_PP_NODE_34, BOOST_PP_NODE_38)
#                        define BOOST_PP_NODE_34(p) BOOST_PP_IIF(p##(34), BOOST_PP_NODE_33, BOOST_PP_NODE_35)
#                            define BOOST_PP_NODE_33(p) BOOST_PP_IIF(p##(33), 33, 34)
#                            define BOOST_PP_NODE_35(p) BOOST_PP_IIF(p##(35), 35, 36)
#                        define BOOST_PP_NODE_38(p) BOOST_PP_IIF(p##(38), BOOST_PP_NODE_37, BOOST_PP_NODE_39)
#                            define BOOST_PP_NODE_37(p) BOOST_PP_IIF(p##(37), 37, 38)
#                            define BOOST_PP_NODE_39(p) BOOST_PP_IIF(p##(39), 39, 40)
#                    define BOOST_PP_NODE_44(p) BOOST_PP_IIF(p##(44), BOOST_PP_NODE_42, BOOST_PP_NODE_46)
#                        define BOOST_PP_NODE_42(p) BOOST_PP_IIF(p##(42), BOOST_PP_NODE_41, BOOST_PP_NODE_43)
#                            define BOOST_PP_NODE_41(p) BOOST_PP_IIF(p##(41), 41, 42)
#                            define BOOST_PP_NODE_43(p) BOOST_PP_IIF(p##(43), 43, 44)
#                        define BOOST_PP_NODE_46(p) BOOST_PP_IIF(p##(46), BOOST_PP_NODE_45, BOOST_PP_NODE_47)
#                            define BOOST_PP_NODE_45(p) BOOST_PP_IIF(p##(45), 45, 46)
#                            define BOOST_PP_NODE_47(p) BOOST_PP_IIF(p##(47), 47, 48)
#                define BOOST_PP_NODE_56(p) BOOST_PP_IIF(p##(56), BOOST_PP_NODE_52, BOOST_PP_NODE_60)
#                    define BOOST_PP_NODE_52(p) BOOST_PP_IIF(p##(52), BOOST_PP_NODE_50, BOOST_PP_NODE_54)
#                        define BOOST_PP_NODE_50(p) BOOST_PP_IIF(p##(50), BOOST_PP_NODE_49, BOOST_PP_NODE_51)
#                            define BOOST_PP_NODE_49(p) BOOST_PP_IIF(p##(49), 49, 50)
#                            define BOOST_PP_NODE_51(p) BOOST_PP_IIF(p##(51), 51, 52)
#                        define BOOST_PP_NODE_54(p) BOOST_PP_IIF(p##(54), BOOST_PP_NODE_53, BOOST_PP_NODE_55)
#                            define BOOST_PP_NODE_53(p) BOOST_PP_IIF(p##(53), 53, 54)
#                            define BOOST_PP_NODE_55(p) BOOST_PP_IIF(p##(55), 55, 56)
#                    define BOOST_PP_NODE_60(p) BOOST_PP_IIF(p##(60), BOOST_PP_NODE_58, BOOST_PP_NODE_62)
#                        define BOOST_PP_NODE_58(p) BOOST_PP_IIF(p##(58), BOOST_PP_NODE_57, BOOST_PP_NODE_59)
#                            define BOOST_PP_NODE_57(p) BOOST_PP_IIF(p##(57), 57, 58)
#                            define BOOST_PP_NODE_59(p) BOOST_PP_IIF(p##(59), 59, 60)
#                        define BOOST_PP_NODE_62(p) BOOST_PP_IIF(p##(62), BOOST_PP_NODE_61, BOOST_PP_NODE_63)
#                            define BOOST_PP_NODE_61(p) BOOST_PP_IIF(p##(61), 61, 62)
#                            define BOOST_PP_NODE_63(p) BOOST_PP_IIF(p##(63), 63, 64)
#        define BOOST_PP_NODE_96(p) BOOST_PP_IIF(p##(96), BOOST_PP_NODE_80, BOOST_PP_NODE_112)
#            define BOOST_PP_NODE_80(p) BOOST_PP_IIF(p##(80), BOOST_PP_NODE_72, BOOST_PP_NODE_88)
#                define BOOST_PP_NODE_72(p) BOOST_PP_IIF(p##(72), BOOST_PP_NODE_68, BOOST_PP_NODE_76)
#                    define BOOST_PP_NODE_68(p) BOOST_PP_IIF(p##(68), BOOST_PP_NODE_66, BOOST_PP_NODE_70)
#                        define BOOST_PP_NODE_66(p) BOOST_PP_IIF(p##(66), BOOST_PP_NODE_65, BOOST_PP_NODE_67)
#                            define BOOST_PP_NODE_65(p) BOOST_PP_IIF(p##(65), 65, 66)
#                            define BOOST_PP_NODE_67(p) BOOST_PP_IIF(p##(67), 67, 68)
#                        define BOOST_PP_NODE_70(p) BOOST_PP_IIF(p##(70), BOOST_PP_NODE_69, BOOST_PP_NODE_71)
#                            define BOOST_PP_NODE_69(p) BOOST_PP_IIF(p##(69), 69, 70)
#                            define BOOST_PP_NODE_71(p) BOOST_PP_IIF(p##(71), 71, 72)
#                    define BOOST_PP_NODE_76(p) BOOST_PP_IIF(p##(76), BOOST_PP_NODE_74, BOOST_PP_NODE_78)
#                        define BOOST_PP_NODE_74(p) BOOST_PP_IIF(p##(74), BOOST_PP_NODE_73, BOOST_PP_NODE_75)
#                            define BOOST_PP_NODE_73(p) BOOST_PP_IIF(p##(73), 73, 74)
#                            define BOOST_PP_NODE_75(p) BOOST_PP_IIF(p##(75), 75, 76)
#                        define BOOST_PP_NODE_78(p) BOOST_PP_IIF(p##(78), BOOST_PP_NODE_77, BOOST_PP_NODE_79)
#                            define BOOST_PP_NODE_77(p) BOOST_PP_IIF(p##(77), 77, 78)
#                            define BOOST_PP_NODE_79(p) BOOST_PP_IIF(p##(79), 79, 80)
#                define BOOST_PP_NODE_88(p) BOOST_PP_IIF(p##(88), BOOST_PP_NODE_84, BOOST_PP_NODE_92)
#                    define BOOST_PP_NODE_84(p) BOOST_PP_IIF(p##(84), BOOST_PP_NODE_82, BOOST_PP_NODE_86)
#                        define BOOST_PP_NODE_82(p) BOOST_PP_IIF(p##(82), BOOST_PP_NODE_81, BOOST_PP_NODE_83)
#                            define BOOST_PP_NODE_81(p) BOOST_PP_IIF(p##(81), 81, 82)
#                            define BOOST_PP_NODE_83(p) BOOST_PP_IIF(p##(83), 83, 84)
#                        define BOOST_PP_NODE_86(p) BOOST_PP_IIF(p##(86), BOOST_PP_NODE_85, BOOST_PP_NODE_87)
#                            define BOOST_PP_NODE_85(p) BOOST_PP_IIF(p##(85), 85, 86)
#                            define BOOST_PP_NODE_87(p) BOOST_PP_IIF(p##(87), 87, 88)
#                    define BOOST_PP_NODE_92(p) BOOST_PP_IIF(p##(92), BOOST_PP_NODE_90, BOOST_PP_NODE_94)
#                        define BOOST_PP_NODE_90(p) BOOST_PP_IIF(p##(90), BOOST_PP_NODE_89, BOOST_PP_NODE_91)
#                            define BOOST_PP_NODE_89(p) BOOST_PP_IIF(p##(89), 89, 90)
#                            define BOOST_PP_NODE_91(p) BOOST_PP_IIF(p##(91), 91, 92)
#                        define BOOST_PP_NODE_94(p) BOOST_PP_IIF(p##(94), BOOST_PP_NODE_93, BOOST_PP_NODE_95)
#                            define BOOST_PP_NODE_93(p) BOOST_PP_IIF(p##(93), 93, 94)
#                            define BOOST_PP_NODE_95(p) BOOST_PP_IIF(p##(95), 95, 96)
#            define BOOST_PP_NODE_112(p) BOOST_PP_IIF(p##(112), BOOST_PP_NODE_104, BOOST_PP_NODE_120)
#                define BOOST_PP_NODE_104(p) BOOST_PP_IIF(p##(104), BOOST_PP_NODE_100, BOOST_PP_NODE_108)
#                    define BOOST_PP_NODE_100(p) BOOST_PP_IIF(p##(100), BOOST_PP_NODE_98, BOOST_PP_NODE_102)
#                        define BOOST_PP_NODE_98(p) BOOST_PP_IIF(p##(98), BOOST_PP_NODE_97, BOOST_PP_NODE_99)
#                            define BOOST_PP_NODE_97(p) BOOST_PP_IIF(p##(97), 97, 98)
#                            define BOOST_PP_NODE_99(p) BOOST_PP_IIF(p##(99), 99, 100)
#                        define BOOST_PP_NODE_102(p) BOOST_PP_IIF(p##(102), BOOST_PP_NODE_101, BOOST_PP_NODE_103)
#                            define BOOST_PP_NODE_101(p) BOOST_PP_IIF(p##(101), 101, 102)
#                            define BOOST_PP_NODE_103(p) BOOST_PP_IIF(p##(103), 103, 104)
#                    define BOOST_PP_NODE_108(p) BOOST_PP_IIF(p##(108), BOOST_PP_NODE_106, BOOST_PP_NODE_110)
#                        define BOOST_PP_NODE_106(p) BOOST_PP_IIF(p##(106), BOOST_PP_NODE_105, BOOST_PP_NODE_107)
#                            define BOOST_PP_NODE_105(p) BOOST_PP_IIF(p##(105), 105, 106)
#                            define BOOST_PP_NODE_107(p) BOOST_PP_IIF(p##(107), 107, 108)
#                        define BOOST_PP_NODE_110(p) BOOST_PP_IIF(p##(110), BOOST_PP_NODE_109, BOOST_PP_NODE_111)
#                            define BOOST_PP_NODE_109(p) BOOST_PP_IIF(p##(109), 109, 110)
#                            define BOOST_PP_NODE_111(p) BOOST_PP_IIF(p##(111), 111, 112)
#                define BOOST_PP_NODE_120(p) BOOST_PP_IIF(p##(120), BOOST_PP_NODE_116, BOOST_PP_NODE_124)
#                    define BOOST_PP_NODE_116(p) BOOST_PP_IIF(p##(116), BOOST_PP_NODE_114, BOOST_PP_NODE_118)
#                        define BOOST_PP_NODE_114(p) BOOST_PP_IIF(p##(114), BOOST_PP_NODE_113, BOOST_PP_NODE_115)
#                            define BOOST_PP_NODE_113(p) BOOST_PP_IIF(p##(113), 113, 114)
#                            define BOOST_PP_NODE_115(p) BOOST_PP_IIF(p##(115), 115, 116)
#                        define BOOST_PP_NODE_118(p) BOOST_PP_IIF(p##(118), BOOST_PP_NODE_117, BOOST_PP_NODE_119)
#                            define BOOST_PP_NODE_117(p) BOOST_PP_IIF(p##(117), 117, 118)
#                            define BOOST_PP_NODE_119(p) BOOST_PP_IIF(p##(119), 119, 120)
#                    define BOOST_PP_NODE_124(p) BOOST_PP_IIF(p##(124), BOOST_PP_NODE_122, BOOST_PP_NODE_126)
#                        define BOOST_PP_NODE_122(p) BOOST_PP_IIF(p##(122), BOOST_PP_NODE_121, BOOST_PP_NODE_123)
#                            define BOOST_PP_NODE_121(p) BOOST_PP_IIF(p##(121), 121, 122)
#                            define BOOST_PP_NODE_123(p) BOOST_PP_IIF(p##(123), 123, 124)
#                        define BOOST_PP_NODE_126(p) BOOST_PP_IIF(p##(126), BOOST_PP_NODE_125, BOOST_PP_NODE_127)
#                            define BOOST_PP_NODE_125(p) BOOST_PP_IIF(p##(125), 125, 126)
#                            define BOOST_PP_NODE_127(p) BOOST_PP_IIF(p##(127), 127, 128)
#    define BOOST_PP_NODE_192(p) BOOST_PP_IIF(p##(192), BOOST_PP_NODE_160, BOOST_PP_NODE_224)
#        define BOOST_PP_NODE_160(p) BOOST_PP_IIF(p##(160), BOOST_PP_NODE_144, BOOST_PP_NODE_176)
#            define BOOST_PP_NODE_144(p) BOOST_PP_IIF(p##(144), BOOST_PP_NODE_136, BOOST_PP_NODE_152)
#                define BOOST_PP_NODE_136(p) BOOST_PP_IIF(p##(136), BOOST_PP_NODE_132, BOOST_PP_NODE_140)
#                    define BOOST_PP_NODE_132(p) BOOST_PP_IIF(p##(132), BOOST_PP_NODE_130, BOOST_PP_NODE_134)
#                        define BOOST_PP_NODE_130(p) BOOST_PP_IIF(p##(130), BOOST_PP_NODE_129, BOOST_PP_NODE_131)
#                            define BOOST_PP_NODE_129(p) BOOST_PP_IIF(p##(129), 129, 130)
#                            define BOOST_PP_NODE_131(p) BOOST_PP_IIF(p##(131), 131, 132)
#                        define BOOST_PP_NODE_134(p) BOOST_PP_IIF(p##(134), BOOST_PP_NODE_133, BOOST_PP_NODE_135)
#                            define BOOST_PP_NODE_133(p) BOOST_PP_IIF(p##(133), 133, 134)
#                            define BOOST_PP_NODE_135(p) BOOST_PP_IIF(p##(135), 135, 136)
#                    define BOOST_PP_NODE_140(p) BOOST_PP_IIF(p##(140), BOOST_PP_NODE_138, BOOST_PP_NODE_142)
#                        define BOOST_PP_NODE_138(p) BOOST_PP_IIF(p##(138), BOOST_PP_NODE_137, BOOST_PP_NODE_139)
#                            define BOOST_PP_NODE_137(p) BOOST_PP_IIF(p##(137), 137, 138)
#                            define BOOST_PP_NODE_139(p) BOOST_PP_IIF(p##(139), 139, 140)
#                        define BOOST_PP_NODE_142(p) BOOST_PP_IIF(p##(142), BOOST_PP_NODE_141, BOOST_PP_NODE_143)
#                            define BOOST_PP_NODE_141(p) BOOST_PP_IIF(p##(141), 141, 142)
#                            define BOOST_PP_NODE_143(p) BOOST_PP_IIF(p##(143), 143, 144)
#                define BOOST_PP_NODE_152(p) BOOST_PP_IIF(p##(152), BOOST_PP_NODE_148, BOOST_PP_NODE_156)
#                    define BOOST_PP_NODE_148(p) BOOST_PP_IIF(p##(148), BOOST_PP_NODE_146, BOOST_PP_NODE_150)
#                        define BOOST_PP_NODE_146(p) BOOST_PP_IIF(p##(146), BOOST_PP_NODE_145, BOOST_PP_NODE_147)
#                            define BOOST_PP_NODE_145(p) BOOST_PP_IIF(p##(145), 145, 146)
#                            define BOOST_PP_NODE_147(p) BOOST_PP_IIF(p##(147), 147, 148)
#                        define BOOST_PP_NODE_150(p) BOOST_PP_IIF(p##(150), BOOST_PP_NODE_149, BOOST_PP_NODE_151)
#                            define BOOST_PP_NODE_149(p) BOOST_PP_IIF(p##(149), 149, 150)
#                            define BOOST_PP_NODE_151(p) BOOST_PP_IIF(p##(151), 151, 152)
#                    define BOOST_PP_NODE_156(p) BOOST_PP_IIF(p##(156), BOOST_PP_NODE_154, BOOST_PP_NODE_158)
#                        define BOOST_PP_NODE_154(p) BOOST_PP_IIF(p##(154), BOOST_PP_NODE_153, BOOST_PP_NODE_155)
#                            define BOOST_PP_NODE_153(p) BOOST_PP_IIF(p##(153), 153, 154)
#                            define BOOST_PP_NODE_155(p) BOOST_PP_IIF(p##(155), 155, 156)
#                        define BOOST_PP_NODE_158(p) BOOST_PP_IIF(p##(158), BOOST_PP_NODE_157, BOOST_PP_NODE_159)
#                            define BOOST_PP_NODE_157(p) BOOST_PP_IIF(p##(157), 157, 158)
#                            define BOOST_PP_NODE_159(p) BOOST_PP_IIF(p##(159), 159, 160)
#            define BOOST_PP_NODE_176(p) BOOST_PP_IIF(p##(176), BOOST_PP_NODE_168, BOOST_PP_NODE_184)
#                define BOOST_PP_NODE_168(p) BOOST_PP_IIF(p##(168), BOOST_PP_NODE_164, BOOST_PP_NODE_172)
#                    define BOOST_PP_NODE_164(p) BOOST_PP_IIF(p##(164), BOOST_PP_NODE_162, BOOST_PP_NODE_166)
#                        define BOOST_PP_NODE_162(p) BOOST_PP_IIF(p##(162), BOOST_PP_NODE_161, BOOST_PP_NODE_163)
#                            define BOOST_PP_NODE_161(p) BOOST_PP_IIF(p##(161), 161, 162)
#                            define BOOST_PP_NODE_163(p) BOOST_PP_IIF(p##(163), 163, 164)
#                        define BOOST_PP_NODE_166(p) BOOST_PP_IIF(p##(166), BOOST_PP_NODE_165, BOOST_PP_NODE_167)
#                            define BOOST_PP_NODE_165(p) BOOST_PP_IIF(p##(165), 165, 166)
#                            define BOOST_PP_NODE_167(p) BOOST_PP_IIF(p##(167), 167, 168)
#                    define BOOST_PP_NODE_172(p) BOOST_PP_IIF(p##(172), BOOST_PP_NODE_170, BOOST_PP_NODE_174)
#                        define BOOST_PP_NODE_170(p) BOOST_PP_IIF(p##(170), BOOST_PP_NODE_169, BOOST_PP_NODE_171)
#                            define BOOST_PP_NODE_169(p) BOOST_PP_IIF(p##(169), 169, 170)
#                            define BOOST_PP_NODE_171(p) BOOST_PP_IIF(p##(171), 171, 172)
#                        define BOOST_PP_NODE_174(p) BOOST_PP_IIF(p##(174), BOOST_PP_NODE_173, BOOST_PP_NODE_175)
#                            define BOOST_PP_NODE_173(p) BOOST_PP_IIF(p##(173), 173, 174)
#                            define BOOST_PP_NODE_175(p) BOOST_PP_IIF(p##(175), 175, 176)
#                define BOOST_PP_NODE_184(p) BOOST_PP_IIF(p##(184), BOOST_PP_NODE_180, BOOST_PP_NODE_188)
#                    define BOOST_PP_NODE_180(p) BOOST_PP_IIF(p##(180), BOOST_PP_NODE_178, BOOST_PP_NODE_182)
#                        define BOOST_PP_NODE_178(p) BOOST_PP_IIF(p##(178), BOOST_PP_NODE_177, BOOST_PP_NODE_179)
#                            define BOOST_PP_NODE_177(p) BOOST_PP_IIF(p##(177), 177, 178)
#                            define BOOST_PP_NODE_179(p) BOOST_PP_IIF(p##(179), 179, 180)
#                        define BOOST_PP_NODE_182(p) BOOST_PP_IIF(p##(182), BOOST_PP_NODE_181, BOOST_PP_NODE_183)
#                            define BOOST_PP_NODE_181(p) BOOST_PP_IIF(p##(181), 181, 182)
#                            define BOOST_PP_NODE_183(p) BOOST_PP_IIF(p##(183), 183, 184)
#                    define BOOST_PP_NODE_188(p) BOOST_PP_IIF(p##(188), BOOST_PP_NODE_186, BOOST_PP_NODE_190)
#                        define BOOST_PP_NODE_186(p) BOOST_PP_IIF(p##(186), BOOST_PP_NODE_185, BOOST_PP_NODE_187)
#                            define BOOST_PP_NODE_185(p) BOOST_PP_IIF(p##(185), 185, 186)
#                            define BOOST_PP_NODE_187(p) BOOST_PP_IIF(p##(187), 187, 188)
#                        define BOOST_PP_NODE_190(p) BOOST_PP_IIF(p##(190), BOOST_PP_NODE_189, BOOST_PP_NODE_191)
#                            define BOOST_PP_NODE_189(p) BOOST_PP_IIF(p##(189), 189, 190)
#                            define BOOST_PP_NODE_191(p) BOOST_PP_IIF(p##(191), 191, 192)
#        define BOOST_PP_NODE_224(p) BOOST_PP_IIF(p##(224), BOOST_PP_NODE_208, BOOST_PP_NODE_240)
#            define BOOST_PP_NODE_208(p) BOOST_PP_IIF(p##(208), BOOST_PP_NODE_200, BOOST_PP_NODE_216)
#                define BOOST_PP_NODE_200(p) BOOST_PP_IIF(p##(200), BOOST_PP_NODE_196, BOOST_PP_NODE_204)
#                    define BOOST_PP_NODE_196(p) BOOST_PP_IIF(p##(196), BOOST_PP_NODE_194, BOOST_PP_NODE_198)
#                        define BOOST_PP_NODE_194(p) BOOST_PP_IIF(p##(194), BOOST_PP_NODE_193, BOOST_PP_NODE_195)
#                            define BOOST_PP_NODE_193(p) BOOST_PP_IIF(p##(193), 193, 194)
#                            define BOOST_PP_NODE_195(p) BOOST_PP_IIF(p##(195), 195, 196)
#                        define BOOST_PP_NODE_198(p) BOOST_PP_IIF(p##(198), BOOST_PP_NODE_197, BOOST_PP_NODE_199)
#                            define BOOST_PP_NODE_197(p) BOOST_PP_IIF(p##(197), 197, 198)
#                            define BOOST_PP_NODE_199(p) BOOST_PP_IIF(p##(199), 199, 200)
#                    define BOOST_PP_NODE_204(p) BOOST_PP_IIF(p##(204), BOOST_PP_NODE_202, BOOST_PP_NODE_206)
#                        define BOOST_PP_NODE_202(p) BOOST_PP_IIF(p##(202), BOOST_PP_NODE_201, BOOST_PP_NODE_203)
#                            define BOOST_PP_NODE_201(p) BOOST_PP_IIF(p##(201), 201, 202)
#                            define BOOST_PP_NODE_203(p) BOOST_PP_IIF(p##(203), 203, 204)
#                        define BOOST_PP_NODE_206(p) BOOST_PP_IIF(p##(206), BOOST_PP_NODE_205, BOOST_PP_NODE_207)
#                            define BOOST_PP_NODE_205(p) BOOST_PP_IIF(p##(205), 205, 206)
#                            define BOOST_PP_NODE_207(p) BOOST_PP_IIF(p##(207), 207, 208)
#                define BOOST_PP_NODE_216(p) BOOST_PP_IIF(p##(216), BOOST_PP_NODE_212, BOOST_PP_NODE_220)
#                    define BOOST_PP_NODE_212(p) BOOST_PP_IIF(p##(212), BOOST_PP_NODE_210, BOOST_PP_NODE_214)
#                        define BOOST_PP_NODE_210(p) BOOST_PP_IIF(p##(210), BOOST_PP_NODE_209, BOOST_PP_NODE_211)
#                            define BOOST_PP_NODE_209(p) BOOST_PP_IIF(p##(209), 209, 210)
#                            define BOOST_PP_NODE_211(p) BOOST_PP_IIF(p##(211), 211, 212)
#                        define BOOST_PP_NODE_214(p) BOOST_PP_IIF(p##(214), BOOST_PP_NODE_213, BOOST_PP_NODE_215)
#                            define BOOST_PP_NODE_213(p) BOOST_PP_IIF(p##(213), 213, 214)
#                            define BOOST_PP_NODE_215(p) BOOST_PP_IIF(p##(215), 215, 216)
#                    define BOOST_PP_NODE_220(p) BOOST_PP_IIF(p##(220), BOOST_PP_NODE_218, BOOST_PP_NODE_222)
#                        define BOOST_PP_NODE_218(p) BOOST_PP_IIF(p##(218), BOOST_PP_NODE_217, BOOST_PP_NODE_219)
#                            define BOOST_PP_NODE_217(p) BOOST_PP_IIF(p##(217), 217, 218)
#                            define BOOST_PP_NODE_219(p) BOOST_PP_IIF(p##(219), 219, 220)
#                        define BOOST_PP_NODE_222(p) BOOST_PP_IIF(p##(222), BOOST_PP_NODE_221, BOOST_PP_NODE_223)
#                            define BOOST_PP_NODE_221(p) BOOST_PP_IIF(p##(221), 221, 222)
#                            define BOOST_PP_NODE_223(p) BOOST_PP_IIF(p##(223), 223, 224)
#            define BOOST_PP_NODE_240(p) BOOST_PP_IIF(p##(240), BOOST_PP_NODE_232, BOOST_PP_NODE_248)
#                define BOOST_PP_NODE_232(p) BOOST_PP_IIF(p##(232), BOOST_PP_NODE_228, BOOST_PP_NODE_236)
#                    define BOOST_PP_NODE_228(p) BOOST_PP_IIF(p##(228), BOOST_PP_NODE_226, BOOST_PP_NODE_230)
#                        define BOOST_PP_NODE_226(p) BOOST_PP_IIF(p##(226), BOOST_PP_NODE_225, BOOST_PP_NODE_227)
#                            define BOOST_PP_NODE_225(p) BOOST_PP_IIF(p##(225), 225, 226)
#                            define BOOST_PP_NODE_227(p) BOOST_PP_IIF(p##(227), 227, 228)
#                        define BOOST_PP_NODE_230(p) BOOST_PP_IIF(p##(230), BOOST_PP_NODE_229, BOOST_PP_NODE_231)
#                            define BOOST_PP_NODE_229(p) BOOST_PP_IIF(p##(229), 229, 230)
#                            define BOOST_PP_NODE_231(p) BOOST_PP_IIF(p##(231), 231, 232)
#                    define BOOST_PP_NODE_236(p) BOOST_PP_IIF(p##(236), BOOST_PP_NODE_234, BOOST_PP_NODE_238)
#                        define BOOST_PP_NODE_234(p) BOOST_PP_IIF(p##(234), BOOST_PP_NODE_233, BOOST_PP_NODE_235)
#                            define BOOST_PP_NODE_233(p) BOOST_PP_IIF(p##(233), 233, 234)
#                            define BOOST_PP_NODE_235(p) BOOST_PP_IIF(p##(235), 235, 236)
#                        define BOOST_PP_NODE_238(p) BOOST_PP_IIF(p##(238), BOOST_PP_NODE_237, BOOST_PP_NODE_239)
#                            define BOOST_PP_NODE_237(p) BOOST_PP_IIF(p##(237), 237, 238)
#                            define BOOST_PP_NODE_239(p) BOOST_PP_IIF(p##(239), 239, 240)
#                define BOOST_PP_NODE_248(p) BOOST_PP_IIF(p##(248), BOOST_PP_NODE_244, BOOST_PP_NODE_252)
#                    define BOOST_PP_NODE_244(p) BOOST_PP_IIF(p##(244), BOOST_PP_NODE_242, BOOST_PP_NODE_246)
#                        define BOOST_PP_NODE_242(p) BOOST_PP_IIF(p##(242), BOOST_PP_NODE_241, BOOST_PP_NODE_243)
#                            define BOOST_PP_NODE_241(p) BOOST_PP_IIF(p##(241), 241, 242)
#                            define BOOST_PP_NODE_243(p) BOOST_PP_IIF(p##(243), 243, 244)
#                        define BOOST_PP_NODE_246(p) BOOST_PP_IIF(p##(246), BOOST_PP_NODE_245, BOOST_PP_NODE_247)
#                            define BOOST_PP_NODE_245(p) BOOST_PP_IIF(p##(245), 245, 246)
#                            define BOOST_PP_NODE_247(p) BOOST_PP_IIF(p##(247), 247, 248)
#                    define BOOST_PP_NODE_252(p) BOOST_PP_IIF(p##(252), BOOST_PP_NODE_250, BOOST_PP_NODE_254)
#                        define BOOST_PP_NODE_250(p) BOOST_PP_IIF(p##(250), BOOST_PP_NODE_249, BOOST_PP_NODE_251)
#                            define BOOST_PP_NODE_249(p) BOOST_PP_IIF(p##(249), 249, 250)
#                            define BOOST_PP_NODE_251(p) BOOST_PP_IIF(p##(251), 251, 252)
#                        define BOOST_PP_NODE_254(p) BOOST_PP_IIF(p##(254), BOOST_PP_NODE_253, BOOST_PP_NODE_255)
#                            define BOOST_PP_NODE_253(p) BOOST_PP_IIF(p##(253), 253, 254)
#                            define BOOST_PP_NODE_255(p) BOOST_PP_IIF(p##(255), 255, 256)

//refer to:preprocessor/repetition/for.hpp


# define BOOST_PP_FOR BOOST_PP_CAT(BOOST_PP_FOR_, BOOST_PP_AUTO_REC(BOOST_PP_FOR_P, 256))
# define BOOST_PP_FOR_P(n) BOOST_PP_CAT(BOOST_PP_FOR_CHECK_, BOOST_PP_FOR_ ## n(1, BOOST_PP_FOR_SR_P, BOOST_PP_FOR_SR_O, BOOST_PP_FOR_SR_M))


# define BOOST_PP_FOR_SR_P(r, s) s
# define BOOST_PP_FOR_SR_O(r, s) 0
# define BOOST_PP_FOR_SR_M(r, s) BOOST_PP_NIL

# define BOOST_PP_FOR_CHECK_BOOST_PP_NIL 1
#
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_1(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_2(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_3(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_4(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_5(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_6(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_7(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_8(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_9(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_10(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_11(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_12(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_13(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_14(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_15(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_16(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_17(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_18(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_19(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_20(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_21(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_22(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_23(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_24(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_25(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_26(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_27(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_28(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_29(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_30(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_31(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_32(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_33(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_34(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_35(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_36(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_37(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_38(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_39(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_40(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_41(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_42(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_43(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_44(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_45(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_46(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_47(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_48(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_49(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_50(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_51(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_52(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_53(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_54(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_55(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_56(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_57(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_58(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_59(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_60(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_61(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_62(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_63(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_64(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_65(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_66(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_67(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_68(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_69(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_70(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_71(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_72(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_73(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_74(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_75(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_76(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_77(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_78(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_79(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_80(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_81(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_82(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_83(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_84(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_85(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_86(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_87(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_88(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_89(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_90(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_91(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_92(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_93(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_94(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_95(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_96(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_97(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_98(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_99(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_100(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_101(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_102(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_103(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_104(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_105(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_106(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_107(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_108(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_109(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_110(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_111(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_112(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_113(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_114(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_115(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_116(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_117(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_118(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_119(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_120(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_121(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_122(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_123(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_124(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_125(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_126(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_127(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_128(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_129(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_130(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_131(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_132(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_133(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_134(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_135(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_136(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_137(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_138(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_139(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_140(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_141(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_142(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_143(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_144(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_145(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_146(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_147(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_148(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_149(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_150(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_151(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_152(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_153(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_154(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_155(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_156(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_157(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_158(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_159(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_160(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_161(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_162(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_163(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_164(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_165(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_166(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_167(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_168(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_169(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_170(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_171(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_172(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_173(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_174(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_175(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_176(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_177(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_178(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_179(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_180(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_181(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_182(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_183(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_184(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_185(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_186(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_187(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_188(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_189(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_190(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_191(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_192(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_193(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_194(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_195(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_196(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_197(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_198(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_199(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_200(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_201(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_202(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_203(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_204(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_205(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_206(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_207(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_208(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_209(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_210(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_211(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_212(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_213(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_214(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_215(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_216(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_217(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_218(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_219(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_220(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_221(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_222(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_223(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_224(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_225(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_226(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_227(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_228(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_229(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_230(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_231(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_232(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_233(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_234(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_235(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_236(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_237(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_238(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_239(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_240(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_241(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_242(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_243(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_244(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_245(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_246(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_247(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_248(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_249(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_250(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_251(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_252(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_253(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_254(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_255(s, p, o, m) 0
# define BOOST_PP_FOR_CHECK_BOOST_PP_FOR_256(s, p, o, m) 0

//refer to:\preprocessor\cat.hpp
#define BOOST_PP_CAT(a, b) BOOST_PP_CAT_I(a, b)
#define BOOST_PP_CAT_I(a, b) a ## b


//refer to:\preprocessor\list\adt.hpp
#    define BOOST_PP_LIST_IS_CONS(list) BOOST_PP_IS_BINARY(list)

# define BOOST_PP_LIST_REST(list) BOOST_PP_LIST_REST_D(list)
#    define BOOST_PP_LIST_REST_D(list) BOOST_PP_LIST_REST_I ## list
# define BOOST_PP_LIST_REST_I(head, tail) tail

# define BOOST_PP_LIST_FIRST(list) BOOST_PP_LIST_FIRST_D(list)
#    define BOOST_PP_LIST_FIRST_D(list) BOOST_PP_LIST_FIRST_I ## list
# define BOOST_PP_LIST_FIRST_I(head, tail) head

//refer to:preprocessor\detail\is_binary.hpp
#    define BOOST_PP_IS_BINARY(x) BOOST_PP_CHECK(x, BOOST_PP_IS_BINARY_CHECK)

//The BOOST_PP_NIL identifier is reserved by the library to indicate a non-macro.
//This identifier is not defined.  Its purpose is to not be a macro.  It is also used to terminate a list.

# define BOOST_PP_IS_BINARY_CHECK(a, b) 1
# define BOOST_PP_CHECK_RESULT_BOOST_PP_IS_BINARY_CHECK 0, BOOST_PP_NIL

//refer to:preprocessor\detail\check.hpp
#    define BOOST_PP_CHECK(x, type) BOOST_PP_CHECK_D(x, type)

# define BOOST_PP_CHECK_RESULT_1 1, BOOST_PP_NIL

#    define BOOST_PP_CHECK_D(x, type) BOOST_PP_CHECK_1(BOOST_PP_CAT(BOOST_PP_CHECK_RESULT_, type x))
#    define BOOST_PP_CHECK_1(chk) BOOST_PP_CHECK_2(chk)
#    define BOOST_PP_CHECK_2(res, _) res

//refer to:preprocessor\variadic\size.hpp
#        define BOOST_PP_VARIADIC_SIZE(...) BOOST_PP_CAT(BOOST_PP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,),)
#    define BOOST_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size

//refer to:\preprocessor\facilities\overload.hpp
#    define BOOST_PP_OVERLOAD(prefix, ...) BOOST_PP_CAT(prefix, BOOST_PP_VARIADIC_SIZE(__VA_ARGS__))



//refer to:preprocessor\tuple\elem.hpp
#        define BOOST_PP_VARIADIC_ELEM(n, ...) BOOST_PP_VARIADIC_ELEM_I(n,__VA_ARGS__)
#        define BOOST_PP_VARIADIC_ELEM_I(n, ...) BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_VARIADIC_ELEM_, n)(__VA_ARGS__,),)

#    define BOOST_PP_VARIADIC_ELEM_0(e0, ...) e0
#    define BOOST_PP_VARIADIC_ELEM_1(e0, e1, ...) e1
#    define BOOST_PP_VARIADIC_ELEM_2(e0, e1, e2, ...) e2
#    define BOOST_PP_VARIADIC_ELEM_3(e0, e1, e2, e3, ...) e3
#    define BOOST_PP_VARIADIC_ELEM_4(e0, e1, e2, e3, e4, ...) e4
#    define BOOST_PP_VARIADIC_ELEM_5(e0, e1, e2, e3, e4, e5, ...) e5
#    define BOOST_PP_VARIADIC_ELEM_6(e0, e1, e2, e3, e4, e5, e6, ...) e6
#    define BOOST_PP_VARIADIC_ELEM_7(e0, e1, e2, e3, e4, e5, e6, e7, ...) e7
#    define BOOST_PP_VARIADIC_ELEM_8(e0, e1, e2, e3, e4, e5, e6, e7, e8, ...) e8
#    define BOOST_PP_VARIADIC_ELEM_9(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, ...) e9
#    define BOOST_PP_VARIADIC_ELEM_10(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, ...) e10
#    define BOOST_PP_VARIADIC_ELEM_11(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, ...) e11
#    define BOOST_PP_VARIADIC_ELEM_12(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, ...) e12
#    define BOOST_PP_VARIADIC_ELEM_13(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, ...) e13
#    define BOOST_PP_VARIADIC_ELEM_14(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, ...) e14
#    define BOOST_PP_VARIADIC_ELEM_15(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, ...) e15
#    define BOOST_PP_VARIADIC_ELEM_16(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, ...) e16
#    define BOOST_PP_VARIADIC_ELEM_17(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, ...) e17
#    define BOOST_PP_VARIADIC_ELEM_18(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, ...) e18
#    define BOOST_PP_VARIADIC_ELEM_19(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, ...) e19
#    define BOOST_PP_VARIADIC_ELEM_20(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, ...) e20
#    define BOOST_PP_VARIADIC_ELEM_21(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, ...) e21
#    define BOOST_PP_VARIADIC_ELEM_22(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, ...) e22
#    define BOOST_PP_VARIADIC_ELEM_23(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, ...) e23
#    define BOOST_PP_VARIADIC_ELEM_24(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, ...) e24
#    define BOOST_PP_VARIADIC_ELEM_25(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, ...) e25
#    define BOOST_PP_VARIADIC_ELEM_26(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, ...) e26
#    define BOOST_PP_VARIADIC_ELEM_27(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, ...) e27
#    define BOOST_PP_VARIADIC_ELEM_28(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, ...) e28
#    define BOOST_PP_VARIADIC_ELEM_29(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, ...) e29
#    define BOOST_PP_VARIADIC_ELEM_30(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, ...) e30
#    define BOOST_PP_VARIADIC_ELEM_31(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, ...) e31
#    define BOOST_PP_VARIADIC_ELEM_32(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, ...) e32
#    define BOOST_PP_VARIADIC_ELEM_33(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, ...) e33
#    define BOOST_PP_VARIADIC_ELEM_34(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, ...) e34
#    define BOOST_PP_VARIADIC_ELEM_35(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, ...) e35
#    define BOOST_PP_VARIADIC_ELEM_36(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, ...) e36
#    define BOOST_PP_VARIADIC_ELEM_37(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, ...) e37
#    define BOOST_PP_VARIADIC_ELEM_38(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, ...) e38
#    define BOOST_PP_VARIADIC_ELEM_39(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, ...) e39
#    define BOOST_PP_VARIADIC_ELEM_40(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, ...) e40
#    define BOOST_PP_VARIADIC_ELEM_41(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, ...) e41
#    define BOOST_PP_VARIADIC_ELEM_42(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, ...) e42
#    define BOOST_PP_VARIADIC_ELEM_43(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, ...) e43
#    define BOOST_PP_VARIADIC_ELEM_44(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, ...) e44
#    define BOOST_PP_VARIADIC_ELEM_45(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, ...) e45
#    define BOOST_PP_VARIADIC_ELEM_46(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, ...) e46
#    define BOOST_PP_VARIADIC_ELEM_47(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, ...) e47
#    define BOOST_PP_VARIADIC_ELEM_48(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, ...) e48
#    define BOOST_PP_VARIADIC_ELEM_49(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, ...) e49
#    define BOOST_PP_VARIADIC_ELEM_50(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, ...) e50
#    define BOOST_PP_VARIADIC_ELEM_51(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, ...) e51
#    define BOOST_PP_VARIADIC_ELEM_52(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, ...) e52
#    define BOOST_PP_VARIADIC_ELEM_53(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, ...) e53
#    define BOOST_PP_VARIADIC_ELEM_54(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, ...) e54
#    define BOOST_PP_VARIADIC_ELEM_55(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, ...) e55
#    define BOOST_PP_VARIADIC_ELEM_56(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, ...) e56
#    define BOOST_PP_VARIADIC_ELEM_57(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, ...) e57
#    define BOOST_PP_VARIADIC_ELEM_58(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, ...) e58
#    define BOOST_PP_VARIADIC_ELEM_59(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, ...) e59
#    define BOOST_PP_VARIADIC_ELEM_60(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, ...) e60
#    define BOOST_PP_VARIADIC_ELEM_61(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, ...) e61
#    define BOOST_PP_VARIADIC_ELEM_62(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, ...) e62
#    define BOOST_PP_VARIADIC_ELEM_63(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, ...) e63

//refer to:\preprocessor\tuple\rem.hpp
# define BOOST_PP_TUPLE_REM_1(e0) e0
# define BOOST_PP_TUPLE_REM_2(e0, e1) e0, e1
# define BOOST_PP_TUPLE_REM_3(e0, e1, e2) e0, e1, e2
# define BOOST_PP_TUPLE_REM_4(e0, e1, e2, e3) e0, e1, e2, e3

#        define BOOST_PP_TUPLE_ELEM(...) BOOST_PP_TUPLE_ELEM_I(BOOST_PP_OVERLOAD(BOOST_PP_TUPLE_ELEM_O_, __VA_ARGS__), (__VA_ARGS__))
#        define BOOST_PP_TUPLE_ELEM_I(m, args) BOOST_PP_TUPLE_ELEM_II(m, args)
#        define BOOST_PP_TUPLE_ELEM_II(m, args) BOOST_PP_CAT(m ## args,)

#    define BOOST_PP_TUPLE_ELEM_O_2(n, tuple) BOOST_PP_VARIADIC_ELEM(n, BOOST_PP_REM tuple)
#    define BOOST_PP_TUPLE_ELEM_O_3(size, n, tuple) BOOST_PP_TUPLE_ELEM_O_2(n, tuple)

//refer to:preprocessor/list/for_each_i.hpp
#    define BOOST_PP_LIST_FOR_EACH_I(macro, data, list) BOOST_PP_LIST_FOR_EACH_I_I(macro, data, list)
#    define BOOST_PP_LIST_FOR_EACH_I_I(macro, data, list) BOOST_PP_FOR((macro, data, list, 0), BOOST_PP_LIST_FOR_EACH_I_P, BOOST_PP_LIST_FOR_EACH_I_O, BOOST_PP_LIST_FOR_EACH_I_M)

#    define BOOST_PP_LIST_FOR_EACH_I_P(r, x) BOOST_PP_LIST_IS_CONS(BOOST_PP_TUPLE_ELEM(4, 2, x))
#    define BOOST_PP_LIST_FOR_EACH_I_O(r, x) (BOOST_PP_TUPLE_ELEM(4, 0, x), BOOST_PP_TUPLE_ELEM(4, 1, x), BOOST_PP_LIST_REST(BOOST_PP_TUPLE_ELEM(4, 2, x)), BOOST_PP_INC(BOOST_PP_TUPLE_ELEM(4, 3, x)))

#    define BOOST_PP_LIST_FOR_EACH_I_M(r, x) BOOST_PP_LIST_FOR_EACH_I_M_I(r, BOOST_PP_TUPLE_REM_4 x)
#    define BOOST_PP_LIST_FOR_EACH_I_M_I(r, x_e) BOOST_PP_LIST_FOR_EACH_I_M_D(r, x_e)

# define BOOST_PP_LIST_FOR_EACH_I_M_D(r, m, d, l, i) m(r, d, i, BOOST_PP_LIST_FIRST(l))

#    define BOOST_PP_LIST_FOR_EACH_I_R(r, macro, data, list) BOOST_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list)
#    define BOOST_PP_LIST_FOR_EACH_I_R_I(r, macro, data, list) BOOST_PP_FOR_ ## r((macro, data, list, 0), BOOST_PP_LIST_FOR_EACH_I_P, BOOST_PP_LIST_FOR_EACH_I_O, BOOST_PP_LIST_FOR_EACH_I_M)

//refer to:\preprocessor\tuple\to_list.hpp
#        define BOOST_PP_TUPLE_TO_LIST(...) BOOST_PP_TUPLE_TO_LIST_I(BOOST_PP_OVERLOAD(BOOST_PP_TUPLE_TO_LIST_O_, __VA_ARGS__), (__VA_ARGS__))
#        define BOOST_PP_TUPLE_TO_LIST_I(m, args) BOOST_PP_TUPLE_TO_LIST_II(m, args)
#        define BOOST_PP_TUPLE_TO_LIST_II(m, args) BOOST_PP_CAT(m ## args,)

//refer to:mpl\aux_\preprocessor\params.hpp
#   define BOOST_MPL_PP_PARAMS(n,p) \
	BOOST_PP_CAT(BOOST_MPL_PP_PARAMS_,n)(p) \

#   define BOOST_MPL_PP_PARAMS_0(p)
#   define BOOST_MPL_PP_PARAMS_1(p) p##1
#   define BOOST_MPL_PP_PARAMS_2(p) p##1,p##2
#   define BOOST_MPL_PP_PARAMS_3(p) p##1,p##2,p##3
#   define BOOST_MPL_PP_PARAMS_4(p) p##1,p##2,p##3,p##4
#   define BOOST_MPL_PP_PARAMS_5(p) p##1,p##2,p##3,p##4,p##5
#   define BOOST_MPL_PP_PARAMS_6(p) p##1,p##2,p##3,p##4,p##5,p##6
#   define BOOST_MPL_PP_PARAMS_7(p) p##1,p##2,p##3,p##4,p##5,p##6,p##7
#   define BOOST_MPL_PP_PARAMS_8(p) p##1,p##2,p##3,p##4,p##5,p##6,p##7,p##8
#   define BOOST_MPL_PP_PARAMS_9(p) p##1,p##2,p##3,p##4,p##5,p##6,p##7,p##8,p##9

//refer to: mpl\aux_\lambda_support.hpp

#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_HAS_REBIND(i, name, params) \
	template< BOOST_MPL_PP_PARAMS(i,typename T) > \
	yes_tag operator|( \
	has_rebind_tag<int> \
	, has_rebind_tag< name<BOOST_MPL_PP_PARAMS(i,T)> >* \
	); \

//#   define BOOST_MPL_AUX_ADL_BARRIER_NAMESPACE mpl_
#   define BOOST_MPL_AUX_LAMBDA_SUPPORT_ARG_TYPEDEF_FUNC(R,typedef_,i,param) \
	typedef_ param BOOST_PP_CAT(arg,BOOST_PP_INC(i)); \

#define BOOST_MPL_AUX_LAMBDA_SUPPORT_SPEC(i, name, params) \
	typedef int_<i> arity; \
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

//refer to:container\detail\mpl.hpp
template <class T, T val>
struct integral_constant
{
	static const T value = val;
	typedef integral_constant<T,val> type;
};

//refer to:type_traits\detail\bool_trait_def.hpp
#define BOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(C) \
	typedef integral_constant<bool,C> base_; \
	using base_::value; \
	/**/


//refer to:type_traits\detail\template_arity_spec.hpp
#   define BOOST_TT_AUX_TEMPLATE_ARITY_SPEC(i, name) \
	template< BOOST_MPL_PP_PARAMS(i, typename T) > \
struct template_arity< \
	name< BOOST_MPL_PP_PARAMS(i, T) > \
> \
	: int_<i> \
{ \
}; \
	/**/

#define BOOST_TT_AUX_BOOL_C_BASE(C) : public integral_constant<bool,C>

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

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_member_pointer,T,is_member_pointer_impl<T>::value)

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