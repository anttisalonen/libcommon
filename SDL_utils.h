#ifndef COMMON_SDL_UTILS_H
#define COMMON_SDL_UTILS_H

#include <SDL.h>
#include <SDL_ttf.h>

#include "Vector3.h"
#include "Texture.h"
#include "Rectangle.h"
#include "FontConfig.h"
#include "Color.h"

namespace Common {

	class SDL_utils {
		public:
			static SDL_Surface* initSDL(int w, int h, const char* caption);
			static void setupOrthoScreen(int w, int h);
			static const char* GLErrorToString(GLenum err);
			static void drawSpriteWithColor(const Common::Texture& t,
					const Common::Rectangle& vertcoords,
					const Common::Rectangle& texcoords, float depth,
					const Common::Color& color, float alpha = 1.0f);
			static void drawSprite(const Common::Texture& t,
					const Common::Rectangle& vertcoords,
					const Common::Rectangle& texcoords, float depth,
					float alpha = 1.0f);
			static void drawText(TextMap& tm, TTF_Font* font, const Vector3& camera,
					float scaleLevel, int screenWidth, int screenHeight,
					float x, float y,
					const FontConfig& f,
					bool screencoordinates, bool centered);
			static void drawCircle(float x, float y, float rad);
			static void drawPoint(const Vector3& coords, float size, const Common::Color& col);
			static void drawRectangle(float x, float y, float x2, float y2,
					const Common::Color& c, float alpha, bool onlyframes);
			static void drawLine(const Common::Vector3& p1, const Common::Vector3& p2, const Common::Color& c, float alpha);
	};

}

#endif

