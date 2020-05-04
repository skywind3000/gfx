//=====================================================================
//
// GFXTexture.cpp - 
//
// Last Modified: 2020/04/26 16:50:55
//
//=====================================================================
#include "GFXTexture.h"

//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
Texture::Texture()
{
	InitParameter(0, 0, FMT_A8R8G8B8, true);
}



//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
Texture::~Texture()
{
}


//---------------------------------------------------------------------
// init parameters
//---------------------------------------------------------------------
void Texture::InitParameter(int w, int h, PixelFormat fmt, bool lockable)
{
	m_width = w;
	m_height = h;
	m_format = fmt;
	m_levels = 0;
	m_locked_pitch = 0;
	m_locked_bits = NULL;
	m_locked_w = -1;
	m_locked_h = -1;
	m_bpp = Image::FormatToBpp(fmt);
	m_inv_width = (w == 0)? 0.0f : 1.0f / ((float)w);
	m_inv_height = (h == 0)? 0.0f : 1.0f / ((float)h);
	m_lockable = lockable;
}


//---------------------------------------------------------------------
// get level
//---------------------------------------------------------------------
int Texture::GetLevelWidth(int level) const
{
	if (level < 0 || level >= m_levels) return 0;
	int x = m_width >> level;
	return (x < 1)? 1 : x;
}


//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
int Texture::GetLevelHeight(int level) const
{
	if (level < 0 || level >= m_levels) return 0;
	int x = m_height >> level;
	return (x < 1)? 1 : x;
}


//---------------------------------------------------------------------
// update rect
//---------------------------------------------------------------------
bool Texture::UpdateTexture(int mip, const Rect *rect, const void *bits, int pitch)
{
	if (m_lockable && m_locked_bits == NULL) {
		const uint8_t *src = reinterpret_cast<const uint8_t*>(bits);
		Rect rc;
		if (rect == NULL) {
			rc.left = 0;
			rc.top = 0;
			rc.right = GetLevelWidth(mip);
			rc.bottom = GetLevelHeight(mip);
			rect = &rc;
		}
		if (Lock(mip, rect, false) == NULL) {
			return false;
		}
		uint8_t *dst = m_locked_bits;
		int w = rect->right - rect->left;
		int h = rect->bottom - rect->top;
		int size = (m_bpp / 8) * w;
		for (int j = 0; j < h; j++) {
			memcpy(dst, src, size);
			dst += m_locked_pitch;
			src += pitch;
		}
		Unlock(mip);
		return true;
	}
	return false;
}



//---------------------------------------------------------------------
// read texture
//---------------------------------------------------------------------
bool Texture::ReadTexture(int mip, const Rect *rect, void *bits, int pitch)
{
	if (m_lockable && m_locked_bits == NULL) {
		uint8_t *dst = reinterpret_cast<uint8_t*>(bits);
		Rect rc;
		if (rect == NULL) {
			rc.left = 0;
			rc.top = 0;
			rc.right = GetLevelWidth(mip);
			rc.bottom = GetLevelHeight(mip);
			rect = &rc;
		}
		if (Lock(mip, rect, true) == NULL) {
			return false;
		}
		const uint8_t *src = m_locked_bits;
		int w = rect->right - rect->left;
		int h = rect->bottom - rect->top;
		int size = (m_bpp / 8) * w;
		for (int j = 0; j < h; j++) {
			memcpy(dst, src, size);
			dst += pitch;
			src += m_locked_pitch;
		}
	}
	return false;
}


//---------------------------------------------------------------------
// lock
//---------------------------------------------------------------------
void* Texture::Lock(int mip, const Rect *rect, bool readOnly)
{
	return NULL;
}


//---------------------------------------------------------------------
// unlock
//---------------------------------------------------------------------
void Texture::Unlock(int mip)
{
}


//---------------------------------------------------------------------
// pre reset
//---------------------------------------------------------------------
int Texture::PreReset()
{
	return 0;
}


//---------------------------------------------------------------------
// post reset
//---------------------------------------------------------------------
int Texture::PostReset()
{
	return 0;
}


//---------------------------------------------------------------------
// copy rectangel
//---------------------------------------------------------------------
void Texture::CopyTo(int mip, int x, int y, Image *src, int sx, int sy, int sw, int sh)
{
	if (GetBpp() != src->GetBpp()) {
		return;
	}
	int pixelbytes = m_bpp / 8;
	if (mip == 0) {
		int clipdst[4] = { 0, 0, GetWidth(), GetHeight() };
		int clipsrc[4] = { 0, 0, src->GetWidth(), src->GetHeight() };
		int rectsrc[4] = { sx, sy, sx + sw, sy + sh };
		int mode = 0;
		if (Image::ClipRect(clipdst, clipsrc, &x, &y, rectsrc, mode) != 0) {
			return;
		}
		sx = rectsrc[0];
		sy = rectsrc[1];
		sw = rectsrc[2] - rectsrc[0];
		sh = rectsrc[3] - rectsrc[1];
	}
	Rect rc;
	rc.left = x;
	rc.top = y;
	rc.right = x + sw;
	rc.bottom = y + sh;
	unsigned char *bits = src->GetLine(sy) + pixelbytes * sx;
	ReadTexture(mip, &rc, bits, src->GetPitch());
}


//---------------------------------------------------------------------
// copy rectangle
//---------------------------------------------------------------------
void Texture::CopyFrom(int mip, int x, int y, const Image *src, int sx, int sy, int sw, int sh)
{
	if (GetBpp() != src->GetBpp()) {
		return;
	}
	int pixelbytes = m_bpp / 8;
	if (mip == 0) {
		int clipdst[4] = { 0, 0, GetWidth(), GetHeight() };
		int clipsrc[4] = { 0, 0, src->GetWidth(), src->GetHeight() };
		int rectsrc[4] = { sx, sy, sx + sw, sy + sh };
		int mode = 0;
		if (Image::ClipRect(clipdst, clipsrc, &x, &y, rectsrc, mode) != 0) {
			return;
		}
		sx = rectsrc[0];
		sy = rectsrc[1];
		sw = rectsrc[2] - rectsrc[0];
		sh = rectsrc[3] - rectsrc[1];
	}
	Rect rc;
	rc.left = x;
	rc.top = y;
	rc.right = x + sw;
	rc.bottom = y + sh;
	const unsigned char *bits = src->GetLine(sy) + pixelbytes * sx;
	UpdateTexture(mip, &rc, bits, src->GetPitch());
}


//---------------------------------------------------------------------
// copy rectangle
//---------------------------------------------------------------------
void Texture::CopyFrom(int x, int y, Texture *src, int sx, int sy, int sw, int sh)
{
	if (GetBpp() != src->GetBpp()) {
		return;
	}
	if (m_lockable == false || src->m_lockable == false) {
		return;
	}
	unsigned char *sbits = (unsigned char*)Lock(0, NULL, false);
	if (sbits == NULL) return;
	unsigned char *dbits = (unsigned char*)src->Lock(0, NULL, true);
	if (dbits) {
		int clipdst[4] = { 0, 0, GetWidth(), GetHeight() };
		int clipsrc[4] = { 0, 0, src->GetWidth(), src->GetHeight() };
		int rectsrc[4] = { sx, sy, sx + sw, sy + sh };
		int mode = 0;
		if (Image::ClipRect(clipdst, clipsrc, &x, &y, rectsrc, mode) == 0) {
			sx = rectsrc[0];
			sy = rectsrc[1];
			sw = rectsrc[2] - rectsrc[0];
			sh = rectsrc[3] - rectsrc[1];
			int need = sw * (GetBpp() / 8);
			int pixelbytes = m_bpp / 8;
			for (int j = 0; j < sh; j++) {
				const unsigned char *ss = dbits + (sy + j) * src->m_locked_pitch;
				unsigned char *dd = sbits + (y + j) * m_locked_pitch; 
				memcpy(dd + pixelbytes * x, ss + pixelbytes * sx, need);
			}
		}
		src->Unlock(0);
	}
	Unlock(0);
}


//---------------------------------------------------------------------
// save to image
//---------------------------------------------------------------------
void Texture::SaveToImage(int mip, Image *img)
{
	CopyTo(mip, 0, 0, img, 0, 0, GetWidth(), GetHeight());
}


//---------------------------------------------------------------------
// restore from image
//---------------------------------------------------------------------
void Texture::RestoreFromImage(int mip, const Image *img)
{
	CopyFrom(mip, 0, 0, img, 0, 0, GetWidth(), GetHeight());
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



