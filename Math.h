#ifndef MATH_H
#define MATH_H

#include <algorithm>

#include "Vector3.h"

#define PI 3.1415926535
#define TWO_PI 6.283185306
#define HALF_PI 1.570796326
#define QUARTER_PI 0.785398163

namespace Common {

template <typename T>
T signum(T v)
{
	if(v < 0)
		return -1;
	else if(v > 0)
		return 1;
	else
		return 0;
}

template <typename T>
T clamp(T minv, T v, T maxv)
{
	return std::min(std::max(minv, v), maxv);
}

class Math {
	public:
		static double pointToLineDistance(const Vector3& l1,
				const Vector3& l2,
				const Vector3& p);
		static double pointToSegmentDistance(const Vector3& l1,
				const Vector3& l2,
				const Vector3& p, Vector3* nearest = nullptr);
		static Vector3 lineLineIntersection2D(const Vector3& p1,
				const Vector3& p2,
				const Vector3& p3,
				const Vector3& p4);
		static Vector3 segmentSegmentIntersection2D(const Vector3& p1,
				const Vector3& p2,
				const Vector3& p3,
				const Vector3& p4, bool* found);
		static bool lineCircleIntersect(const Vector3& l1,
				const Vector3& l2,
				const Vector3& c, float radius);
		static bool segmentCircleIntersect(const Vector3& l1,
				const Vector3& l2,
				const Vector3& c, float radius);
		static bool tps(const Vector3& pos,
				const Vector3& vel, float c, float& ret1, float& ret2);
};

}

#endif

