#include <stdexcept>

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <GL/gl.h>

#include "SDL_utils.h"
#include "Math.h"


namespace Common {

SDL_Surface* SDL_utils::initSDL(int w, int h, const char* caption)
{
	SDL_Surface* screen;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		fprintf(stderr, "Unable to init SDL: %s\n", SDL_GetError());
		throw std::runtime_error("SDL init");
	}
	if(SDL_EnableUNICODE(1) == -1) {
		fprintf(stderr, "Unable to enable Unicode: %s\n", SDL_GetError());
		throw std::runtime_error("Enable Unicode");
	}
	screen = SDL_SetVideoMode(w, h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_OPENGL);
	if(!screen) {
		fprintf(stderr, "Unable to set video mode\n");
		throw std::runtime_error("Set video mode");
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifdef __WIN32__
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
#endif
	if(IMG_Init(IMG_INIT_PNG) == -1) {
		fprintf(stderr, "Unable to init SDL_image: %s\n", IMG_GetError());
		throw std::runtime_error("SDL_image");
	}
	if(TTF_Init() == -1) {
		fprintf(stderr, "Unable to init SDL_ttf: %s\n", TTF_GetError());
		throw std::runtime_error("SDL_ttf");
	}
	SDL_EnableKeyRepeat(0, SDL_DEFAULT_REPEAT_INTERVAL);
	if(caption)
		SDL_WM_SetCaption(caption, nullptr);

	return screen;
}

void SDL_utils::setupOrthoScreen(int w, int h)
{
	GLenum err;
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0, 0, 0.1, 1);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);

	err = glGetError();
	if(err != GL_NO_ERROR) {
		fprintf(stderr, "GL error: %s (%d)\n",
				GLErrorToString(err),
				err);
		throw std::runtime_error("GL error");
	}
}

const char* SDL_utils::GLErrorToString(GLenum err)
{
	switch(err) {
		case GL_NO_ERROR: return "GL_NO_ERROR";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
		case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
		case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		case GL_TABLE_TOO_LARGE: return "GL_TABLE_TOO_LARGE";
	}
	return "unknown error";
}

void SDL_utils::drawSprite(const Texture& t,
		const Rectangle& vertcoords,
		const Rectangle& texcoords, float depth, float alpha)
{
	glColor4f(1.0f, 1.0f, 1.0f, alpha);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, t.getTexture());
	glBegin(GL_QUADS);
	glTexCoord2f(texcoords.x, texcoords.y);
	glVertex3f(vertcoords.x, vertcoords.y, depth);
	glTexCoord2f(texcoords.x + texcoords.w, texcoords.y);
	glVertex3f(vertcoords.x + vertcoords.w, vertcoords.y, depth);
	glTexCoord2f(texcoords.x + texcoords.w, texcoords.y + texcoords.h);
	glVertex3f(vertcoords.x + vertcoords.w, vertcoords.y + vertcoords.h, depth);
	glTexCoord2f(texcoords.x, texcoords.y + texcoords.h);
	glVertex3f(vertcoords.x, vertcoords.y + vertcoords.h, depth);
	glEnd();
}

void SDL_utils::drawText(TextMap& tm, TTF_Font* font, const Vector3& camera,
		float scaleLevel, int screenWidth, int screenHeight,
		float x, float y,
		const FontConfig& f,
		bool screencoordinates, bool centered)
{
	if(f.mText.size() == 0)
		return;
	auto it = tm.find(f);
	if(it == tm.end()) {
		SDL_Surface* text;
		SDL_Color color = {f.mColor.r, f.mColor.g, f.mColor.b};

		text = TTF_RenderUTF8_Blended(font, f.mText.c_str(), color);
		if(!text) {
			fprintf(stderr, "Could not render text: %s\n",
					TTF_GetError());
			return;
		}
		else {
			boost::shared_ptr<Texture> texture(new Texture(text));
			boost::shared_ptr<TextTexture> ttexture(new TextTexture(texture, text->w, text->h));
			auto it2 = tm.insert(std::make_pair(f, ttexture));
			it = it2.first;
			SDL_FreeSurface(text);
		}

	}

	assert(it != tm.end());
	float spritex, spritey;
	float spritewidth, spriteheight;
	if(screencoordinates) {
		spritex = x;
		spritey = y;
		spritewidth  = it->first.mScale * it->second->mWidth;
		spriteheight = it->first.mScale * it->second->mHeight;
	}
	else {
		spritex = (-camera.x + x) * scaleLevel + screenWidth * 0.5f;
		spritey = (-camera.y + y) * scaleLevel + screenHeight * 0.5f;
		spritewidth  = scaleLevel * it->first.mScale * it->second->mWidth;
		spriteheight = scaleLevel * it->first.mScale * it->second->mHeight;
	}
	if(centered) {
		spritex -= spritewidth * 0.5f;
	}

	drawSprite(*it->second->mTexture, Rectangle(spritex, spritey,
				spritewidth, spriteheight),
			Rectangle(0, 1, 1, -1), 0.0f);
}

void SDL_utils::drawCircle(float x, float y, float rad)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0f, 1.0f, 1.0f);
	for(int i = 0; i < 32; i++) {
		float v = PI * i / 16.0f;
		glVertex3f(rad * sin(v) + x,
				rad * cos(v) + y,
				0.0f);
	}
	glVertex3f(x, y + rad, 0.0f);
	glEnd();
}

void SDL_utils::drawPoint(const Vector3& coords, float size, const Common::Color& col)
{
	glDisable(GL_TEXTURE_2D);
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(col.r, col.g, col.b);
	glVertex3f(coords.x, coords.y, 0.0f);
	glEnd();
}

void SDL_utils::drawRectangle(float x, float y, float x2, float y2,
		const Common::Color& c, float alpha, bool onlyframes)
{
	glDisable(GL_TEXTURE_2D);
	if(onlyframes)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_QUADS);
	glColor4ub(c.r, c.g, c.b, alpha * 255);
	glVertex3f(x,  y,  0.0f);
	glVertex3f(x,  y2, 0.0f);
	glVertex3f(x2, y2, 0.0f);
	glVertex3f(x2, y,  0.0f);
	glEnd();
}

void SDL_utils::drawLine(const Common::Vector3& p1, const Common::Vector3& p2, const Common::Color& c, float alpha)
{
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	glColor4ub(c.r, c.g, c.b, alpha * 255);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
}

}

