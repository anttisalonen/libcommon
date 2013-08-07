#ifndef COMMON_MATRIX22_H
#define COMMON_MATRIX22_H

#include "Vector2.h"

namespace Common {

class Matrix22 {
	public:
		Matrix22();
		Matrix22(float a11, float a12,
				float a21, float a22);

		Matrix22 operator*(const Matrix22& rhs) const;
		Vector2 operator*(const Vector2& rhs) const;
		Matrix22 operator*(float rhs) const;
		void operator*=(const Matrix22& rhs);

		Matrix22 transposed() const;
		void transpose();

		float determinant() const;
		Matrix22 inverse() const;

		static const Matrix22 Identity;

		float m[4];
};

inline std::ostream& operator<<(std::ostream& out, const Matrix22& m)
{
	out << "[" << m.m[0] << " " << m.m[1] << "]\n";
	out << "[" << m.m[2] << " " << m.m[3] << "]";
	return out;
}

}

#endif

