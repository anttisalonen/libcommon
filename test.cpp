#include <time.h>

#include <iostream>
#include <cstdlib>

int math_segment_segment_3d_distance(int argc, char** argv);
int geometry(int argc, char** argv);
int quadtree(int argc, char** argv);
int linequadtree(int argc, char** argv);
int math_quaternion(int argc, char** argv);

int main(int argc, char** argv)
{
	int seed;
	if(argc > 1)
		seed = atoi(argv[1]);
	else
		seed = time(0);

	std::cout << "Seed: " << seed << "\n";
	srand(seed);

	bool failed = false;

	if(geometry(argc, argv)) {
		std::cerr << "Geometry test failed.\n";
		failed = true;
	}

	if(quadtree(argc, argv)) {
		std::cerr << "Quadtree test failed.\n";
		failed = true;
	}

	if(linequadtree(argc, argv)) {
		std::cerr << "Quadtree test failed.\n";
		failed = true;
	}

	if(math_segment_segment_3d_distance(argc, argv)) {
		std::cerr << "Math segment-segment 3D distance test failed.\n";
		failed = true;
	}

	if(math_quaternion(argc, argv)) {
		std::cerr << "Math quaternion test failed.\n";
		failed = true;
	}

	return failed ? 1 : 0;
}
