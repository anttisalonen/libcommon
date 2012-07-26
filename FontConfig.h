#ifndef COMMON_FONTCONFIG_H
#define COMMON_FONTCONFIG_H

#include <string>

#include "Color.h"

namespace Common {

struct FontConfig {
	inline FontConfig(const char* str, const Common::Color& c, float scale);
	inline bool operator==(const FontConfig& f) const;
	inline bool operator<(const FontConfig& f) const;
	std::string mText;
	Common::Color mColor;
	float mScale;
};

FontConfig::FontConfig(const char* str, const Common::Color& c, float scale)
	: mText(str),
	mColor(c),
	mScale(scale)
{
}

bool FontConfig::operator==(const FontConfig& f) const
{
	return mText == f.mText && mColor == f.mColor && mScale == f.mScale;
}

bool FontConfig::operator<(const FontConfig& f) const
{
	if(mText != f.mText)
		return mText < f.mText;
	if(!(mColor == f.mColor))
		return mColor < f.mColor;
	return mScale < f.mScale;
}

struct TextTexture {
	inline TextTexture(boost::shared_ptr<Common::Texture> t, unsigned int w, unsigned int h);
	boost::shared_ptr<Common::Texture> mTexture;
	unsigned int mWidth;
	unsigned int mHeight;
};

TextTexture::TextTexture(boost::shared_ptr<Common::Texture> t, unsigned int w, unsigned int h)
	: mTexture(t),
	mWidth(w),
	mHeight(h)
{
}

typedef std::map<FontConfig, boost::shared_ptr<TextTexture>> TextMap;


}

#endif
