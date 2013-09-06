#ifndef COMMON_MATRIX44_H
#define COMMON_MATRIX44_H

#include <iostream>

namespace Common {

class Matrix44 {
	public:
		Matrix44();
		Matrix44(float a11, float a12, float a13, float a14,
				float a21, float a22, float a23, float a24,
				float a31, float a32, float a33, float a34,
				float a41, float a42, float a43, float a44);

		Matrix44 operator*(const Matrix44& rhs) const;
		void operator*=(const Matrix44& rhs);

		Matrix44 transposed() const;
		void transpose();

		static const Matrix44 Identity;

		float m[16];
};

inline std::ostream& operator<<(std::ostream& out, const Matrix44& m)
{
	out << "[" << m.m[0] << " " << m.m[1] << " " << m.m[2] << " " << m.m[3] << "]\n";
	out << "[" << m.m[4] << " " << m.m[5] << " " << m.m[6] << " " << m.m[7] << "]\n";
	out << "[" << m.m[8] << " " << m.m[9] << " " << m.m[10] << " " << m.m[11] << "]\n";
	out << "[" << m.m[12] << " " << m.m[13] << " " << m.m[14] << " " << m.m[15] << "]\n";
	return out;
}

}

#endif

