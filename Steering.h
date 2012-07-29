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
		Vector3 pursuit(const Vehicle& tgt);
		Vector3 evade(const Vehicle& threat);
		Vector3 wander();
		Vector3 obstacleAvoidance(const std::vector<Obstacle*> obstacles);
		Vector3 wallAvoidance(const std::vector<Wall*> walls);
		bool accumulate(Vector3& runningTotal, const Vector3& add);

	private:
		const Vehicle& mUnit;
		float mWanderRadius;
		float mWanderDistance;
		float mWanderJitter;
		Vector3 mWanderTarget;
};

}

#endif

