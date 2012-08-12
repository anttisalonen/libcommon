#ifndef COMMON_RECTANGLE_H
#define COMMON_RECTANGLE_H

#include <iostream>

namespace Common {

struct Rectangle {
	inline Rectangle();
	inline Rectangle(float x_, float y_, float w_, float h_);
	inline bool pointWithin(float x_, float y_) const;
	float x;
	float y;
	float w;
	float h;
};

inline std::ostream& operator<<(std::ostream& out, const Rectangle& r)
{
	out << "(" << r.x << ", " << r.y << ") - (" << r.x + r.w << ", " << r.y + r.h << ")";
	return out;
}

Rectangle::Rectangle()
	: x(0), y(0), w(0), h(0) { }

Rectangle::Rectangle(float x_, float y_, float w_, float h_)
	: x(x_), y(y_), w(w_), h(h_)
{
}

bool Rectangle::pointWithin(float x_, float y_) const
{
	return x_ >= x && x_ <= x + w &&
		y_ >= y && y_ <= y + h;
}

}

#endif

