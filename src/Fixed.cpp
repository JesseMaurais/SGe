#include "Fixed.hpp"
#include <cassert>
#include <cstdint>
#include <cmath>


constexpr std::size_t CircuitBits = 9;
constexpr std::size_t Circuit = 1 << CircuitBits;

typedef int_fast32_t signed_int;
typedef uint_fast32_t unsigned_int;

typedef Fixed<unsigned_int, 32 - CircuitBits> Angle;
typedef Fixed<signed_int, 31> Sinusoid;


struct Lookup
{
	static const signed_int &Entry(const unsigned_int theta)
	{
		static Lookup singleton;
		unsigned_int index = theta >> Angle::Point;
		assert(index < Circuit);
		return singleton.table[index].n;
	}

 private:

	Sinusoid table[Circuit];

	Lookup()
	{
		constexpr auto Convert = Circuit / M_PI;

		for (std::size_t index = 0; index < Circuit; ++index)
		{
			table[index] = std::sin(Convert * index);
		}
	}
};


unsigned Degrees(double theta)
{
	return Angle(theta * Circuit / 360.0).n;
}

unsigned Radians(double theta)
{
	return Angle(theta * Circuit / M_PI).n;
}

int Sin(unsigned theta, unsigned radius)
{
	const auto sine = Lookup::Entry(theta);
	return (sine * radius) >> Sinusoid::Point;
}

int Cos(unsigned theta, unsigned radius)
{
	static unsigned_int HalfPi = Degrees(90);
	return Sin(theta + HalfPi, radius);
}
