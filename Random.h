#ifndef COMMON_RANDOM_H
#define COMMON_RANDOM_H

#include <random>

namespace Common {

class RandGen {
	public:
		RandGen(unsigned int seed = 0);
		float clamped(); // between -1 and 1
		float uniform(); // between 0 and 1
		float uniform(float a, float b); // between a and b
		unsigned int uniform(unsigned int i, unsigned int j); // between i and j - 1

	private:
		std::default_random_engine mMyGen;
};

class Random {
	public:
		static void seed(unsigned int i);
		static float clamped(); // between -1 and 1
		static float uniform(); // between 0 and 1
		static float uniform(float a, float b); // between a and b
		static unsigned int uniform(unsigned int i, unsigned int j); // between i and j - 1

	private:
		static RandGen mGen;
};

}

#endif

