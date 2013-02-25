#ifndef COMMON_QUATERNION_H
#define COMMON_QUATERNION_H

#include "Vector3.h"

namespace Common {

class Quaternion {
	public:
		Quaternion(float x_, float y_, float z_, float w_);
		Quaternion conjugated() const;
		float norm() const;
		Quaternion versor() const;
		Quaternion operator*(const Vector3& v) const;
		Quaternion operator*(const Quaternion& q) const;
		static Quaternion fromAxisAngle(const Vector3& axis, float angle);

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

