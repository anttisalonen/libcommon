#include <sys/time.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <iostream>
#include <algorithm>

#include "Clock.h"
#include "Random.h"

namespace Common {

Clock::Clock()
	: mFrames(0)
{
	mLastTime = getTime();
	mStatTime = mLastTime;
}

double Clock::limitFPS(int fps, bool output)
{
	double newtime = getTime();
	double maxadv = 1.0f / fps;
	double diff = newtime - mLastTime;
	if(maxadv > diff) {
		usleep((maxadv - diff) * 1000000);
		mLastTime = getTime();
	}
	else {
		mLastTime = newtime;
	}
	mFrames++;
	if(output && newtime - mStatTime >= 2.0f) {
		std::cout << "FPS: " << mFrames / (newtime - mStatTime) << "\n";
		mStatTime = newtime;
		mFrames = 0;
	}
	return std::min(diff, maxadv);
}

double Clock::getTime()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + (double)(tv.tv_usec / 1000000.0f);
}

Countdown::Countdown(float from)
	: mFrom(from),
	mNow(from),
	mChecked(true)
{
}

void Countdown::doCountdown(float howmuch)
{
	if(mNow > 0.0f)
		mNow -= howmuch;
}

bool Countdown::checkAndRewind()
{
	if(check()) {
		rewind();
		return true;
	}
	else {
		return false;
	}
}

bool Countdown::countdownAndRewind(float howmuch)
{
	doCountdown(howmuch);
	return checkAndRewind();
}

void Countdown::rewind()
{
	mNow = mFrom;
	mChecked = false;
}

bool Countdown::check()
{
	bool done = mNow <= 0.0f;
	if(done) {
		mChecked = true;
	}
	return done;
}

bool Countdown::running() const
{
	return !mChecked;
}

float Countdown::timeLeft() const
{
	return std::max(0.0f, mNow);
}

void Countdown::clear()
{
	mNow = 0.0f;
	mChecked = true;
}

float Countdown::getMaxTime() const
{
	return mFrom;
}

SteadyTimer::SteadyTimer(float steptime)
	: mStepTime(steptime)
{
	mLeftTime = Random::uniform() * mStepTime;
}

bool SteadyTimer::check(float elapsedtime)
{
	mLeftTime -= elapsedtime;
	if(mLeftTime <= 0.0f) {
		mLeftTime += mStepTime;
		if(mLeftTime <= 0.0f) {
			int steps = fabs((mLeftTime - mStepTime) / mStepTime);
			mLeftTime += steps * mStepTime;
			assert(mLeftTime >= 0.0f && mLeftTime <= mStepTime);
		}
		return true;
	}
	else {
		return false;
	}
}

}

