#ifndef __MATH_SQUARE_ROOT__
#define __MATH_SQUARE_ROOT__

#include <stdint.h>

class MATH_SQUARE_ROOT
{
public:
	static float GetSquareRoot(const float value)
	{
		union
		{
			int i;
			float value;
		} u;
		u.value = value;
		u.i = (1 << 29) + (u.i >> 1) - (1 << 22);
		return u.value;
	}

	static float invSquareRoot(const float value)
	{
		union {
			float f;
			uint32_t i;
		} conv;

		float x2;
		const float three_halfs = 1.5F;

		x2 = value * 0.5F;
		conv.f = value;
		conv.i = 0x5f3759df - (conv.i >> 1);
		conv.f = conv.f * (three_halfs - (x2 * conv.f * conv.f));
		return conv.f;
	}
};
#endif