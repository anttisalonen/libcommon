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
	float w_ = - (w * q.w) - (x * q.x) - (y * q.y) - (z * q.z);
	float x_ =   (x * q.w) + (w * q.x) + (y * q.z) - (z * q.y);
	float y_ =   (y * q.w) + (w * q.y) + (z * q.x) - (x * q.z);
	float z_ =   (z * q.w) + (w * q.z) + (x * q.y) - (y * q.x);
	return Quaternion(x_, y_, z_, w_);
}


}

