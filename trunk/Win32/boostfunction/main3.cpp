#include <stdio.h>
#include <stdlib.h>

//refer to:http://www.boost.org/doc/libs/1_49_0/boost/mpl/if.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/detail/sp_typeinfo.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/current_function.hpp
//refer to:https://github.com/mantidproject/3rdpartyincludes/blob/master/boost/ref.hpp

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

template<class T> class reference_wrapper
{ 
public:
	typedef T type;

#if defined( BOOST_MSVC ) && BOOST_WORKAROUND( BOOST_MSVC, < 1300 )

	explicit reference_wrapper(T& t): t_(&t) {}

#else

	explicit reference_wrapper(T& t): t_(boost::addressof(t)) {}

#endif

	operator T& () const { return *t_; }

	T& get() const { return *t_; }

	T* get_pointer() const { return t_; }

private:

	T* t_;
};

# if defined( __BORLANDC__ ) && BOOST_WORKAROUND( __BORLANDC__, BOOST_TESTED_AT(0x581) )
#  define BOOST_REF_CONST
# else
#  define BOOST_REF_CONST const
# endif

template<class T> inline reference_wrapper<T> BOOST_REF_CONST ref(T & t)
{ 
	return reference_wrapper<T>(t);
}

template<class T> inline reference_wrapper<T const> BOOST_REF_CONST cref(T const & t)
{
	return reference_wrapper<T const>(t);
}

# undef BOOST_REF_CONST

# ifndef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

template<typename T>
class is_reference_wrapper : public mpl::false_
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
		return std::less< sp_typeinfo const* >()( this, &rhs );
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