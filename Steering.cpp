#include "Steering.h"
#include "Random.h"

namespace Common {

Steering::Steering(const Vehicle& e)
	: mUnit(e),
	mWanderRadius(2.0f),
	mWanderDistance(3.0f),
	mWanderJitter(1.0f)
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

Vector3 Steering::pursuit(const Vehicle& tgt)
{
	Vector3 totgt = tgt.getPosition() - mUnit.getPosition();

	float relHeading = mUnit.getVelocity().dot(tgt.getVelocity());

	if((totgt.dot(mUnit.getPosition()) > 0.0f) && (relHeading < -0.95f)) {
		return seek(tgt.getPosition());
	}

	float lookAheadTime = totgt.length() * (1.0f / mUnit.getMaxSpeed() + tgt.getSpeed());

	return seek(tgt.getPosition() + tgt.getVelocity() * lookAheadTime);
}

Vector3 Steering::evade(const Vehicle& threat)
{
	Vector3 tothreat = threat.getPosition() - mUnit.getPosition();

	float lookAheadTime = tothreat.length() * (1.0f / mUnit.getMaxSpeed() + threat.getSpeed());

	return flee(threat.getPosition() + threat.getVelocity() * lookAheadTime);
}

Vector3 Steering::wander()
{
	mWanderTarget += Vector3(Random::clamped() * mWanderJitter,
			Random::clamped() * mWanderJitter,
			0.0f);

	mWanderTarget.normalize();

	mWanderTarget *= mWanderRadius;

	Vector3 target = mUnit.getPosition() + mUnit.getVelocity().normalized() * mWanderDistance + mWanderTarget;

	return target - mUnit.getPosition();
}

}

