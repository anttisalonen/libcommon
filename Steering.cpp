#include "Steering.h"

namespace Common {

Steering::Steering(const Entity& e)
	: mEntity(e)
{
}

Vector3 Steering::seek(const Vector3& tgtpos)
{
	Vector3 desiredVelocity = (tgtpos - mEntity.getPosition()).normalized();

	return desiredVelocity - mEntity.getVelocity().normalized();
}


}

