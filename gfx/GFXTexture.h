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
#include "GFXTypes.h"
#include "GFXImage.h"
#include "GFXMath.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);

class VideoDriver;


//---------------------------------------------------------------------
// Texture
//---------------------------------------------------------------------
class Texture
{
public:
	virtual ~Texture();
	Texture();

public:

	virtual bool UpdateTexture(int mip, const Rect *rect, const void *bits, int pitch);
	virtual bool ReadTexture(int mip, const Rect *rect, void *bits, int pitch);

	virtual int PreReset();
	virtual int PostReset();

	virtual void *Lock(int mip, const Rect *rect, bool readOnly = false);
	virtual void Unlock(int mip);

	virtual void CopyTo(int mip, int x, int y, Image *src, int sx, int sy, int sw, int sh);
	virtual void CopyFrom(int mip, int x, int y, const Image *src, int sx, int sy, int sw, int sh);
	virtual void CopyFrom(int x, int y, Texture *src, int sx, int sy, int sw, int sh);

	virtual void SaveToImage(int mip, Image *img);
	virtual void RestoreFromImage(int mip, const Image *img);

	virtual int GetLevelWidth(int level) const;
	virtual int GetLevelHeight(int level) const;

	inline virtual bool Available() const { return false; }

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline PixelFormat GetFormat() const { return m_format; }
	inline int GetBpp() const { return m_bpp; }

	inline float GetInvWidth() const { return m_inv_width; }
	inline float GetInvHeight() const { return m_inv_height; }

	inline bool IsLockable() const { return m_lockable; }
	inline int32_t LockedPitch() const { return m_locked_pitch; }
	inline void* LockedBits() { return m_locked_bits; }
	inline const void* LockedBits() const { return m_locked_bits; }
	inline int LockedWidth() const { return m_locked_w; }
	inline int LockedHeight() const { return m_locked_h; }

protected:
	void InitParameter(int w, int h, PixelFormat fmt, bool lockable);

protected:
	PixelFormat m_format;
	int m_bpp;
	int m_width;
	int m_height;
	int m_levels;
	float m_inv_width;
	float m_inv_height;
	bool m_lockable;
	int m_locked_w;
	int m_locked_h;
	uint8_t *m_locked_bits;
	int32_t m_locked_pitch;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif



