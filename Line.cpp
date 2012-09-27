#include "Line.h"

#include <cstdlib>

namespace Common {

/* Bresenham's algorithm */
std::list<Point2> Line::line(const Point2& from, const Point2& to)
{
	std::list<Point2> ret;
	bool rev = false;
	int x0 = from.x;
	int y0 = from.y;
	int x1 = to.x;
	int y1 = to.y;

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if(steep) {
		std::swap(x0, y0);
		std::swap(x1, y1);
	}
	if(x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
		rev = true;
	}
	int d_x = x1 - x0;
	int d_y = abs(y1 - y0);
	int error = d_x / 2;
	int ystep;
	int y = y0;
	if(y0 < y1)
		ystep = 1;
	else
		ystep = -1;
	for(int x = x0; x <= x1; x++) {
		if(steep) {
			if(rev)
				ret.push_front(Point2(y, x));
			else
				ret.push_back(Point2(y, x));
		} else {
			if(rev)
				ret.push_front(Point2(x, y));
			else
				ret.push_back(Point2(x, y));
		}
		error -= d_y;
		if(error < 0) {
			y += ystep;
			error += d_x;
		}
	}
	return ret;
}

}

