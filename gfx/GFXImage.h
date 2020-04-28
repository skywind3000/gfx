//=====================================================================
//
// GFXImage.h - 
//
// Last Modified: 2020/04/26 20:42:22
//
//=====================================================================
#ifndef _GFX_IMAGE_H_
#define _GFX_IMAGE_H_

#include <stddef.h>
#include <stdint.h>


//---------------------------------------------------------------------
// Namespace
//---------------------------------------------------------------------
#ifndef NAMESPACE_BEGIN
#define NAMESPACE_BEGIN(x)  namespace x {
#endif

#ifndef NAMESPACE_END
#define NAMESPACE_END(x) }
#endif


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// Pixel Format
//---------------------------------------------------------------------
enum PixelFormat
{
	FMT_A8R8G8B8 = 0,
	FMT_A8B8G8R8,
	FMT_X8R8G8B8,
	FMT_R8G8B8,
	FMT_B8G8R8,
	FMT_A1R5G5B5,
	FMT_A4R4G4B4,
	FMT_R5G6B5,
	FMT_G8,
	FMT_UNKNOWN,
};


//---------------------------------------------------------------------
// Image
//---------------------------------------------------------------------
class Image
{
public:
	virtual ~Image();
	Image(int w, int h, PixelFormat fmt);

public:
	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
	inline int32_t GetPitch() const { return m_pitch; }
	inline int GetBpp() const { return m_bpp; }
	inline PixelFormat GetFormat() const { return m_fmt; }

	inline unsigned char *GetBits() { return m_bits; }
	inline const unsigned char *GetBits() const { return m_bits; }

	inline unsigned char *GetLine(int y) { return m_bits + y * m_pitch; }
	inline const unsigned char *GetLine(int y) const { return m_bits + y * m_pitch; }

	inline unsigned char* operator[](int y) { return GetLine(y); }
	inline const unsigned char* operator[](int y) const { return GetLine(y); }

public:
	void CopyRect(int x, int y, const Image *src, int sx, int sy, int sw, int sh);

public:

	static int FormatToBpp(PixelFormat fmt);
	
	// ClipRect - clip the rectangle from the src clip and dst clip then
	// caculate a new rectangle shared between dst and src cliprect:
	// clipdst  - dest clip array (left, top, right, bottom)
	// clipsrc  - source clip array (left, top, right, bottom)
	// (x, y)   - dest position
	// rectsrc  - source rect
	// mode     - check 1/HFLIP or 2/VFLIP
	// return zero for successful, return non-zero if there is no shared part
	static int ClipRect(const int *clipdst, const int *clipsrc, int *x, int *y,
			int *rectsrc, int mode);

protected:
	unsigned char *m_bits;
	int32_t m_pitch;
	int32_t m_psize;
	PixelFormat m_fmt;
	int m_width;
	int m_height;
	int m_bpp;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif


