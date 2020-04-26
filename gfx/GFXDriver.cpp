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
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


