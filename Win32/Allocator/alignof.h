#ifndef _YON_CORE_ALIGNOF_H_
#define _YON_CORE_ALIGNOF_H_

namespace yon{
namespace core{

	//refer to:http://llvm.org/docs/doxygen/html/AlignOf_8h_source.html

	template<typename T>
	struct AlignmentImpl{
		c8 c;
		T t;
	private:
		AlignmentImpl(){} // Never instantiate.
	};

	/// AlignOf - A templated class that contains an enum value representing
	///  the alignment of the template argument.  For example,
	///  AlignOf<int>::Alignment represents the alignment of type "int".  The
	///  alignment calculated is the minimum alignment, and not necessarily
	///  the "desired" alignment returned by GCC's __alignof__ (for example).  Note
	///  that because the alignment is an enum value, it can be used as a
	///  compile-time constant (e.g., for template instantiation).

	template<typename T>
	struct AlignOf{
		enum {ALIGNMENT=static_cast<u32>(sizeof(AlignmentImpl<T>)-sizeof(T))};

		enum {ALIGNMENT_GE2=ALIGNMENT>=2?1:0};
		enum {ALIGNMENT_GE4=ALIGNMENT>=4?1:0};
		enum {ALIGNMENT_GE8=ALIGNMENT>=8?1:0};
		enum {ALIGNMENT_GE16=ALIGNMENT>=16?1:0};

		enum {ALIGNMENT_LE2=ALIGNMENT<=2?1:0};
		enum {ALIGNMENT_LE4=ALIGNMENT<=4?1:0};
		enum {ALIGNMENT_LE8=ALIGNMENT<=8?1:0};
		enum {ALIGNMENT_LE16=ALIGNMENT<=16?1:0};
	};

	/// alignOf - A templated function that returns the minimum alignment of
	///  of a type.  This provides no extra functionality beyond the AlignOf
	///  class besides some cosmetic cleanliness.  Example usage:
	///  alignOf<int>() returns the alignment of an int.
	template<typename T>
	inline unsigned alignOf() { return AlignOf<T>::ALIGNMENT; }
}
}
#endif