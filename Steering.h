#ifndef COMMON_STEERING_H
#define COMMON_STEERING_h

#include "Vector3.h"
#include "Vehicle.h"

namespace Common {

class Steering {
	public:
		Steering(const Vehicle& e);
		Vector3 seek(const Vector3& tgtpos);
		Vector3 flee(const Vector3& threatpos);
		Vector3 arrive(const Vector3& tgtpos);

	private:
		const Vehicle& mUnit;
};

}

#endif

