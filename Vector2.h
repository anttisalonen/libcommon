#ifndef COMMON_VECTOR2_H
#define COMMON_VECTOR2_H

#include <iostream>
#include <math.h>

#include <boost/serialization/access.hpp>

namespace Common {

class Vector2 {
	public:
		inline Vector2();
		inline Vector2(float x_, float y_);
		float x;
		float y;
		inline Vector2 operator-(const Vector2& rhs) const;
		inline void operator-=(const Vector2& rhs);
		inline Vector2 operator+(const Vector2& rhs) const;
		inline void operator+=(const Vector2& rhs);
		inline Vector2 operator*(float v) const;
		inline void operator*=(float v);
		inline Vector2 operator/(float v) const;
		inline void operator/=(float v);
		inline Vector2 normalized() const;
		inline void normalize();
		inline float length() const;
		inline float length2() const;
		inline bool null() const;
		inline double dot(const Vector2& v) const;
		inline void zero();
		inline void truncate(float len);
		inline float distance(const Vector2& v) const;
		inline float distance2(const Vector2& v) const;
		inline float cross2d(const Vector2& v) const;
		inline void negate();
		inline Vector2 negated() const;

	private:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & x;
			ar & y;
		}
};

inline std::ostream& operator<<(std::ostream& out, const Vector2& vec)
{
	out << "(" << vec.x << ", " << vec.y << ")";
	return out;
}

Vector2::Vector2()
	: x(0.0f),
	y(0.0f) { }

Vector2::Vector2(float x_, float y_)
	: x(x_),
	y(y_) { }

Vector2 Vector2::operator-(const Vector2& rhs) const
{
	Vector2 r(*this);
	r -= rhs;
	return r;
}

void Vector2::operator-=(const Vector2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

Vector2 Vector2::operator+(const Vector2& rhs) const
{
	Vector2 r(*this);
	r += rhs;
	return r;
}

void Vector2::operator+=(const Vector2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

inline Vector2 Vector2::operator*(float v) const
{
	Vector2 r(*this);
	r.x *= v;
	r.y *= v;
	return r;
}

inline void Vector2::operator*=(float v)
{
	x *= v;
	y *= v;
}

inline Vector2 Vector2::operator/(float v) const
{
	Vector2 r(*this);
	r.x /= v;
	r.y /= v;
	return r;
}

inline void Vector2::operator/=(float v)
{
	x /= v;
	y /= v;
}

Vector2 Vector2::normalized() const
{
	Vector2 r(*this);
	r.normalize();
	return r;
}

void Vector2::normalize()
{
	float l = length();
	if(l != 0.0f) {
		x /= l;
		y /= l;
	}
}

float Vector2::length() const
{
	return sqrt(length2());
}

float Vector2::length2() const
{
	return x * x + y * y;
}

bool Vector2::null() const
{
	return x == 0.0f && y == 0.0f;
}

inline double Vector2::dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

inline void Vector2::zero()
{
	x = 0.0f;
	y = 0.0f;
}

inline void Vector2::truncate(float len)
{
	if(length2() > len * len) {
		normalize();
		x *= len;
		y *= len;
	}
}

inline float Vector2::distance(const Vector2& v) const
{
	return (*this - v).length();
}

inline float Vector2::distance2(const Vector2& v) const
{
	return (*this - v).length2();
}

inline float Vector2::cross2d(const Vector2& v) const
{
	return x * v.y - y * v.x;
}

inline void Vector2::negate()
{
	x = -x;
	y = -y;
}

inline Vector2 Vector2::negated() const
{
	Vector2 v(*this);
	v.negate();
	return v;
}

}

#endif
