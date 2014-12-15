#include <stdio.h>
#include <sstream>
#include <stdexcept>

#include <SDL_image.h>
#include <GL/gl.h>

#include "Texture.h"

namespace Common {

Texture::Texture(const SDLSurface& surf, unsigned int startrow, unsigned int height)
{
	setupSDLSurface(surf.getSurface(), startrow, height);
}

Texture::Texture(const char* filename, unsigned int startrow, unsigned int height)
{
	SDLSurface surf(filename);
	setupSDLSurface(surf.getSurface(), startrow, height);
}

Texture::Texture(const SDL_Surface* surf, unsigned int startrow, unsigned int height)
{
	setupSDLSurface(surf, startrow, height);
}

void Texture::setupSDLSurface(const SDL_Surface* surf, unsigned int startrow, unsigned int height)
{
	mTexture = loadTexture(surf, startrow, height);
	mWidth = surf->w;
	mHeight = surf->h;
}

GLuint Texture::loadTexture(const char* filename,
		unsigned int startrow, unsigned int height)
{
	SDLSurface surf(filename);
	return loadTexture(surf.getSurface(), startrow, height);
}

GLuint Texture::loadTexture(const SDL_Surface* surf,
		unsigned int startrow, unsigned int height)
{
	bool hasAlpha = surf->format->BytesPerPixel == 4;
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	GLenum format;
	if(hasAlpha) {
		if (surf->format->Rmask == 0x000000ff)
			format = GL_RGBA;
		else
			format = GL_BGRA;
	} else {
		if (surf->format->Rmask == 0x000000ff)
			format = GL_RGB;
		else
			format = GL_BGR;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, hasAlpha ? GL_RGBA8 : GL_RGB8, surf->w, height ? height : surf->h,
			0, format, GL_UNSIGNED_BYTE,
			(char*)surf->pixels + startrow * surf->w * surf->format->BytesPerPixel);
	return texture;
}

Texture::~Texture()
{
	glDeleteTextures(1, &mTexture);
}

GLuint Texture::getTexture() const
{
	return mTexture;
}

int Texture::getWidth() const
{
	return mWidth;
}

int Texture::getHeight() const
{
	return mHeight;
}

}
