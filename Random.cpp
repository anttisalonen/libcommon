#include "Random.h"

#include <stdlib.h>
#include <cassert>

namespace Common {

float Random::clamped()
{
	float r = uniform();
	r *= 2.0f;
	r -= 1.0f;
	return r;
}

float Random::uniform()
{
	float r = rand() / (float)RAND_MAX;
	return r;
}

unsigned int Random::uniform(unsigned int i, unsigned int j)
{
	assert(j > i);
	unsigned int range = j - i;
	unsigned int x = rand() % range;
	return i + x;
}

}
