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
		unsigned int getScreenWidth() const;
		unsigned int getScreenHeight() const;

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
		virtual bool handleKeyDown(float frameTime, SDLKey key);
		virtual bool handleKeyUp(float frameTime, SDLKey key);
		virtual bool handleMouseMotion(float frameTime, const SDL_MouseMotionEvent& ev);
		virtual bool handleMousePress(float frameTime, Uint8 button);
		virtual bool handleMouseRelease(float frameTime, Uint8 button);
		virtual bool handleQuit();

	private:
		unsigned int mScreenWidth;
		unsigned int mScreenHeight;
		SDL_Surface* mScreen;
		bool mPaused;
		bool mDisableGUI;
		float mFixedFrameTime;
		bool mRandomise;
};

}

#endif

