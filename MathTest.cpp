#include <time.h>

#include <iostream>
#include <cstdlib>

#include "Math.h"

using namespace Common;

bool test_math_segment_segment_3d_distance(const Vector3& p,
		const Vector3& q,
		const Vector3& r,
		const Vector3& s,
		float expected)
{
	float dist = Math::segmentSegmentDistance3D(p, q, r, s);
	std::cout << "Distance: " << dist << "\n";
	if(fabs((dist - expected) / expected) > 0.05f) {
		std::cout << "Expected: " << expected << "\n";
		return false;
	}
	return true;
}

int math_segment_segment_3d_distance(int argc, char** argv)
{
	{
		// http://stackoverflow.com/a/702174/484899
		Vector3 p(-0.43256,     -1.6656,     0.12533);
		Vector3 q(0.28768,     -1.1465,      1.1909);
		Vector3 r(1.1892,   -0.037633,     0.32729);
		Vector3 s(0.17464,    -0.18671,     0.72579);

		if(!test_math_segment_segment_3d_distance(p, q, r, s, 1.071f)) {
			return 1;
		}
	}

	{
		// http://stackoverflow.com/a/18994296/484899
		Vector3 p(13.43, 21.77, 46.81);
		Vector3 q(27.83, 31.74, -26.60);
		Vector3 r(77.54, 7.53, 6.22);
		Vector3 s(26.99, 12.39, 11.18);

		if(!test_math_segment_segment_3d_distance(p, q, r, s, 15.826f)) {
			return 1;
		}
	}

	std::cout << "Success.\n";

	return 0;
}

int math_quaternion(int argc, char** argv)
{
	Vector3 v(0.0, 1.0, 0.0);
	Quaternion q(sqrt(0.5), 0.0, 0.0, sqrt(0.5)); // 90 degree rotation around X
	Vector3 v2 = q.multiply(v); // should point towards +Z
	std::cout << v2.x << " " << v2.y << " " << v2.z << "\n";
	if(fabs(v2.x - 0.0) > 0.05f) {
		return 1;
	}
	if(fabs(v2.y - 0.0) > 0.05f) {
		return 1;
	}
	if(fabs(v2.z - 1.0) > 0.05f) {
		return 1;
	}
	return 0;
}

