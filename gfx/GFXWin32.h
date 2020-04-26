//=====================================================================
//
// GFXWin32.h - 
//
// Last Modified: 2020/04/26 20:28:39
//
//=====================================================================
#ifndef _GFX_WIN32_H_
#define _GFX_WIN32_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <mmsystem.h>
#include <combaseapi.h>

#include "GFX.h"
#include "GFXImage.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Win32);


//---------------------------------------------------------------------
// GdiPlus
//---------------------------------------------------------------------

int GdiPlus_Init(int startup);

// load image from memory with GdiPlus
GFX::Image* GdiPlus_LoadImage(const void *data, long size, int *errcode);

// load image from file with GdiPlus
GFX::Image* GdiPlus_LoadFile(const char *filename);


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Win32);
NAMESPACE_END(GFX);


#endif


