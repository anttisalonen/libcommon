#ifndef COMMON_LINE_H
#define COMMON_LINE_H

#include <list>

namespace Common {

struct Point2 {
	Point2(int _x = 0, int _y = 0) : x(_x), y(_y) { }
	int x;
	int y;
};

class Line {
	public:
		static std::list<Point2> line(const Point2& from, const Point2& to);
};

}

#endif
