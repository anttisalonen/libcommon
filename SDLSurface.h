#ifndef SDLSURFACE_H
#define SDLSURFACE_H

#include <map>
#include <algorithm>

#include <SDL_image.h>

#include "Color.h"

namespace Common {

class SDLSurface {
	public:
		SDLSurface(const char* filename);
		SDLSurface(const SDLSurface& s);
		~SDLSurface();
		SDLSurface& operator=(const SDLSurface& s);
		const SDL_Surface* getSurface() const;
		SDL_Surface* getSurface();
		void changePixelColor(const Color& from,
				const Color& to);
		void changePixelColors(const std::map<Color, Color>& mapping);
		void mapPixelColor(std::function<Color (const Color&)> mapping);
		void blitOnTop(const SDLSurface& oth);

	private:
		SDL_Surface* mSurface;
};

}

#endif
