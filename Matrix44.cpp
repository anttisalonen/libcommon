#include "Matrix44.h"

#include <string.h>

namespace Common {

Matrix44::Matrix44()
{
	memset(m, 0, sizeof(m));
}


Matrix44::Matrix44(float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44)
{
	m[0] = a11;
	m[1] = a12;
	m[2] = a13;
	m[3] = a14;
	m[4] = a21;
	m[5] = a22;
	m[6] = a23;
	m[7] = a24;
	m[8] = a31;
	m[9] = a32;
	m[10] = a33;
	m[11] = a34;
	m[12] = a41;
	m[13] = a42;
	m[14] = a43;
	m[15] = a44;
}

const Matrix44 Matrix44::Identity = Matrix44(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);


}

