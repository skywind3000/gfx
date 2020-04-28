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
// enable format
//---------------------------------------------------------------------
void VideoDriver::AddDeviceFormat(PixelFormat fmt, ResourceType rt)
{
	int index = (int)fmt;
	if (index >= (int)m_support_formats.size()) {
		int pos = (int)m_support_formats.size();
		m_support_formats.resize(index + 1);
		for (int i = pos; i <= index; i++) {
			m_support_formats[i].texture = false;
			m_support_formats[i].display = false;
			m_support_formats[i].buffer = false;
			m_support_formats[i].index = false;
			m_support_formats[i].framebuffer = false;
		}
	}
	switch (rt) {
	case GRT_TEXTURE:
		m_support_formats[index].texture = true;
		break;
	case GRT_FRAME_BUFFER:
		m_support_formats[index].framebuffer = true;
		break;
	case GRT_VERTEX_BUFFER:
		m_support_formats[index].buffer = true;
		break;
	case GRT_INDEX_BUFFER:
		m_support_formats[index].index = true;
		break;
	case GRT_DISPLAY:
		m_support_formats[index].display = true;
		break;
	default:
		break;
	}
}


//---------------------------------------------------------------------
// check format
//---------------------------------------------------------------------
bool VideoDriver::CheckDeviceFormat(PixelFormat fmt, ResourceType rt)
{
	int index = (int)fmt;
	if (index >= (int)m_support_formats.size()) {
		return false;
	}
	switch (rt) {
	case GRT_TEXTURE:
		return m_support_formats[index].texture;
	case GRT_FRAME_BUFFER:
		return m_support_formats[index].framebuffer;
	case GRT_VERTEX_BUFFER:
		return m_support_formats[index].buffer;
	case GRT_INDEX_BUFFER:
		return m_support_formats[index].index;
	case GRT_DISPLAY:
		return m_support_formats[index].display;
	default:
		break;
	}
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



