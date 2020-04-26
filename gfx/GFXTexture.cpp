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
Texture::Texture(int width, int height, PixelFormat format)
{
	m_width = width;
	m_height = height;
	m_format = format;
	m_pitch = 0;
	m_bpp = Image::FormatToBpp(format);
}



//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
Texture::~Texture()
{
}


//---------------------------------------------------------------------
// lock
//---------------------------------------------------------------------
void* Texture::Lock(bool readOnly)
{
	return NULL;
}


//---------------------------------------------------------------------
// unlock
//---------------------------------------------------------------------
void Texture::Unlock()
{
}


//---------------------------------------------------------------------
// copy rectangel
//---------------------------------------------------------------------
void Texture::CopyTo(int x, int y, Image *src, int sx, int sy, int sw, int sh)
{
	if (GetBpp() != src->GetBpp()) {
		return;
	}
	unsigned char *sbits = (unsigned char*)Lock(false);
	if (sbits) {
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
			for (int j = 0; j < sh; j++) {
				const unsigned char *ss = sbits + (y + j) * m_pitch; 
				unsigned char *dd = src->GetLine(sy + j);
				memcpy(dd, ss, need);
			}
		}
		Unlock();
	}
}


//---------------------------------------------------------------------
// copy rectangle
//---------------------------------------------------------------------
void Texture::CopyFrom(int x, int y, const Image *src, int sx, int sy, int sw, int sh)
{
	if (GetBpp() != src->GetBpp()) {
		return;
	}
	unsigned char *sbits = (unsigned char*)Lock(false);
	if (sbits) {
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
			for (int j = 0; j < sh; j++) {
				const unsigned char *ss = src->GetLine(sy + j);
				unsigned char *dd = sbits + (y + j) * m_pitch; 
				memcpy(dd, ss, need);
			}
		}
		Unlock();
	}
}


//---------------------------------------------------------------------
// copy rectangle
//---------------------------------------------------------------------
void Texture::CopyFrom(int x, int y, Texture *src, int sx, int sy, int sw, int sh)
{
	if (GetBpp() != src->GetBpp()) {
		return;
	}
	unsigned char *sbits = (unsigned char*)Lock(false);
	if (sbits == NULL) return;
	unsigned char *dbits = (unsigned char*)src->Lock(true);
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
			for (int j = 0; j < sh; j++) {
				const unsigned char *ss = dbits + (sy + j) * src->m_pitch;
				unsigned char *dd = sbits + (y + j) * m_pitch; 
				memcpy(dd, ss, need);
			}
		}
		src->Unlock();
	}
	Unlock();
}


//---------------------------------------------------------------------
// save to image
//---------------------------------------------------------------------
void Texture::SaveToImage(Image *img)
{
	CopyTo(0, 0, img, 0, 0, GetWidth(), GetHeight());
}


//---------------------------------------------------------------------
// restore from image
//---------------------------------------------------------------------
void Texture::RestoreFromImage(const Image *img)
{
	CopyFrom(0, 0, img, 0, 0, GetWidth(), GetHeight());
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



