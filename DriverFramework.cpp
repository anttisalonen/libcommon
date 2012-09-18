#include "DriverFramework.h"

#include <stdlib.h>

#include <GL/gl.h>

#include "Clock.h"

namespace Common {

Driver::Driver(unsigned int screenWidth, unsigned int screenHeight, const char* caption)
	: mScreenWidth(screenWidth),
	mScreenHeight(screenHeight),
	mScreen(nullptr),
	mQuitting(false),
	mPaused(false),
	mDisableGUI(false),
	mFixedFrameTime(0.0f),
	mRandomise(false)
{
	if(!mScreenWidth && !mScreenHeight) {
		mDisableGUI = true;
	} else {
		mScreen = SDL_utils::initSDL(mScreenWidth, mScreenHeight, caption);
	}
}

Driver::~Driver()
{
	SDL_Quit();
}

void Driver::setFixedTime(float ticksPerSec, bool randomised)
{
	if(ticksPerSec) {
		mFixedFrameTime = 1.0f / ticksPerSec;
		mRandomise = randomised;
	} else {
		mFixedFrameTime = 0.0f;
	}
}

void Driver::run()
{
	if(!init())
		return;

	double prevTime = Clock::getTime();
	while(1) {
		double newTime = Clock::getTime();
		double frameTime = mFixedFrameTime ? mFixedFrameTime : newTime - prevTime;
		if(!isPaused() && mFixedFrameTime && mRandomise) {
			double add = rand() / (double)RAND_MAX;
			add -= 0.5f;
			add *= 0.01f * mFixedFrameTime;
			frameTime += add;
		}
		prevTime = newTime;

		if(prerenderUpdate(frameTime))
			break;

		if(!mDisableGUI && handleInput(frameTime))
			break;

		if(!mDisableGUI) {
			render();
		}

		if(postrenderUpdate(frameTime))
			break;
	}
	return;
}

unsigned int Driver::getScreenWidth() const
{
	return mScreenWidth;
}

unsigned int Driver::getScreenHeight() const
{
	return mScreenHeight;
}

void Driver::render()
{
	beginFrame();
	drawFrame();
	finishFrame();
}

void Driver::setPaused(bool p)
{
	mPaused = p;
}

bool Driver::isPaused() const
{
	return mPaused;
}

bool Driver::init()
{
	return true;
}

bool Driver::prerenderUpdate(float frameTime)
{
	return false;
}

bool Driver::postrenderUpdate(float frameTime)
{
	return false;
}

void Driver::beginFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Driver::drawFrame()
{
}

void Driver::finishFrame()
{
	SDL_GL_SwapBuffers();
}

bool Driver::handleInput(float frameTime)
{
	bool quitting = false;
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_KEYDOWN:
				if(handleKeyDown(frameTime, event.key.keysym.sym))
					quitting = true;
				break;

			case SDL_KEYUP:
				if(handleKeyUp(frameTime, event.key.keysym.sym))
					quitting = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if(handleMousePress(frameTime, event.button.button))
					quitting = true;
				break;

			case SDL_MOUSEBUTTONUP:
				if(handleMouseRelease(frameTime, event.button.button))
					quitting = true;
				break;

			case SDL_MOUSEMOTION:
				if(handleMouseMotion(frameTime, event.motion))
					quitting = true;
				break;

			case SDL_QUIT:
				if(handleQuit())
					quitting = true;
				break;

			default:
				break;
		}
	}
	return quitting;
}

bool Driver::handleKeyDown(float frameTime, SDLKey key)
{
	if(key == SDLK_ESCAPE)
		return true;

	if(key == SDLK_p) {
		setPaused(!isPaused());
	}

	return false;
}

bool Driver::handleKeyUp(float frameTime, SDLKey key)
{
	return false;
}

bool Driver::handleMouseMotion(float frameTime, const SDL_MouseMotionEvent& ev)
{
	return false;
}

bool Driver::handleMousePress(float frameTime, Uint8 button)
{
	return false;
}

bool Driver::handleMouseRelease(float frameTime, Uint8 button)
{
	return false;
}

bool Driver::handleQuit()
{
	return true;
}


}

