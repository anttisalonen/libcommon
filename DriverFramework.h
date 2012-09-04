#ifndef COMMON_DRIVERFRAMEWORK_H
#define COMMON_DRIVERFRAMEWORK_H

#include "SDL_utils.h"

namespace Common {

class Driver {
	public:
		Driver(unsigned int screenWidth, unsigned int screenHeight, const char* caption);
		virtual ~Driver();
		void setFixedTime(float ticksPerSec, bool randomised);
		void run();

	protected:
		virtual void render();
		virtual void setPaused(bool p);
		virtual bool isPaused() const;
		virtual bool init();
		virtual bool prerenderUpdate(float frameTime);
		virtual bool postrenderUpdate(float frameTime);
		virtual void beginFrame();
		virtual void drawFrame();
		virtual void finishFrame();
		virtual bool handleInput(float frameTime);
		virtual bool handleKeyDown(SDLKey key);
		virtual bool handleKeyUp(SDLKey key);
		virtual bool handleMouseMotion(const SDL_MouseMotionEvent& ev);
		virtual bool handleMousePress(Uint8 button);
		virtual bool handleMouseRelease(Uint8 button);
		virtual bool handleQuit();

	private:
		unsigned int mScreenWidth;
		unsigned int mScreenHeight;
		SDL_Surface* mScreen;
		bool mQuitting;
		bool mPaused;
		bool mDisableGUI;
		float mFixedFrameTime;
		bool mRandomise;
};

}

#endif

