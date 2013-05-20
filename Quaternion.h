#ifndef COMMON_QUATERNION_H
#define COMMON_QUATERNION_H

#include "Vector3.h"

namespace Common {

class Quaternion {
	public:
		Quaternion();
		Quaternion(float x_, float y_, float z_, float w_);
		Quaternion conjugated() const;
		float norm() const;
		Quaternion versor() const;
		Quaternion operator*(float f) const;
		Quaternion operator*(const Vector3& v) const;
		Quaternion operator*(const Quaternion& q) const;
		Quaternion operator-() const;
		Quaternion operator+(const Quaternion& q) const;
		static Quaternion fromAxisAngle(const Vector3& axis, float angle);
		void toAxisAngle(Vector3& axis, float& angle) const;
		float dot(const Quaternion& q2) const;
		Quaternion slerp(const Quaternion& q2, float t) const;
		void reset();
		void toEuler(float& rotx, float& roty, float& rotz) const;

		static Quaternion getRotationTo(const Vector3& from, const Vector3& to);

		float x;
		float y;
		float z;
		float w;
};

inline std::ostream& operator<<(std::ostream& out, const Quaternion& q)
{
	out << "(" << q.x << ", " << q.y << ", " << q.z << ", " << q.w << ")";
	return out;
}

}

#endif

