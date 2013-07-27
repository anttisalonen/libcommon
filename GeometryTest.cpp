#include <time.h>

#include <iostream>
#include <cstdlib>

#include "Geometry.h"

using namespace Common;

static IPoint getRandomPoint()
{
	IPoint point;
	point.x = rand() % 10000 - 5000;
	point.y = rand() % 10000 - 5000;
	return point;
}

int geometry(int argc, char** argv)
{
	for(int i = 0; i < 1000; i++) {
		unsigned int numVertices = rand() % 10 + 3;
		Polygon polygon;

		for(unsigned int j = 0; j < numVertices; j++) {
			polygon.addPoint(getRandomPoint());
		}

		if(!polygon.isSimple()) {
			i--;
			continue;
		}

		Polygon convexHull = polygon.getConvexHull();

		for(unsigned int j = 0; j < 1000; j++) {
			IPoint p1 = getRandomPoint();
			IPoint p2 = getRandomPoint();
			bool p1in = convexHull.pointInPolygon(p1);
			bool p2in = convexHull.pointInPolygon(p2);
			if(p1in && p2in) {
				IPoint mp;
				mp.x = (p1.x + p2.x) / 2;
				mp.y = (p1.y + p2.y) / 2;
				bool mpin = convexHull.pointInPolygon(mp);
				if(mpin != p1in) {
					std::cerr << "Error after " << i << " tests. Convex hull not convex.\n";
					std::cerr << "polygon: " << polygon << "\n";
					std::cerr << "convexHull: " << convexHull << "\n";
					std::cerr << "p1: " << p1 << "\n";
					std::cerr << "p2: " << p2 << "\n";
					std::cerr << "mp: " << mp << "\n";
					std::cerr << "p1in: " << p1in << "\n";
					std::cerr << "p2in: " << p2in << "\n";
					std::cerr << "mpin: " << mpin << "\n";
					return 1;
				}
			}

			if(polygon.pointInPolygon(p1) && !p1in) {
				std::cerr << "Error after " << i << " tests. Point within polygon but not within its convex hull.\n";
				std::cerr << "polygon: " << polygon << "\n";
				std::cerr << "convexHull: " << convexHull << "\n";
				std::cerr << "p1: " << p1 << "\n";
				return 1;
			}
		}
	}

	std::cout << "Successfully passed 1000 tests.\n";

	return 0;
}
