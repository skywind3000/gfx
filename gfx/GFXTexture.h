//=====================================================================
//
// GFXTexture.h - 
//
// Last Modified: 2020/04/26 16:32:43
//
//=====================================================================
#ifndef _GFX_TEXTURE_H_
#define _GFX_TEXTURE_H_

#include "GFX.h"
#include "GFXImage.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// Texture
//---------------------------------------------------------------------
class Texture
{
public:
	virtual ~Texture();
	Texture(int width, int height, PixelFormat format);

public:

	virtual void *Lock(bool readOnly = false);
	virtual void Unlock();

	virtual void CopyTo(int x, int y, Image *src, int sx, int sy, int sw, int sh);
	virtual void CopyFrom(int x, int y, const Image *src, int sx, int sy, int sw, int sh);
	virtual void CopyFrom(int x, int y, Texture *src, int sx, int sy, int sw, int sh);

	virtual void SaveToImage(Image *img);
	virtual void RestoreFromImage(const Image *img);

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline PixelFormat GetFormat() const { return m_format; }
	inline int32_t GetPitch() const { return m_pitch; }
	inline int GetBpp() const { return m_bpp; }

	inline void* GetBits() { return m_bits; }
	inline const void* GetBits() const { return m_bits; }

protected:
	int m_width;
	int m_height;
	int32_t m_pitch;
	void *m_bits;
	int m_bpp;
	PixelFormat m_format;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif



