#include "TextRenderer.h"

#include <iostream>

namespace Common {

TextRenderer::TextRenderer(const char* fontfilename, int size)
{
	if(!TTF_WasInit() && TTF_Init() == -1) {
		std::cerr << "Could not initialise SDL_ttf: " << TTF_GetError() << "\n";
		throw std::runtime_error("Error while initialising SDL_ttf");
	}

	mFont = TTF_OpenFont(fontfilename, size);
	if(!mFont) {
		std::cerr << "Could not open font " << fontfilename << ": " << TTF_GetError() << "\n";
		throw std::runtime_error("Error while opening font");
	}
}

TextRenderer::~TextRenderer()
{
	TTF_CloseFont(mFont);
}

boost::shared_ptr<Common::Texture> TextRenderer::renderText(const char* text, const Color& color)
{
	auto f = FontConfig(text, color, 1.0f);

	auto it = mTextMap.find(f);
	if(it == mTextMap.end()) {
		SDL_Surface* textsurf;
		SDL_Color scolor = {color.r, color.g, color.b};

		textsurf = TTF_RenderUTF8_Blended(mFont, text, scolor);
		if(!textsurf) {
			std::cerr << "Could not render text: " << TTF_GetError() << "\n";
			throw std::runtime_error("Error while rendering text");
		}
		else {
			boost::shared_ptr<Texture> texture(new Texture(textsurf));
			boost::shared_ptr<TextTexture> ttexture(new TextTexture(texture, textsurf->w, textsurf->h));
			auto it2 = mTextMap.insert(std::make_pair(f, ttexture));
			it = it2.first;
			SDL_FreeSurface(textsurf);
		}

	}

	assert(it != mTextMap.end());

	return it->second->mTexture;
}

}

