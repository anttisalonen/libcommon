#ifndef COMMON_PARTITION_H
#define COMMON_PARTITION_H

namespace Common {

struct Point {
	float x;
	float y;
	Point() : x(0), y(0) { }
	Point(float x_, float y_) : x(x_), y(y_) { }
};

inline std::ostream& operator<<(std::ostream& out, const Point& r)
{
	out << "(" << r.x << ", " << r.y << ")";
	return out;
}

struct AABB {
	Point center;
	Point halfDimension;
	AABB(const Point& c, const Point& hd) : center(c), halfDimension(hd) { }
	inline bool contains(const Point& p) const;
	inline bool intersects(const AABB& b) const;
};

inline std::ostream& operator<<(std::ostream& out, const AABB& r)
{
	out << "Center: " << r.center << " - half dimension: " << r.halfDimension;
	return out;
}

bool AABB::contains(const Point& p) const
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

}

#endif

