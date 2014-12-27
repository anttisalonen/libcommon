#ifndef MATH_H
#define MATH_H

#include <algorithm>

#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

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
		static double pointToLineDistance(const Vector2& l1,
				const Vector2& l2,
				const Vector2& p);
		static double pointToLineDistance(const Vector3& l1,
				const Vector3& l2,
				const Vector3& p);
		static double pointToSegmentDistance(const Vector2& l1,
				const Vector2& l2,
				const Vector2& p, Vector2* nearest = nullptr);
		static double pointToSegmentDistance(const Vector3& l1,
				const Vector3& l2,
				const Vector3& p, Vector3* nearest = nullptr);
		static Vector2 lineLineIntersection2D(const Vector2& p1,
				const Vector2& p2,
				const Vector2& p3,
				const Vector2& p4);
		static Vector3 lineLineIntersection2D(const Vector3& p1,
				const Vector3& p2,
				const Vector3& p3,
				const Vector3& p4);
		static Vector3 segmentSegmentIntersection2D(const Vector3& p1,
				const Vector3& p2,
				const Vector3& p3,
				const Vector3& p4, bool* found);
		static float segmentSegmentDistance3D(const Vector3& p1,
				const Vector3& p2,
				const Vector3& p3,
				const Vector3& p4);
		static bool lineCircleIntersect(const Vector3& l1,
				const Vector3& l2,
				const Vector3& c, float radius);
		static bool segmentCircleIntersect(const Vector3& l1,
				const Vector3& l2,
				const Vector3& c, float radius);
		static bool raySphereIntersect(const Vector3& l1,
				const Vector3& l2,
				const Vector3& center, float radius);
		static int solveQuadratic(float a, float b, float c, float& r1, float& r2);
		static bool tps(const Vector3& pos,
				const Vector3& vel, float c, float& ret1, float& ret2);
		static Vector2 rotate2D(const Vector2& v, float angle);
		static Vector3 rotate2D(const Vector3& v, float angle);
		static Vector3 rotate3D(const Vector3& v, float angle, const Vector3& axe);
		static Vector3 rotate3D(const Vector3& v, const Common::Quaternion& q);
		static double degreesToRadians(double d);
		static double radiansToDegrees(double r);
};

}

#endif

