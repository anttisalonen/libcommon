#ifndef COMMON_GEOMETRY_H
#define COMMON_GEOMETRY_H

#include <vector>
#include <algorithm>
#include <iostream>

namespace Common {

typedef long long long64;

struct IPoint {
	long64 x = 0;
	long64 y = 0;

	inline bool operator==(const IPoint& rhs) const;
	inline bool operator!=(const IPoint& rhs) const;
	inline bool operator<(const IPoint& f) const;
};

bool IPoint::operator==(const IPoint& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

bool IPoint::operator!=(const IPoint& rhs) const
{
	return !(*this == rhs);
}

bool IPoint::operator<(const IPoint& f) const
{
	if(x != f.x)
		return x < f.x;
	return y < f.y;
}

inline std::ostream& operator<<(std::ostream& out, const IPoint& p)
{
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}

enum class Tristate {
	True,
	False,
	Unknown
};

class Polygon {
	public:
		inline Polygon(bool guaranteedConvex = false);
		inline Polygon(const std::vector<IPoint>& points, bool guaranteedConvex = false);
		inline ~Polygon();
		inline void addPoint(const IPoint& p);
		inline void removePoint(const IPoint& p);
		inline void clear();
		inline void setPoints(const std::vector<IPoint>& p);
		inline const std::vector<IPoint>& getPoints() const;
		const Polygon& getConvexHull() const;
		inline unsigned int getNumPoints() const;
		inline bool isEmpty() const;
		inline bool isConvex() const; // may trigger convex hull calculation
		bool pointInPolygon(const IPoint& p) const;
		bool isSimple() const; // O(n^2) for now

	private:
		inline void invalidateConvexHull() const;
		inline bool hasConvexHullCached() const;
		Polygon& operator=(const Polygon& o);

		std::vector<IPoint> mPoints;
		mutable Polygon* mConvexHull;
		mutable bool mIsConvex;
		mutable Tristate mIsSimple;
};

Polygon::Polygon(bool guaranteedConvex)
	: mConvexHull(nullptr),
	mIsConvex(guaranteedConvex),
	mIsSimple(Tristate::Unknown)
{
}

Polygon::Polygon(const std::vector<IPoint>& points, bool guaranteedConvex)
	: mPoints(points),
	mConvexHull(nullptr),
	mIsConvex(guaranteedConvex)
{
}

Polygon::~Polygon()
{
	delete mConvexHull;
}

void Polygon::addPoint(const IPoint& p)
{
	invalidateConvexHull();
	mPoints.push_back(p);
}

void Polygon::removePoint(const IPoint& p)
{
	invalidateConvexHull();
	mPoints.erase(std::remove(mPoints.begin(), mPoints.end(), p), mPoints.end());
}

inline void Polygon::clear()
{
	invalidateConvexHull();
	mPoints.clear();
}

inline void Polygon::setPoints(const std::vector<IPoint>& p)
{
	invalidateConvexHull();
	mPoints = p;
}

const std::vector<IPoint>& Polygon::getPoints() const
{
	return mPoints;
}

unsigned int Polygon::getNumPoints() const
{
	return mPoints.size();
}

bool Polygon::isEmpty() const
{
	return mPoints.size() == 0;
}

inline bool Polygon::isConvex() const
{
	return mIsConvex || getConvexHull().getNumPoints() == getNumPoints();
}

inline void Polygon::invalidateConvexHull() const
{
	mIsConvex = false;
	delete mConvexHull;
}

inline bool Polygon::hasConvexHullCached() const
{
	return mConvexHull;
}

inline std::ostream& operator<<(std::ostream& out, const Polygon& p)
{
	out << "[";
	auto points = p.getPoints();
	for(unsigned int i = 0; i < points.size(); i++) {
		out << points[i];
		if(i != points.size() - 1)
			out << ", ";
	}
	out << "]";
	return out;
}

}

#endif

