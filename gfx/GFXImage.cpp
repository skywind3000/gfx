//=====================================================================
//
// GFXImage.cpp - 
//
// Last Modified: 2020/04/26 21:38:06
//
//=====================================================================
#include <stddef.h>
#include <string.h>

#include "GFXImage.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// Format Exchange
//---------------------------------------------------------------------
int Image::FormatToBpp(PixelFormat fmt)
{
	switch (fmt) {
	case FMT_A8R8G8B8:
	case FMT_A8B8G8R8:
	case FMT_X8R8G8B8:
		return 4;
	case FMT_R8G8B8:
	case FMT_B8G8R8:
		return 3;
	case FMT_A1R5G5B5:
	case FMT_A4R4G4B4:
	case FMT_R5G6B5:
		return 2;
	case FMT_G8:
		return 1;
	}
	return 4;
}


//---------------------------------------------------------------------
// Create Image
//---------------------------------------------------------------------
Image::Image(int w, int h, PixelFormat fmt)
{
	m_bpp = FormatToBpp(fmt);
	m_psize = m_bpp / 8;
	m_pitch = (m_psize * w + 3) & ~3;
	m_width = w;
	m_height = h;
	m_bits = new unsigned char[m_pitch * h];
}


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
Image::~Image()
{
	if (m_bits) {
		delete []m_bits;
	}
	m_bits = NULL;
	m_width = 0;
	m_height = 0;
}


//---------------------------------------------------------------------
// copy rectangle
//---------------------------------------------------------------------
void Image::CopyRect(int x, int y, const Image *src, int sx, int sy, int sw, int sh)
{
	if (src->GetBpp() == GetBpp()) {
		int clipdst[4] = { 0, 0, GetWidth(), GetHeight() };
		int clipsrc[4] = { 0, 0, src->GetWidth(), src->GetHeight() };
		int rectsrc[4] = { sx, sy, sx + sw, sy + sh };
		int mode = 0;
		if (ClipRect(clipdst, clipsrc, &x, &y, rectsrc, mode) == 0) {
			sx = rectsrc[0];
			sy = rectsrc[1];
			sw = rectsrc[2] - rectsrc[0];
			sh = rectsrc[3] - rectsrc[1];
			int need = sw * (GetBpp() / 8);
			for (int j = 0; j < sh; j++) {
				const unsigned char *ss = src->GetLine(sy + j);
				unsigned char *dd = GetLine(y + j);
				memcpy(dd, ss, need);
			}
		}
	}
}


//---------------------------------------------------------------------
// ClipRect - clip the rectangle from the src clip and dst clip then
// caculate a new rectangle shared between dst and src cliprect:
// clipdst  - dest clip array (left, top, right, bottom)
// clipsrc  - source clip array (left, top, right, bottom)
// (x, y)   - dest position
// rectsrc  - source rect
// mode     - check 1/HFLIP or 2/VFLIP
// return zero for successful, return non-zero if there is no shared part
//---------------------------------------------------------------------
int Image::ClipRect(const int *clipdst, const int *clipsrc, 
	int *x, int *y, int *rectsrc, int mode)
{
	int dcl = clipdst[0];       /* dest clip: left     */
	int dct = clipdst[1];       /* dest clip: top      */
	int dcr = clipdst[2];       /* dest clip: right    */
	int dcb = clipdst[3];       /* dest clip: bottom   */
	int scl = clipsrc[0];       /* source clip: left   */
	int sct = clipsrc[1];       /* source clip: top    */
	int scr = clipsrc[2];       /* source clip: right  */
	int scb = clipsrc[3];       /* source clip: bottom */
	int dx = *x;                /* dest x position     */
	int dy = *y;                /* dest y position     */
	int sl = rectsrc[0];        /* source rectangle: left   */
	int st = rectsrc[1];        /* source rectangle: top    */
	int sr = rectsrc[2];        /* source rectangle: right  */
	int sb = rectsrc[3];        /* source rectangle: bottom */
	int hflip, vflip;
	int w, h, d;

	hflip = (mode & 1)? 1 : 0;
	vflip = (mode & 2)? 1 : 0;

	if (dcr <= dcl || dcb <= dct || scr <= scl || scb <= sct) 
		return -1;

	if (sr <= scl || sb <= sct || sl >= scr || st >= scb) 
		return -2;

	/* check dest clip: left */
	if (dx < dcl) {
		d = dcl - dx;
		dx = dcl;
		if (!hflip) sl += d;
		else sr -= d;
	}

	/* check dest clip: top */
	if (dy < dct) {
		d = dct - dy;
		dy = dct;
		if (!vflip) st += d;
		else sb -= d;
	}

	w = sr - sl;
	h = sb - st;

	if (w < 0 || h < 0) 
		return -3;

	/* check dest clip: right */
	if (dx + w > dcr) {
		d = dx + w - dcr;
		if (!hflip) sr -= d;
		else sl += d;
	}

	/* check dest clip: bottom */
	if (dy + h > dcb) {
		d = dy + h - dcb;
		if (!vflip) sb -= d;
		else st += d;
	}

	if (sl >= sr || st >= sb) 
		return -4;

	/* check source clip: left */
	if (sl < scl) {
		d = scl - sl;
		sl = scl;
		if (!hflip) dx += d;
	}

	/* check source clip: top */
	if (st < sct) {
		d = sct - st;
		st = sct;
		if (!vflip) dy += d;
	}

	if (sl >= sr || st >= sb) 
		return -5;

	/* check source clip: right */
	if (sr > scr) {
		d = sr - scr;
		sr = scr;
		if (hflip) dx += d;
	}

	/* check source clip: bottom */
	if (sb > scb) {
		d = sb - scb;
		sb = scb;
		if (vflip) dy += d;
	}

	if (sl >= sr || st >= sb) 
		return -6;

	*x = dx;
	*y = dy;

	rectsrc[0] = sl;
	rectsrc[1] = st;
	rectsrc[2] = sr;
	rectsrc[3] = sb;

	return 0;
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



