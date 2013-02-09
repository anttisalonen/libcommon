#include <climits>

#include "Geometry.h"
#include "Math.h"

using namespace Common;

// http://en.wikipedia.org/wiki/Graham_scan
// Three points are a counter-clockwise turn if ccw > 0, clockwise if
// ccw < 0, and collinear if ccw = 0 because ccw is a determinant that
// gives the signed area of the triangle formed by p1, p2 and p3.
static int counterClockwise(const IPoint& p1, const IPoint& p2, const IPoint& p3)
{
	return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

const Polygon& Polygon::getConvexHull() const
{
	if(mIsConvex)
		return *this;

	if(isEmpty() || mPoints.size() < 3) {
		return *this;
	}

	if(mConvexHull) {
		return *mConvexHull;
	}

	if(mPoints.size() == 3) {
		mConvexHull = new Polygon(mPoints, true);
		return *mConvexHull;
	}

	std::vector<IPoint> points = mPoints;
	std::sort(points.begin(), points.end());

	std::vector<IPoint> upper;
	upper.push_back(points[0]);
	upper.push_back(points[1]);

	for(unsigned int i = 2; i < points.size(); i++) {
		upper.push_back(points[i]);
		while(upper.size() > 2) {
			const IPoint& p1 = upper.rbegin()[0];
			const IPoint& p2 = upper.rbegin()[1];
			const IPoint& p3 = upper.rbegin()[2];
			if(counterClockwise(p1, p2, p3) < 0) {
				upper.erase(upper.end() - 2);
			} else {
				break;
			}
		}
	}

	std::vector<IPoint> lower;
	lower.push_back(points.rbegin()[0]);
	lower.push_back(points.rbegin()[1]);

	for(unsigned int i = points.size() - 3; i > 0; i--) {
		lower.push_back(points[i]);
		while(lower.size() > 2) {
			const IPoint& p1 = lower.rbegin()[0];
			const IPoint& p2 = lower.rbegin()[1];
			const IPoint& p3 = lower.rbegin()[2];
			if(counterClockwise(p1, p2, p3) < 0) {
				lower.erase(lower.end() - 2);
			} else {
				break;
			}
		}
	}

	// remove the first and last from lower to avoid duplication
	lower.erase(lower.end() - 1);
	upper.insert(upper.end(), lower.begin() + 1, lower.end());

	mConvexHull = new Polygon(upper, true);
	return *mConvexHull;
}

bool Polygon::pointInPolygon(const IPoint& p) const
{
	long64 minX = LONG_MAX;
	long64 minY = LONG_MAX;
	long64 maxX = LONG_MIN;
	long64 maxY = LONG_MIN;

	for(unsigned int i = 0; i < mPoints.size(); i++) {
		if(mPoints[i].x < minX)
			minX = mPoints[i].x;
		if(mPoints[i].x > maxX)
			maxX = mPoints[i].x;
		if(mPoints[i].y < minY)
			minY = mPoints[i].y;
		if(mPoints[i].y > maxY)
			maxY = mPoints[i].y;
	}

	if (p.x < minX || p.x > maxX || p.y < minY || p.y > maxY) {
		return false;
	}

	// http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	int i, j;
	bool c = false;
	for (i = 0, j = mPoints.size() - 1; i < mPoints.size(); j = i++) {
		if (((mPoints[i].y > p.y) != (mPoints[j].y > p.y)) &&
				(p.x <
				 (mPoints[j].x - mPoints[i].x) * (p.y - mPoints[i].y) /
				 (mPoints[j].y - mPoints[i].y) + mPoints[i].x)) {
			c = !c;
		}
	}

	return c;
}

bool Polygon::isSimple() const
{
	if(mIsSimple == Tristate::True)
		return true;
	else if(mIsSimple == Tristate::False)
		return false;

	bool s = true;

	if(mPoints.size() != 3) {
		std::vector<std::pair<IPoint, IPoint>> segments;
		int i, j;

		for (i = 0, j = mPoints.size() - 1; i < mPoints.size(); j = i++) {
			segments.push_back({mPoints[j], mPoints[i]});
		}

		for(i = 0; s && i < segments.size(); i++) {
			for(j = 0; s && j < segments.size(); j++) {
				if(i == j)
					continue;

				bool found;
				Vector3 a(segments[i].first.x,  segments[i].first.y,  0.0f);
				Vector3 b(segments[i].second.x, segments[i].second.y, 0.0f);
				Vector3 c(segments[j].first.x,  segments[j].first.y,  0.0f);
				Vector3 d(segments[j].second.x, segments[j].second.y, 0.0f);
				(void)Math::segmentSegmentIntersection2D(a, b, c, d, &found);
				if(found) {
					s = false;
				}
			}
		}
	}

	mIsSimple = s ? Tristate::True : Tristate::False;

	return s;
}


