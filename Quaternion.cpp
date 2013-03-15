#include "Quaternion.h"
#include "Math.h"

namespace Common {

Quaternion::Quaternion()
	: x(0), y(0), z(0), w(1)
{
}

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

Quaternion Quaternion::operator*(float f) const
{
	return Quaternion(x * f, y * f, z * f, w * f);
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

Quaternion Quaternion::operator-() const
{
	return Quaternion(-x, -y, -z, -w);
}

Quaternion Quaternion::operator+(const Quaternion& q) const
{
	return Quaternion(x + q.x, y + q.y, z + q.z, w + q.w);
}

Quaternion Quaternion::fromAxisAngle(const Vector3& axis, float angle)
{
	float half = angle * 0.5f;
	float s = sin(half);
	return Quaternion(s * axis.x, s * axis.y, s * axis.z, cos(half));
}

float Quaternion::dot(const Quaternion& q2) const
{
	return w * q2.w + x * q2.x + y * q2.y + z * q2.z;
}

Quaternion Quaternion::slerp(const Quaternion& q2, float t) const
{
	float omega = dot(q2);
	Quaternion q3(0, 0, 0, 1);

	// use shortest path
	if(omega < 0.0f) {
		omega = -omega;
		q3 = -q2;
	}
	else
	{
		q3 = q2;
	}

	if(fabs(omega) < 1.0f - 1.0e3) {
		// Standard case (slerp)
		float sn = sqrt(1 - omega * omega);
		float ang = atan2(sn, omega);
		float isn = 1.0f / sn;
		float c0 = sin((1.0f - t) * ang) * isn;
		float c1 = sin(t * ang) * isn;
		return *this * c0 + q3 * c1;
	} else {
		// linear interpolation (this and q2 are close or inverse)
		Quaternion tn = *this * (1.0f - t) + q3 * t;
		return tn.versor();
	}
}

// based on implementation in Ogre, which is based on
// Stan Melax's article in Game Programming Gems
Quaternion Quaternion::getRotationTo(const Vector3& from, const Vector3& to)
{
	Quaternion q(0, 0, 0, 1);
	Vector3 v0 = from.normalized();
	Vector3 v1 = to.normalized();

	float d = v0.dot(v1);
	if(d >= 1.0f) {
		// equal vectors
		return q;
	}

	if(d < (1e-6f - 1.0f)) {
		// Generate an axis
		Vector3 axis = Vector3(1, 0, 0).cross(from);
		if (axis.length2() < 0.001f) // pick another if colinear
			axis = Vector3(0, 1, 0).cross(from);
		return q.fromAxisAngle(axis.normalized(), PI);
	} else {
		float s = sqrt((1.0f + d) * 2.0f);
		float invs = 1.0f / s;
		Vector3 c = v0.cross(v1);

		q.x = c.x * invs;
		q.y = c.y * invs;
		q.z = c.z * invs;
		q.w = s * 0.5f;
		return q.versor();
	}
}

void Quaternion::reset()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

void Quaternion::toEuler(float& rotx, float& roty, float& rotz) const
{
	rotx = atan2(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
	roty = asin(2.0f * (w * y - z * x));
	rotz = atan2(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
}


}

