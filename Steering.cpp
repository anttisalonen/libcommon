#include "Steering.h"

namespace Common {

Steering::Steering(const Vehicle& e)
	: mUnit(e)
{
}

Vector3 Steering::seek(const Vector3& tgtpos)
{
	Vector3 desiredVelocity = (tgtpos - mUnit.getPosition()).normalized() * mUnit.getMaxSpeed();

	return desiredVelocity - mUnit.getVelocity();
}

Vector3 Steering::flee(const Vector3& threatpos)
{
	Vector3 desiredVelocity = (mUnit.getPosition() - threatpos).normalized() * mUnit.getMaxSpeed();

	return desiredVelocity - mUnit.getVelocity();
}

Vector3 Steering::arrive(const Vector3& tgtpos)
{
	auto dist = tgtpos - mUnit.getPosition();
	float distlen = dist.length();

	if(distlen < 0.0001f)
		return Vector3();

	float speed = std::min(distlen / 0.6f, mUnit.getMaxSpeed());

	Vector3 desiredVelocity = dist * (speed / distlen);

	return desiredVelocity - mUnit.getVelocity();
}

}

