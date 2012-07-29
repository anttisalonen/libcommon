#ifndef COMMON_CLOCK_H
#define COMMON_CLOCK_H

namespace Common {

class Clock {
	public:
		Clock();
		double limitFPS(int fps);
		static double getTime();
	private:
		double mLastTime;
		double mStatTime;
		int mFrames;
};

class Countdown {
	public:
		Countdown(float from);
		void doCountdown(float howmuch);
		bool checkAndRewind();
		void rewind();
		bool check();
		bool running() const;
		float timeLeft() const;
		void clear();
	private:
		float mFrom;
		float mNow;
		bool mChecked;
};

class SteadyTimer {
	public:
		SteadyTimer(float steptime);
		bool check(float elapsedtime);

	private:
		float mStepTime;
		float mLeftTime;
};

}

#endif
