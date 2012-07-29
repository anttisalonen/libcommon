#ifndef COMMON_VECTOR3_H
#define COMMON_VECTOR3_H

#include <iostream>
#include <math.h>

namespace Common {

class Vector3 {
	public:
		inline Vector3();
		inline Vector3(float x_, float y_, float z_);
		float x;
		float y;
		float z;
		inline Vector3 operator-(const Vector3& rhs) const;
		inline void operator-=(const Vector3& rhs);
		inline Vector3 operator+(const Vector3& rhs) const;
		inline void operator+=(const Vector3& rhs);
		inline Vector3 operator*(float v) const;
		inline void operator*=(float v);
		inline Vector3 normalized() const;
		inline void normalize();
		inline float length() const;
		inline float length2() const;
		inline bool null() const;
		inline double dot(const Vector3& v) const;
		inline void zero();
		inline void truncate(float len);
		inline float distance(const Vector3& v) const;
		inline float distance2(const Vector3& v) const;
		inline float cross2d(const Vector3& v) const;
};

inline std::ostream& operator<<(std::ostream& out, const Vector3& vec)
{
	out << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return out;
}

Vector3::Vector3()
	: x(0.0f),
	y(0.0f),
	z(0.0f) { }

Vector3::Vector3(float x_, float y_, float z_)
	: x(x_),
	y(y_),
	z(z_) { }

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	Vector3 r(*this);
	r -= rhs;
	return r;
}

void Vector3::operator-=(const Vector3& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
	Vector3 r(*this);
	r += rhs;
	return r;
}

void Vector3::operator+=(const Vector3& rhs)
{
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
}

inline Vector3 Vector3::operator*(float v) const
{
	Vector3 r(*this);
	r.x *= v;
	r.y *= v;
	r.z *= v;
	return r;
}

inline void Vector3::operator*=(float v)
{
	x *= v;
	y *= v;
	z *= v;
}

Vector3 Vector3::normalized() const
{
	Vector3 r(*this);
	r.normalize();
	return r;
}

void Vector3::normalize()
{
	float l = length();
	if(l != 0.0f) {
		x /= l;
		y /= l;
		z /= l;
	}
}

float Vector3::length() const
{
	return sqrt(length2());
}

float Vector3::length2() const
{
	return x * x + y * y + z * z;
}

bool Vector3::null() const
{
	return x == 0.0f && y == 0.0f && z == 0.0f;
}

inline double Vector3::dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

inline void Vector3::zero()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

inline void Vector3::truncate(float len)
{
	if(length2() > len * len) {
		normalize();
		x *= len;
		y *= len;
		z *= len;
	}
}

inline float Vector3::distance(const Vector3& v) const
{
	return (*this - v).length();
}

inline float Vector3::distance2(const Vector3& v) const
{
	return (*this - v).length2();
}

inline float Vector3::cross2d(const Vector3& v) const
{
	return x * v.y - y * v.x;
}

}

#endif
