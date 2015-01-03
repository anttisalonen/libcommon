#include <stdlib.h>

#include "Math.h"
#include "Quaternion.h"

namespace Common {

double Math::pointToLineDistance(const Vector2& l1,
		const Vector2& l2,
		const Vector2& p)
{
	double nlen = sqrt((l2.x - l1.x) * (l2.x - l1.x) + (l2.y - l1.y) * (l2.y - l1.y));
	if(nlen)
		return fabs((l2.x - l1.x) * (l1.y - p.y) - (l1.x - p.x) * (l2.y - l1.y)) / nlen;
	else
		return (p - l1).length();
}

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

double Math::pointToSegmentDistance(const Vector2& l1,
		const Vector2& l2,
		const Vector2& p, Vector2* nearest)
{
	const float dist2 = l1.distance2(l2);
	if (dist2 == 0.0f) {
		if(nearest)
			*nearest = l1;
		return p.distance(l1);
	}
	const float t = (p - l1).dot(l2 - l1) * (1.0f / dist2);
	if (t < 0.0f) {
		if(nearest)
			*nearest = l1;
		return p.distance(l1);
	}
	else if (t > 1.0f) {
		if(nearest)
			*nearest = l2;
		return p.distance(l2);
	}
	const Vector2 projection = l1 + (l2 - l1) * t;
	if(nearest)
		*nearest = projection;
	return p.distance(projection);
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

Vector2 Math::lineLineIntersection2D(const Vector2& p1,
		const Vector2& p2,
		const Vector2& p3,
		const Vector2& p4)
{
	float x1 = p1.x; float y1 = p1.y; float x2 = p2.x; float y2 = p2.y;
	float x3 = p3.x; float y3 = p3.y; float x4 = p4.x; float y4 = p4.y;

	float denom = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	if(denom == 0.0f) {
		return Vector2();
	}
	else {
		float nom1 = (x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4);
		float nom2 = (x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4);
		return Vector2(nom1 / denom, nom2 / denom);
	}
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

Vector2 Math::segmentSegmentIntersection2D(const Vector2& p1,
		const Vector2& p2,
		const Vector2& p3,
		const Vector2& p4, bool* found)
{
	return Math::segmentSegmentIntersection2D<Vector2>(p1, p2, p3, p4, found);
}

Vector3 Math::segmentSegmentIntersection2D(const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		const Vector3& p4, bool* found)
{
	return Math::segmentSegmentIntersection2D<Vector3>(p1, p2, p3, p4, found);
}

template<typename T>
T Math::segmentSegmentIntersection2D(const T& p1,
		const T& p2,
		const T& p3,
		const T& p4, bool* found)
{
	T p = p1;
	T r = p2 - p1;
	T q = p3;
	T s = p4 - p3;

	float denom = r.cross2d(s);
	if(denom == 0.0f) {
		// parallel
		if(found)
			*found = false;
		return T();
	}

	float t = (q - p).cross2d(s) / denom;
	float u = (q - p).cross2d(r) / denom;

	if(t < 0.0f || t > 1.0f || u < 0.0f || u > 1.0f) {
		if(found)
			*found = false;
		if(t < 0.0f)
			return p;
		else
			return p + r;
	}

	if(found)
		*found = true;
	return p + r * t;
}

float Math::segmentSegmentDistance3D(const Vector3& p1,
		const Vector3& p2,
		const Vector3& p3,
		const Vector3& p4)
{
	// http://geomalgorithms.com/a07-_distance.html
	// Copyright 2001 softSurfer, 2012 Dan Sunday
	// This code may be freely used and modified for any purpose
	// providing that this copyright notice is included with it.
	// SoftSurfer makes no warranty for this code, and cannot be held
	// liable for any real or imagined damage resulting from its use.
	// Users of this code must verify correctness for their application.
	Vector3 u = p2 - p1;
	Vector3 v = p4 - p3;
	Vector3 w = p1 - p3;
	float a = u.dot(u);
	float b = u.dot(v);
	float c = v.dot(v);
	float d = u.dot(w);
	float e = v.dot(w);
	float D = a * c - b * b;
	float sc, sN, sD = D;
	float tc, tN, tD = D;

	if(D < 0.0001f) { // almost parallel
		sN = 0.0f;
		sD = 1.0f;
		tN = e;
		tD = c;
	} else {
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if(sN < 0.0f) {
			sN = 0.0f;
			tN = e;
			tD = c;
		} else if(sN > sD) {
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if(tN < 0.0f) {
		tN = 0.0f;
		if(-d < 0.0f) {
			sN = 0.0f;
		} else if(-d > a) {
			sN = sD;
		} else {
			sN = -d;
			sD = a;
		}
	} else if(tN > tD) {
		tN = tD;
		if((-d + b) < 0.0f) {
			sN = 0;
		} else if((-d + b) > a) {
			sN = sD;
		} else {
			sN = (-d + b);
			sD = a;
		}
	}

	sc = fabs(sN) < 0.001f ? 0.0f : (sN / sD);
	tc = fabs(tN) < 0.001f ? 0.0f : (tN / tD);

	Vector3 dP = w + (u * sc) - (v * tc);

	return dP.length();
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

bool Math::tps(const Vector3& pos,
		const Vector3& vel, float c, float& ret1, float& ret2)
{
	/* Given an enemy position relative to origo, the enemy velocity and
	 * the velocity of your projectile,
	 * returns the two points in time where the projectile shot by you
	 * will hit the enemy. 0-2 of the points in time may be negative.
	 * The returning bool says whether the time points were found.
	 *
	 * This is what it should look like - from starrover2:
	   tps x y p q c = ((term1 - term2) / term3, (term1 + term2) / term3)
		   where term1 = p * x + q * y
		         term2 = sqrt (c*c*x*x - q*q*x*x + 2 * p * q * x * y + c*c*y*y - p*p*y*y)
		         term3 = c*c - p*p - q*q

	   I don't know what tps means though. I should've written it down. */

	float x = pos.x;
	float y = pos.y;
	float p = vel.x;
	float q = vel.y;
	float x2 = x * x;
	float y2 = y * y;
	float p2 = p * p;
	float q2 = q * q;
	float c2 = c * c;

	float term3 = c2 - p2 - q2;

	if(term3 == 0.0f)
		return false;

	float sq = c2 * x2 - q2 * x2 + 2 * p * q * x * y + c2 * y2 - p2 * y2;
	if(sq < 0.0f)
		return false;

	float term1 = p * x + q * y;
	float term2 = sqrt (sq);

	ret1 = (term1 - term2) / term3;
	ret2 = (term1 + term2) / term3;
	return true;
}

Vector2 Math::rotate2D(const Vector2& v, float angle)
{
	Vector2 rot(v);

	rot.x = v.x * cos(angle) - v.y * sin(angle);
	rot.y = v.x * sin(angle) + v.y * cos(angle);

	return rot;
}

Vector3 Math::rotate2D(const Vector3& v, float angle)
{
	Vector3 rot(v);

	rot.x = v.x * cos(angle) - v.y * sin(angle);
	rot.y = v.x * sin(angle) + v.y * cos(angle);

	return rot;
}

Vector3 Math::rotate3D(const Vector3& v, float angle, const Vector3& axe)
{
	Quaternion q(Quaternion::fromAxisAngle(axe, angle));
	return rotate3D(v, q);
}

Vector3 Math::rotate3D(const Vector3& v, const Common::Quaternion& q)
{
	// x' = q * x * q'
	// where q = rotation quaternion
	//       q' = inverse q
	//       x = vector treated as a quaternion
	// inverse q is equivalent to conjugate of q for an unknown reason
	// (proof left as an exercise to the reader)
	auto n = v.normalized();
	auto nq = Quaternion(n.x, n.y, n.z, 0.0f);
	auto w  = q * nq * q.conjugated();

	return Common::Vector3(w.x, w.y, w.z);
}

double Math::degreesToRadians(double d)
{
	return d * PI / 180.0f;
}

double Math::radiansToDegrees(double r)
{
	return r * 180.0f / PI;
}

bool Math::raySphereIntersect(const Vector3& l1,
		const Vector3& l2,
		const Vector3& center, float radius)
{
	return segmentCircleIntersect(l1, l2, center, radius);
}

bool Math::isInsideTriangle(const Common::Vector2& p,
		const Common::Vector2& p1,
		const Common::Vector2& p2,
		const Common::Vector2& p3)
{
	// http://stackoverflow.com/a/14382692/484899
	float area = 1.0f / 2.0f * (-p2.y * p3.x + p1.y * (-p2.x + p3.x) + p1.x * (p2.y - p3.y) + p2.x * p3.y);
	float s = 1.0f / (2.0f * area) * (p1.y * p3.x - p1.x * p3.y + (p3.y - p1.y) * p.x + (p1.x - p3.x) * p.y);
	float t = 1.0f / (2.0f * area) * (p1.x * p2.y - p1.y * p2.x + (p1.y - p2.y) * p.x + (p2.x - p1.x) * p.y);
	return s > 0.0f and t > 0.0f && 1 - s - t > 0.0f;
}

}

