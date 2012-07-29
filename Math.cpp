#include <stdlib.h>

#include "Math.h"

namespace Common {

double Math::pointToLineDistance(const Vector3& l1,
		const Vector3& l2,
		const Vector3& p)
{
	double nlen = sqrt((l2.x - l1.x) * (l2.x - l1.x) + (l2.y - l1.y) * (l2.y - l1.y));
	if(nlen)
		return fabs((l2.x - l1.x) * (l1.y - p.y) - (l1.x - p.x) * (l2.y - l1.y)) / nlen;
	else
		return (p - l1).length();
}

double Math::pointToSegmentDistance(const Vector3& l1,
		const Vector3& l2,
		const Vector3& p, Vector3* nearest)
{
	// http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
	// Return minimum distance between line segment l1w and point p
	const float dist2 = l1.distance2(l2); // i.e. |l2-l1|^2 -  avoid a sqrt
	if (dist2 == 0.0f) {
		if(nearest)
			*nearest = l1;
		return p.distance(l1);   // l1 == l2 case
	}
	// Consider the line extending the segment, parameterized as l1 + t (l2 - l1).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-l1) . (l2-l1)] / |l2-l1|^2
	const float t = (p - l1).dot(l2 - l1) * (1.0f / dist2);
	if (t < 0.0f) {
		if(nearest)
			*nearest = l1;
		return p.distance(l1);       // Beyond the 'l1' end of the segment
	}
	else if (t > 1.0f) {
		if(nearest)
			*nearest = l2;
		return p.distance(l2);  // Beyond the 'l2' end of the segment
	}
	const Vector3 projection = l1 + (l2 - l1) * t;  // Projection falls on the segment
	if(nearest)
		*nearest = projection;
	return p.distance(projection);
}

Vector3 Math::lineLineIntersection2D(const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		const Vector3& p4)
{
	float x1 = p1.x; float y1 = p1.y; float x2 = p2.x; float y2 = p2.y;
	float x3 = p3.x; float y3 = p3.y; float x4 = p4.x; float y4 = p4.y;

	float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if(denom == 0.0f) {
		return Vector3();
	}
	else {
		float nom1 = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
		float nom2 = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
		return Vector3(nom1 / denom, nom2 / denom, 0);
	}
}

Vector3 Math::segmentSegmentIntersection2D(const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		const Vector3& p4, bool* found)
{
	Vector3 p = p1;
	Vector3 r = p2 - p1;
	Vector3 q = p3;
	Vector3 s = p4 - p3;

	float denom = r.cross2d(s);
	if(denom == 0.0f) {
		// parallel
		if(found)
			*found = false;
		return Vector3();
	}

	float t = (q - p).cross2d(s) / denom;
	if(t < 0.0f || t > 1.0f) {
		if(found)
			*found = false;
		return Vector3();
	}

	if(found)
		*found = true;
	return p + r * t;
}

bool Math::lineCircleIntersect(const Vector3& l1,
		const Vector3& l2,
		const Vector3& c, float radius)
{
	float D = l1.x * l2.y - l2.x * l1.y;
	float dx = l2.x - l1.x;
	float dy = l2.y - l1.y;
	float dr = sqrt(dx * dx + dy * dy);
	float disc = radius * radius * dr * dr - D * D;
	return disc > 0.0f;
}

bool Math::segmentCircleIntersect(const Vector3& l1,
		const Vector3& l2,
		const Vector3& c, float radius)
{
	float f = pointToSegmentDistance(l1, l2, c, nullptr);
	return f < radius;
}

}

