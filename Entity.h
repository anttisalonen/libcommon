#ifndef COMMON_ENTITY_H
#define COMMON_ENTITY_H

#include "common/Vector3.h"

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
		inline virtual void update(float time);
		static inline Vector3 vectorFromTo(const Entity<T>& me1,
				const Entity<T>& me2);
		static inline double distanceBetween(const Entity<T>& me1,
				const Entity<T>& me2);

	protected:
		T mT;
		Vector3 mPosition;
		Vector3 mVelocity;
		Vector3 mAcceleration;
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
	: mT(t)
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
void Entity<T>::update(float time)
{
	mVelocity += mAcceleration * time;
	mPosition += mVelocity * time;
	mAcceleration = Vector3();
}

}

#endif

