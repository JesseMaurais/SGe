#ifndef Fixed_hpp
#define Fixed_hpp


unsigned Degrees(double theta);
unsigned Radians(double theta);

int Sin(unsigned theta, unsigned radius);
int Cos(unsigned theta, unsigned radius);


template <typename Base, unsigned Bits> struct Fixed
{
	Base n;

	using Mask = Base;

	static constexpr Mask Point = Bits;
	static constexpr Mask Zero = 0;
	static constexpr Mask Unit = 1 << Point;
	static constexpr Mask Flip = ~Zero;
	static constexpr Mask Abs = Flip >> 1;
	static constexpr Mask Sign = ~Abs;
	static constexpr Mask Trunc = Flip << Point;
	static constexpr Mask Whole = Trunc & Abs;
	static constexpr Mask Fract = ~Trunc;

	struct Parts
	{
		Mask sign;
		Base fract;
		Base whole;

		Parts(const Base &n)
		{
			sign = n & Sign;
			fract = n & Fract;
			whole = (n & Whole) >> Point;
		}
	};

	Fixed(double d)
	{
		n = Base(d * Unit);
	}

	Fixed(const Parts &m)
	{
		n = m.sign | m.fract | m.whole << Point;
	}

	Fixed() = default;

	inline Fixed operator+=(const Base &that)
	{
		n += (that.n << Point) | (that.n & Sign);
		return *this;
	}

	inline Fixed operator+=(const Fixed &that)
	{
		n += that.n;
		return *this;
	}

	inline Fixed operator-=(const Base &that)
	{
		n -= (that.n << Point) | (that.n & Sign);
		return *this;
	}

	inline Fixed operator-=(const Fixed &that)
	{
		n -= that.n;
		return *this;
	}

	inline Fixed operator*=(const Base &that)
	{
		n *= that;
		return *this;
	}

	inline Fixed operator*=(const Fixed &that)
	{
		Parts A(n), B(that.n);

		n  = (A.whole * B.whole) << Point; // First
		n += (A.whole * B.fract);          // Outside
		n += (A.fract * B.whole);          // Inside
		n += (A.fract * B.fract) >> Point; // Last

		n |= (A.sign ^ B.sign) & Sign;

		return *this;
	}

	inline Fixed operator/=(const Base &that)
	{
		n /= that;
		return *this;
	}
};

#endif // file
