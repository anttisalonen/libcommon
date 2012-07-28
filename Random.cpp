#include "Random.h"

#include <stdlib.h>

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


}
