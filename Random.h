#ifndef COMMON_RANDOM_H
#define COMMON_RANDOM_H

namespace Common {

class Random {
	public:
		static float clamped(); // between -1 and 1
		static float uniform(); // between 0 and 1
		static unsigned int uniform(unsigned int i, unsigned int j); // between i and j - 1
};

}

#endif

