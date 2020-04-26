//=====================================================================
//
// GFXColor.h - 
//
// Last Modified: 2020/04/26 17:49:54
//
//=====================================================================
#ifndef _GFX_COLOR_H_
#define _GFX_COLOR_H_

#include "GFXVector.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// Color
//---------------------------------------------------------------------
struct Color 
{
	uint32_t color;

	inline Color() {}
	inline Color(uint32_t c): color(c) {}
	inline Color(uint32_t a, uint32_t r, uint32_t g, uint32_t b):
		color((a << 24) | (r << 16) | (g << 8) | b) {}

	inline uint32_t GetRed() const { return (color >> 16) & 0xff; }
	inline uint32_t GetGreen() const { return (color >> 8) & 0xff; }
	inline uint32_t GetBlue() const { return (color >> 0) & 0xff; }
	inline uint32_t GetAlpha() const { return (color >> 24) & 0xff; }

	inline void SetRed(uint32_t r) { color = (color & 0xff00ffff) | ((r & 0xff) << 16); }
	inline void SetGreen(uint32_t g) { color = (color & 0xffff00ff) | ((g & 0xff) << 8); }
	inline void SetBlue(uint32_t b) { color = (color & 0xffffff00) | ((b & 0xff) << 0); }
	inline void SetAlpha(uint32_t a) { color = (color & 0xffffff) | ((a & 0xff) << 24); }

	inline void Set(uint32_t c) { color = c; }
	inline void Set(uint32_t a, uint32_t r, uint32_t g, uint32_t b) {
		color = (a << 24) | (r << 16) | (g << 8) | b;
	}

	inline float GetLuminance() const {
		return 0.3f * GetRed() + 0.59f * GetGreen() + 0.11f * GetBlue();
	}
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);

#endif


