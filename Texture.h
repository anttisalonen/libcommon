#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

#include "SDLSurface.h"

namespace Common {

class Texture {
	public:
		Texture(const SDLSurface& surf, unsigned int startrow = 0,
				unsigned int height = 0);
		Texture(const SDL_Surface* surf, unsigned int startrow = 0,
				unsigned int height = 0);
		Texture(const char* filename, unsigned int startrow = 0,
				unsigned int height = 0);

		~Texture();
		Texture& operator=(const Texture&) = delete;
		Texture(const Texture&) = delete;

		GLuint getTexture() const;
		int getWidth() const;
		int getHeight() const;
		static GLuint loadTexture(const char* filename,
				unsigned int startrow = 0, unsigned int height = 0);
		static GLuint loadTexture(const SDL_Surface* surf,
				unsigned int startrow = 0, unsigned int height = 0);

	private:
		void setupSDLSurface(const SDL_Surface* s,
				unsigned int startrow, unsigned int height);
		GLuint mTexture;
		int mWidth;
		int mHeight;
};

}

#endif

