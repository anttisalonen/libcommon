#include "Quaternion.h"

namespace Common {

Quaternion::Quaternion(float x_, float y_, float z_, float w_)
	: x(x_),
	y(y_),
	z(z_),
	w(w_)
{
}

Quaternion Quaternion::conjugated() const
{
	return Quaternion(-x, -y, -z, w);
}

float Quaternion::norm() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::versor() const
{
	float n = norm();
	return Quaternion(x / n, y / n, z / n, w / n);
}

Quaternion Quaternion::operator*(const Vector3& v) const
{
	float w_ = - (x * v.x) - (y * v.y) - (z * v.z);
	float x_ =   (w * v.x) + (y * v.z) - (z * v.y);
	float y_ =   (w * v.y) + (z * v.x) - (x * v.z);
	float z_ =   (w * v.z) + (x * v.y) - (y * v.x);
	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::operator*(const Quaternion& q) const
{
	// the ordering is correct
	float w_ =   (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z);
	float x_ =   (x * q.w) + (w * q.x) + (y * q.z) - (z * q.y);
	float y_ =   (y * q.w) + (w * q.y) + (z * q.x) - (x * q.z);
	float z_ =   (z * q.w) + (w * q.z) + (x * q.y) - (y * q.x);
	return Quaternion(x_, y_, z_, w_);
}

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, float angle)
{
	float half = angle * 0.5f;
	float s = sin(half);
	return Quaternion(s * axis.x, s * axis.y, s * axis.z, cos(half));
}


}

