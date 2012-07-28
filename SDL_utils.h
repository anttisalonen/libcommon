#ifndef COMMON_SDL_UTILS_H
#define COMMON_SDL_UTILS_H

#include <SDL.h>

#include "Vector3.h"
#include "Texture.h"
#include "Rectangle.h"
#include "FontConfig.h"

namespace Common {

	class SDL_utils {
		public:
			static SDL_Surface* initSDL(int w, int h, const char* caption);
			static void setupOrthoScreen(int w, int h);
			static const char* GLErrorToString(GLenum err);
			static void drawSprite(const Common::Texture& t,
					const Common::Rectangle& vertcoords,
					const Common::Rectangle& texcoords, float depth);
			static void drawText(TextMap& tm, TTF_Font* font, const Vector3& camera,
					float scaleLevel, int screenWidth, int screenHeight,
					float x, float y,
					const FontConfig& f,
					bool screencoordinates, bool centered);
			static void drawCircle(float x, float y, float rad);
	};

}

#endif

