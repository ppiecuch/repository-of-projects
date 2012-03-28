#ifndef _YON_CORE_RANDOMIZER_H_
#define _YON_CORE_RANDOMIZER_H_

namespace yon{
namespace core{

	class randomizer{
	public:
		static void reset();
		static s32 rand();
	private:
		static s32 seed;
	};

	s32 randomizer::seed = 0x0f0f0f0f;
	s32 randomizer::rand()
	{
		const s32 m = 2147483399;	// a non-Mersenne prime
		const s32 a = 40692;		// another spectral success story
		const s32 q = m/a;
		const s32 r = m%a;		// again less than q

		seed = a * (seed%q) - r* (seed/q);
		if (seed<0) seed += m;

		return seed;
	}
	void randomizer::reset()
	{
		seed = 0x0f0f0f0f;
	}

}//core
}//yon
#endif