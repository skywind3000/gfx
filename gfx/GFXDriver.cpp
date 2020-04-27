//=====================================================================
//
// GFXDriver.cpp - 
//
// Last Modified: 2020/04/27 00:02:22
//
//=====================================================================
#include "GFXDriver.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
VideoDriver::VideoDriver()
{
	m_initialized = false;
	m_background_color.color = 0xff191970;
	m_video_capacity.texture_size_pow2 = true;
	m_video_capacity.texture_square_only = true;
	m_video_capacity.texture_has_alpha = false;
	m_video_capacity.texture_has_mipmap = false;
	m_video_capacity.texture_max_width = 0;
	m_video_capacity.texture_max_height = 0;
}


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
VideoDriver::~VideoDriver()
{
}


//---------------------------------------------------------------------
// create
//---------------------------------------------------------------------
int VideoDriver::Create(const CreationParameter *params)
{
	return -1;
}


//---------------------------------------------------------------------
// release
//---------------------------------------------------------------------
int VideoDriver::Release()
{
	return -1;
}


//---------------------------------------------------------------------
// Setup size
//---------------------------------------------------------------------
void VideoDriver::InitSize(int width, int height)
{
	m_video_width = width;
	m_video_height = height;
	m_inv_width = (width == 0)? 0.0f : 1.0f / ((float)width);
	m_inv_height = (height == 0)? 0.0f : 1.0f / ((float)height);
}


//---------------------------------------------------------------------
// set background color
//---------------------------------------------------------------------
void VideoDriver::SetColor(Core::Color color)
{
	m_background_color = color;
}


//---------------------------------------------------------------------
// clear buffer
//---------------------------------------------------------------------
bool VideoDriver::Clear(bool target, bool zBuffer, bool stencil)
{
	return false;
}


//---------------------------------------------------------------------
// begin scene
//---------------------------------------------------------------------
bool VideoDriver::BeginScene()
{
	return false;
}


//---------------------------------------------------------------------
// end scene
//---------------------------------------------------------------------
bool VideoDriver::EndScene()
{
	return false;
}


//---------------------------------------------------------------------
// present
//---------------------------------------------------------------------
bool VideoDriver::Present()
{
	return false;
}


//---------------------------------------------------------------------
// check reset
//---------------------------------------------------------------------
bool VideoDriver::CheckReset()
{
	return false;
}


//---------------------------------------------------------------------
// calculate texture size
//---------------------------------------------------------------------
bool VideoDriver::TextureFit(int w, int h, PixelFormat fmt, int *tw, int *th)
{
	int neww = w;
	int newh = h;
	if (m_video_capacity.texture_size_pow2) {
		for (neww = 4; neww < w; neww *= 2);
		for (newh = 4; newh < h; newh *= 2);
	}
	if (m_video_capacity.texture_square_only) {
		int maxs = (neww > newh)? neww : newh;
		neww = newh = maxs;
	}
	*tw = neww;
	*th = newh;
	return true;
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



