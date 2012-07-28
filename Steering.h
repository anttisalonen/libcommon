#ifndef COMMON_STEERING_H
#define COMMON_STEERING_h

#include "Vector3.h"
#include "Entity.h"

namespace Common {

class Steering {
	public:
		Steering(const Entity& e);
		Vector3 seek(const Vector3& tgtpos);

	private:
		const Entity& mEntity;
};

}

#endif

