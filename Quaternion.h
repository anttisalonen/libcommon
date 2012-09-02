#ifndef COMMON_QUATERNION_H
#define COMMON_QUATERNION_H

#include "Vector3.h"

namespace Common {

class Quaternion {
	public:
		Quaternion(float x_, float y_, float z_, float w_);
		Quaternion conjugated() const;
		Quaternion operator*(const Vector3& v) const;
		Quaternion operator*(const Quaternion& q) const;

		float x;
		float y;
		float z;
		float w;
};

}

#endif

