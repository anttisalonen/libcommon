#ifndef COMMON_PARTITION_H
#define COMMON_PARTITION_H

#include "Vector2.h"

namespace Common {

struct AABB {
	Vector2 center;
	Vector2 halfDimension;
	AABB(const Vector2& c, const Vector2& hd) : center(c), halfDimension(hd) { }
	inline bool contains(const Vector2& p) const;
	inline bool intersects(const AABB& b) const;
};

inline std::ostream& operator<<(std::ostream& out, const AABB& r)
{
	out << "Center: " << r.center << " - half dimension: " << r.halfDimension;
	return out;
}

bool AABB::contains(const Vector2& p) const
{
	float dx = fabs(p.x - center.x);
	float dy = fabs(p.y - center.y);
	return dx <= halfDimension.x && dy <= halfDimension.y;
}

bool AABB::intersects(const AABB& b) const
{
	float dx = fabs(b.center.x - center.x);
	float dy = fabs(b.center.y - center.y);
	return dx <= halfDimension.x + b.halfDimension.x && dy <= halfDimension.y + b.halfDimension.y;
}

struct BoundingBox3 {
	Vector3 center;
	Vector3 halfDimension;
	BoundingBox3(const Vector3& c, const Vector3& hd) : center(c), halfDimension(hd) { }
	inline bool contains(const Vector3& p) const;
	inline bool intersects(const BoundingBox3& b) const;
};

inline std::ostream& operator<<(std::ostream& out, const BoundingBox3& r)
{
	out << "Center: " << r.center << " - half dimension: " << r.halfDimension;
	return out;
}

bool BoundingBox3::contains(const Vector3& p) const
{
	float dx = fabs(p.x - center.x);
	float dy = fabs(p.y - center.y);
	float dz = fabs(p.z - center.z);
	return dx <= halfDimension.x && dy <= halfDimension.y && dz <= halfDimension.z;
}

bool BoundingBox3::intersects(const BoundingBox3& b) const
{
	float dx = fabs(b.center.x - center.x);
	float dy = fabs(b.center.y - center.y);
	float dz = fabs(b.center.z - center.z);
	return dx <= halfDimension.x + b.halfDimension.x &&
		dy <= halfDimension.y + b.halfDimension.y &&
		dz <= halfDimension.z + b.halfDimension.z;
}

}

#endif

