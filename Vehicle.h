#ifndef COMMON_VEHICLE_H
#define COMMON_VEHICLE_H

#include "Entity.h"

namespace Common {

class Vehicle : public Entity {
	public:
		inline Vehicle(float maxspeed, float maxacc);
		float getMaxSpeed() const { return mMaxSpeed; }
		float getMaxAcceleration() const { return mMaxAcceleration; }
		virtual inline void update(float time) override;

	protected:
		float mMaxSpeed;
		float mMaxAcceleration;
		
	private:
};

Vehicle::Vehicle(float maxspeed, float maxacc)
	: mMaxSpeed(maxspeed),
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

