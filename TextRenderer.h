#ifndef COMMON_TEXTRENDERER_H
#define COMMON_TEXTRENDERER_H

#include <boost/shared_ptr.hpp>

#include <SDL_ttf.h>

#include "Texture.h"
#include "Color.h"
#include "FontConfig.h"

namespace Common {

class TextRenderer {
	public:
		TextRenderer(const char* fontfilename, int size);
		~TextRenderer();
		boost::shared_ptr<Common::Texture> renderText(const char* text, const Color& color);

	private:
		TTF_Font* mFont;
		Common::TextMap mTextMap;
};

}

#endif

