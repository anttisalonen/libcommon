#ifndef COLOR_H
#define COLOR_H

#include <stdlib.h>

#include "Serialization.h"

namespace Common {

struct Color {
	Color(unsigned char r_ = 0,
			unsigned char g_ = 0,
			unsigned char b_ = 0)
		: r(r_),
		g(g_),
		b(b_) { }
	inline bool operator==(const Color& f) const;
	inline bool operator<(const Color& f) const;
	inline int operator-(const Color& f) const;
	inline void mix(const Color& c);
	inline unsigned char luminance() const;
	inline float hue() const; // returns radians [-pi, pi]
	unsigned char r, g, b;

	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Black;
	static const Color White;
	static const Color Yellow;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar & r;
		ar & g;
		ar & b;
	}
};

bool Color::operator==(const Color& f) const
{
	return r == f.r && g == f.g && b == f.b;
}

bool Color::operator<(const Color& f) const
{
	if(r != f.r)
		return r < f.r;
	if(g != f.g)
		return g < f.g;
	return b < f.b;
}

int Color::operator-(const Color& f) const
{
	int ret = 0;
	ret += abs(r - f.r);
	ret += abs(g - f.g);
	ret += abs(b - f.b);
	return ret;
}

void Color::mix(const Color& c)
{
	int r1 = r + c.r;
	int g1 = g + c.g;
	int b1 = b + c.b;

	r = r1 / 2;
	g = g1 / 2;
	b = b1 / 2;
}

unsigned char Color::luminance() const
{
	return 0.3 * r + 0.59 * g + 0.11 * b;
}

float Color::hue() const
{
	float rr = r / 255.0f;
	float gg = g / 255.0f;
	float bb = b / 255.0f;
	return atan2(sqrt(3.0) * (gg - bb), 2.0 * rr - gg - bb);
}

}

#endif

