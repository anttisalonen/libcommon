#include <stdlib.h>

#include "QuadTree.h"
#include "LineQuadTree.h"

using namespace Common;

static Vector2 getRandomPoint()
{
	return Vector2((rand() % 100000) / 1000,
			(rand() % 100000) / 1000);
}

static AABB getRandomLine()
{
	auto p1 = Vector2((rand() % 100000) / 1000,
			(rand() % 100000) / 1000);
	auto p2 = Vector2((rand() % 100000) / 2000,
			(rand() % 100000) / 2000);
	return AABB(p1, p2);
}

int quadtree(int argc, char** argv)
{
	for(int i = 0; i < 1000; i++) {
		QuadTree<int> points(AABB(Vector2(0, 0), Vector2(500, 500)));
		int numPoints = 10 + rand() % 10 * 100;
		for(int j = 0; j < numPoints; j++) {
			bool ret = points.insert(j, getRandomPoint());
			assert(ret);
		}

		int reportedSize = points.size();

		int countedSize = 0;
		for(auto it = points.begin(); it != points.end(); ++it) {
			countedSize++;
		}

		if(reportedSize != numPoints) {
			printf("Quadtree: reported size %d, expected size %d\n",
					reportedSize, numPoints);
			return 1;
		}

		if(countedSize != numPoints) {
			printf("Quadtree: counted size %d, expected size %d\n",
					countedSize, numPoints);
			return 1;
		}
	}

	printf("Successfully passed 1000 tests.\n");
	return 0;
}

int linequadtree(int argc, char** argv)
{
	for(int i = 0; i < 1000; i++) {
		LineQuadTree<int> points(AABB(Vector2(0, 0), Vector2(500, 500)));
		int numPoints = 10 + rand() % 10 * 100;
		for(int j = 0; j < numPoints; j++) {
			bool ret = points.insert(j, getRandomLine());
			assert(ret);
		}

		int reportedSize = points.size();

		int countedSize = 0;
		for(auto it = points.begin(); it != points.end(); ++it) {
			countedSize++;
		}

		if(reportedSize != numPoints) {
			printf("Quadtree: reported size %d, expected size %d\n",
					reportedSize, numPoints);
			return 1;
		}

		if(countedSize != numPoints) {
			printf("Quadtree: counted size %d, expected size %d\n",
					countedSize, numPoints);
			return 1;
		}
	}

	printf("Successfully passed 1000 tests.\n");
	return 0;
}


