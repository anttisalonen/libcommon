#include "Matrix22.h"

#include <stdexcept>
#include <string.h>

namespace Common {

Matrix22::Matrix22()
{
	memset(m, 0, sizeof(m));
	m[0] = 1.0f;
	m[3] = 1.0f;
}


Matrix22::Matrix22(float a11, float a12,
		float a21, float a22)
{
	m[0] = a11;
	m[1] = a12;
	m[2] = a21;
	m[3] = a22;
}

Matrix22 Matrix22::operator*(const Matrix22& rhs) const
{
	Matrix22 res;

	res.m[0] = m[0] * rhs.m[0] + m[1] * rhs.m[2];
	res.m[1] = m[0] * rhs.m[1] + m[1] * rhs.m[3];

	res.m[2] = m[2] * rhs.m[0] + m[3] * rhs.m[2];
	res.m[3] = m[2] * rhs.m[1] + m[3] * rhs.m[3];

	return res;
}

Vector2 Matrix22::operator*(const Vector2& rhs) const
{
	Vector2 ret;
	ret.x = m[0] * rhs.x + m[1] * rhs.y;
	ret.y = m[2] * rhs.x + m[3] * rhs.y;
	return ret;
}

Matrix22 Matrix22::operator*(float rhs) const
{
	Matrix22 ret(*this);
	ret.m[0] *= rhs;
	ret.m[1] *= rhs;
	ret.m[2] *= rhs;
	ret.m[3] *= rhs;
	return ret;
}

void Matrix22::operator*=(const Matrix22& rhs)
{
	Matrix22 res = *this * rhs;
	*this = res;
}

Matrix22 Matrix22::transposed() const
{
	Matrix22 ret(*this);

	ret.m[1] = m[3];

	return ret;
}

void Matrix22::transpose()
{
	*this = this->transposed();
}

float Matrix22::determinant() const
{
	return m[0] * m[3] - m[1] * m[2];
}

Matrix22 Matrix22::inverse() const
{
	float det = determinant();
	if(!det) {
		throw std::runtime_error("Trying to get the inverse of a 2x2 matrix with a zero determinant");
	}
	Matrix22 ret;
	ret.m[0] = m[3];
	ret.m[1] = -m[1];
	ret.m[2] = -m[2];
	ret.m[3] = m[0];

	return ret * det;
}

const Matrix22 Matrix22::Identity = Matrix22(1, 0,
		0, 1);


}

