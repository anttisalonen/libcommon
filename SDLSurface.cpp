#include <stdexcept>
#include <sstream>

#include "SDLSurface.h"

namespace Common {

SDLSurface::SDLSurface(const char* filename)
{
	mSurface = IMG_Load(filename);
	if(!mSurface) {
		std::stringstream ss;
		ss << "Could not load image " << filename << ": " << SDL_GetError();
		throw std::runtime_error(ss.str());
	}
}

SDLSurface::~SDLSurface()
{
	SDL_FreeSurface(mSurface);
}

SDLSurface::SDLSurface(const SDLSurface& s)
{
	SDL_Surface* ss = const_cast<SDL_Surface*>(s.getSurface());
	mSurface = SDL_ConvertSurface(ss, ss->format,
			ss->flags);
}

SDLSurface& SDLSurface::operator=(const SDLSurface& s)
{
	if(this != &s) {
		SDL_Surface* ss = const_cast<SDL_Surface*>(s.getSurface());
		SDL_Surface* old = mSurface;
		SDL_Surface* newsurf = SDL_ConvertSurface(ss,
				ss->format,
				ss->flags);
		SDL_FreeSurface(old);
		mSurface = newsurf;
	}
	return *this;
}

const SDL_Surface* SDLSurface::getSurface() const
{
	return mSurface;
}

SDL_Surface* SDLSurface::getSurface()
{
	return mSurface;
}

void SDLSurface::changePixelColor(const Color& from,
		const Color& to)
{
	changePixelColors({ { from, to } });
}

void SDLSurface::changePixelColors(const std::map<Color, Color>& mapping)
{
	return mapPixelColor([&] (const Color& c) -> Color {
			auto it = mapping.find(c);
			if(it != mapping.end())
				return it->second;
			else
				return c;
			});
}

// http://www.libsdl.org/cgi/docwiki.cgi/Pixel_Access
static inline Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}
}

static inline void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16 *)p = pixel;
			break;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			} else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

void SDLSurface::mapPixelColor(std::function<Color (const Color&)> mapping)
{
	int bpp = mSurface->format->BytesPerPixel;
	if(bpp != 4 && bpp != 3) {
		throw std::runtime_error("Can only change pixel color with bpp = 3 or 4");
	}

	for(int i = 0; i < mSurface->h; i++) {
		for(int j = 0; j < mSurface->w; j++) {
			Uint32 v = getpixel(mSurface, j, i);
			Uint8 r, g, b, a;
			SDL_GetRGBA(v, mSurface->format, &r, &g, &b, &a);
			Color c = Color(r, g, b);
			Color c2 = mapping(c);
			v = SDL_MapRGBA(mSurface->format, c2.r, c2.g, c2.b, a);
			putpixel(mSurface, j, i, v);
		}
	}
}

}


