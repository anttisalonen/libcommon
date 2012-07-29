#ifndef COMMON_VEHICLE_H
#define COMMON_VEHICLE_H

#include "Entity.h"

namespace Common {

class Wall {
	public:
		Wall(const Vector3& start, const Vector3& end)
			: mStart(start), mEnd(end) { }
		virtual ~Wall() { }
		const Vector3& getStart() const { return mStart; }
		const Vector3& getEnd() const { return mEnd; }

	protected:
		Vector3 mStart;
		Vector3 mEnd;
};

class Obstacle : public Entity {
	public:
		inline Obstacle(float radius) : mRadius(radius) { }
		float getRadius() const { return mRadius; }

	protected:
		float mRadius;
};

class Vehicle : public Obstacle {
	public:
		inline Vehicle(float radius, float maxspeed, float maxacc);
		float getMaxSpeed() const { return mMaxSpeed; }
		float getMaxAcceleration() const { return mMaxAcceleration; }
		virtual inline void update(float time) override;

	protected:
		float mMaxSpeed;
		float mMaxAcceleration;
};

Vehicle::Vehicle(float radius, float maxspeed, float maxacc)
	: Obstacle(radius),
	mMaxSpeed(maxspeed),
	mMaxAcceleration(maxacc)
{
}

void Vehicle::update(float time)
{
	mAcceleration.truncate(mMaxAcceleration);
	Entity::update(time);
	mVelocity.truncate(mMaxSpeed);
	mPosition.z = 0.0f;
}

}

#endif

