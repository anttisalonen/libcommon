#ifndef COMMON_ENTITY_H
#define COMMON_ENTITY_H

#include "Vector3.h"
#include "Math.h"

#include <cmath>

namespace Common {

class Entity {
	public:
		inline Entity();
		virtual ~Entity() { }
		inline void move(const Vector3& v);
		inline void setVelocity(const Vector3& v);
		inline void addVelocity(const Vector3& v);
		inline void setPosition(const Vector3& v);
		inline void setAcceleration(const Vector3& v);
		inline const Vector3& getPosition() const;
		inline const Vector3& getVelocity() const;
		inline void setXYRotation(float r);
		inline void addXYRotation(float r);
		inline float getXYRotation() const;
		inline void setXYRotationalVelocity(float r);
		inline void addXYRotationalVelocity(float r);
		inline float getXYRotationalVelocity() const;
		inline void setXYRotationalAcceleration(float r);
		inline void addXYRotationalAcceleration(float r);
		inline float getXYRotationalAcceleration() const;
		inline virtual void update(float time);
		static inline Vector3 vectorFromTo(const Entity& me1,
				const Entity& me2);
		static inline double distanceBetween(const Entity& me1,
				const Entity& me2);
		inline float getSpeed() const;
		inline void setAutomaticHeading();
		inline void setVelocityToHeading();
		inline Vector3 getHeadingVector() const;

	protected:
		Vector3 mPosition;
		Vector3 mVelocity;
		Vector3 mAcceleration;
		float mRotation;
		float mRotationalVelocity;
		float mRotationalAcceleration;
};

Vector3 Entity::vectorFromTo(const Entity& me1,
		const Entity& me2)
{
	return Vector3(me2.getPosition() - me1.getPosition());
}

double Entity::distanceBetween(const Entity& me1,
		const Entity& me2)
{
	return vectorFromTo(me1, me2).length();
}

Entity::Entity()
	: mRotation(0.0f),
	mRotationalVelocity(0.0f),
	mRotationalAcceleration(0.0f)
{
}

void Entity::move(const Vector3& v)
{
	mPosition += v;
}

void Entity::setPosition(const Vector3& v)
{
	mPosition = v;
}

void Entity::setVelocity(const Vector3& v)
{
	mVelocity = v;
}

void Entity::addVelocity(const Vector3& v)
{
	mVelocity += v;
}

void Entity::setAcceleration(const Vector3& v)
{
	mAcceleration = v;
}

const Vector3& Entity::getPosition() const
{
	return mPosition;
}

const Vector3& Entity::getVelocity() const
{
	return mVelocity;
}

void Entity::setXYRotation(float r)
{
	mRotation = 0.0f;
	addXYRotation(r);
}

void Entity::addXYRotation(float r)
{
	mRotation += r;
	if(mRotation >= PI || mRotation < -PI) {
		mRotation = std::fmod(mRotation, PI);
	}
}

float Entity::getXYRotation() const
{
	return mRotation;
}

void Entity::setXYRotationalVelocity(float r)
{
	mRotationalVelocity = 0.0f;
	addXYRotationalVelocity(r);
}

void Entity::addXYRotationalVelocity(float r)
{
	mRotationalVelocity += r;
}

float Entity::getXYRotationalVelocity() const
{
	return mRotationalVelocity;
}

void Entity::setXYRotationalAcceleration(float r)
{
	mRotationalAcceleration = 0.0f;
	addXYRotationalAcceleration(r);
}

void Entity::addXYRotationalAcceleration(float r)
{
	mRotationalAcceleration += r;
}

float Entity::getXYRotationalAcceleration() const
{
	return mRotationalAcceleration;
}

void Entity::update(float time)
{
	mVelocity += mAcceleration * time;
	mPosition += mVelocity * time;
	mAcceleration = Vector3();

	mRotationalVelocity += mRotationalAcceleration * time;
	mRotation += mRotation * time;
	mRotationalAcceleration = 0.0f;
}

inline float Entity::getSpeed() const
{
	return mVelocity.length();
}

void Entity::setAutomaticHeading()
{
	if(mVelocity.null()) {
		return;
	}
	mRotation = atan2(mVelocity.y, mVelocity.x);
}

inline void Entity::setVelocityToHeading()
{
	float s = getSpeed();
	mVelocity = getHeadingVector() * s;
}

inline Vector3 Entity::getHeadingVector() const
{
	return Vector3(cos(mRotation), sin(mRotation), 0.0f);
}


}

#endif

