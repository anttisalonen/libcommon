#include <time.h>

#include <iostream>
#include <cstdlib>

int geometry(int argc, char** argv);

int main(int argc, char** argv)
{
	int seed;
	if(argc > 1)
		seed = atoi(argv[1]);
	else
		seed = time(0);

	std::cout << "Seed: " << seed << "\n";
	srand(seed);
	return geometry(argc, argv);
}
