#include <cfloat>

#include "Steering.h"

#include "Math.h"
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

Vector3 Steering::obstacleAvoidance(const std::vector<Obstacle*> obstacles)
{
	Obstacle* nearest = nullptr;

	float distToNearest = FLT_MAX;

	if(mUnit.getVelocity().null())
		return Vector3();

	for(auto o : obstacles) {
		float dot = mUnit.getPosition().dot(o->getPosition());
		if(dot < 0.0f) {
			continue;
		}

		float rad = mUnit.getRadius() + o->getRadius();

		float dist = Math::pointToSegmentDistance(mUnit.getPosition(),
				mUnit.getPosition() + mUnit.getVelocity() * 0.5f,
				o->getPosition()) - rad;
		if(dist < distToNearest) {
			distToNearest = dist;
			nearest = o;
		}
	}

	if(!nearest) {
		return Vector3();
	}

	Vector3 vecFromObj = Entity::vectorFromTo(*nearest, mUnit);
	if(vecFromObj.length() < mUnit.getRadius() + nearest->getRadius()) {
		// we're inside the obstacle
		return vecFromObj.normalized() * 100.0f;
	}
	distToNearest = std::max(0.01f, distToNearest);
	float velmultiplier = 0.1f + mUnit.getVelocity().length() * 0.5f;
	float multiplier = velmultiplier / distToNearest;

	Vector3 res = vecFromObj.normalized() * multiplier;

	return res;
}

bool Steering::accumulate(Vector3& runningTotal, const Vector3& add)
{
	float magnitude = runningTotal.length();
	float remaining = mUnit.getMaxAcceleration() - magnitude;

	if(remaining <= 0.0f)
		return false;

	double toAdd = add.length();

	if(toAdd < remaining) {
		runningTotal += add;
		return true;
	}
	else {
		runningTotal += add.normalized() * remaining;
		return false;
	}
}

Vector3 Steering::wallAvoidance(const std::vector<Wall*> walls)
{
	Vector3 nearestPointOnWall;
	float distToNearest = FLT_MAX;

	if(mUnit.getVelocity().null())
		return Vector3();

	for(auto w : walls) {
		bool found = false;
		Math::segmentSegmentIntersection2D(mUnit.getPosition(),
				mUnit.getPosition() + mUnit.getVelocity() * 0.5f,
				w->getStart(), w->getEnd(), &found);
		if(found) {
			Vector3 nearest;
			float dist = Math::pointToSegmentDistance(w->getStart(),
					w->getEnd(),
					mUnit.getPosition(), &nearest);
			if(dist < distToNearest) {
				distToNearest = dist;
				nearestPointOnWall = nearest;
			}
		}
	}

	if(distToNearest == FLT_MAX) {
		return Vector3();
	}

	Vector3 vecFromPoint = mUnit.getPosition() - nearestPointOnWall;
	float velmultiplier = 5.0f + mUnit.getVelocity().length() * 1.0f;
	float multiplier = velmultiplier / distToNearest;

	Vector3 res = vecFromPoint.normalized() * multiplier;

	return res;
}

}

