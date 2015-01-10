#include "Random.h"

#include <stdlib.h>
#include <cassert>

namespace Common {

RandGen Random::mGen(0);

void Random::seed(unsigned int i)
{
	mGen = RandGen(i);
}

float Random::clamped()
{
	return mGen.clamped();
}

float Random::uniform()
{
	return mGen.uniform();
}

float Random::uniform(float a, float b)
{
	return mGen.uniform(a, b);
}

unsigned int Random::uniform(unsigned int i, unsigned int j)
{
	return mGen.uniform(i, j);
}


RandGen::RandGen(unsigned int seed)
{
	mMyGen = std::default_random_engine(seed);
}

float RandGen::clamped()
{
	std::uniform_real_distribution<float> cl(-1.0f, 1.0f);
	auto v = cl(mMyGen);
	return v;
}

float RandGen::uniform()
{
	std::uniform_real_distribution<float> cl(0.0f, 1.0f);
	auto v = cl(mMyGen);
	return v;
}

float RandGen::uniform(float a, float b)
{
	std::uniform_real_distribution<float> cl(a, b);
	return cl(mMyGen);
}

unsigned int RandGen::uniform(unsigned int i, unsigned int j)
{
	std::uniform_int_distribution<unsigned int> cl(i, j - 1);
	return cl(mMyGen);
}



}
