#ifndef COMMON_ENTITY_H
#define COMMON_ENTITY_H

#include "common/Vector3.h"
#include "common/Math.h"

#include <cmath>

namespace Common {

template<typename T>
class Entity {
	public:
		Entity(T t);
		virtual ~Entity() { }
		inline const T getT() const;
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
		// returns true if maximum reached
		inline virtual bool capXYSpeed(float maxspeed);
		// returns true if falling down
		inline virtual bool applyGravity(float time, float ground = 0.0f, float gravity = 9.81f);
		inline virtual void updateComplete(float time, float maxspeed, float ground = 0.0f, float gravity = 9.81f);
		static inline Vector3 vectorFromTo(const Entity<T>& me1,
				const Entity<T>& me2);
		static inline double distanceBetween(const Entity<T>& me1,
				const Entity<T>& me2);

	protected:
		T mT;
		Vector3 mPosition;
		Vector3 mVelocity;
		Vector3 mAcceleration;
		float mRotation;
		float mRotationalVelocity;
		float mRotationalAcceleration;
};

template<typename T>
Vector3 Entity<T>::vectorFromTo(const Entity<T>& me1,
		const Entity<T>& me2)
{
	return Vector3(me2.getPosition().v - me1.getPosition().v);
}

template<typename T>
double Entity<T>::distanceBetween(const Entity<T>& me1,
		const Entity<T>& me2)
{
	return vectorFromTo(me1, me2).v.length();
}

template<typename T>
Entity<T>::Entity(T t)
	: mT(t),
	mRotation(0.0f),
	mRotationalVelocity(0.0f),
	mRotationalAcceleration(0.0f)
{
}

template<typename T>
const T Entity<T>::getT() const
{
	return mT;
}

template<typename T>
void Entity<T>::move(const Vector3& v)
{
	mPosition += v;
}

template<typename T>
void Entity<T>::setPosition(const Vector3& v)
{
	mPosition = v;
}

template<typename T>
void Entity<T>::setVelocity(const Vector3& v)
{
	mVelocity = v;
}

template<typename T>
void Entity<T>::addVelocity(const Vector3& v)
{
	mVelocity += v;
}

template<typename T>
void Entity<T>::setAcceleration(const Vector3& v)
{
	mAcceleration = v;
}

template<typename T>
const Vector3& Entity<T>::getPosition() const
{
	return mPosition;
}

template<typename T>
const Vector3& Entity<T>::getVelocity() const
{
	return mVelocity;
}

template<typename T>
void Entity<T>::setXYRotation(float r)
{
	mRotation = 0.0f;
	addXYRotation(r);
}

template<typename T>
void Entity<T>::addXYRotation(float r)
{
	mRotation += r;
	if(mRotation >= PI || mRotation < -PI) {
		mRotation = std::fmod(mRotation, PI);
	}
}

template<typename T>
float Entity<T>::getXYRotation() const
{
	return mRotation;
}

template<typename T>
void Entity<T>::setXYRotationalVelocity(float r)
{
	mRotationalVelocity = 0.0f;
	addXYRotationalVelocity(r);
}

template<typename T>
void Entity<T>::addXYRotationalVelocity(float r)
{
	mRotationalVelocity += r;
}

template<typename T>
float Entity<T>::getXYRotationalVelocity() const
{
	return mRotationalVelocity;
}

template<typename T>
void Entity<T>::setXYRotationalAcceleration(float r)
{
	mRotationalAcceleration = 0.0f;
	addXYRotationalAcceleration(r);
}

template<typename T>
void Entity<T>::addXYRotationalAcceleration(float r)
{
	mRotationalAcceleration += r;
}

template<typename T>
float Entity<T>::getXYRotationalAcceleration() const
{
	return mRotationalAcceleration;
}

template<typename T>
void Entity<T>::update(float time)
{
	mVelocity += mAcceleration * time;
	mPosition += mVelocity * time;
	mAcceleration = Vector3();

	mRotationalVelocity += mRotationalAcceleration * time;
	mRotation += mRotation * time;
	mRotationalAcceleration = 0.0f;
}

template<typename T>
bool Entity<T>::capXYSpeed(float maxspeed)
{
	Vector3 planevel(mVelocity);
	planevel.z = 0.0f;
	if(planevel.length() > maxspeed) {
		planevel.normalize();
		planevel *= maxspeed;
		mVelocity.x = planevel.x;
		mVelocity.y = planevel.y;
		return true;
	}
	return false;
}

template<typename T>
bool Entity<T>::applyGravity(float time, float ground, float gravity)
{
	if(mPosition.z < ground) {
		mVelocity.z = std::max(0.0f, mVelocity.z);
		mPosition.z = std::max(ground, mPosition.z);
		return false;
	}
	else {
		mVelocity.z -= gravity * time;
		return true;
	}
}

template<typename T>
void Entity<T>::updateComplete(float time, float maxspeed, float ground, float gravity)
{
	Entity<T>::update(time);
	capXYSpeed(maxspeed);
	applyGravity(time, ground, gravity);
}

}

#endif

