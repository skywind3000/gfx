//=====================================================================
//
// GFXTypes.h - 
//
// Last Modified: 2020/04/27 02:24:16
//
//=====================================================================
#ifndef _GFX_TYPES_H_
#define _GFX_TYPES_H_

#include <stddef.h>
#include <stdint.h>

#ifndef __cplusplus
#error This file can only be compiled in C++ mode !!
#endif


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
// Rect
//---------------------------------------------------------------------
struct Rect
{
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};


//---------------------------------------------------------------------
// Float Rectangle
//---------------------------------------------------------------------
struct RectFloat
{
	float left;
	float top;
	float right;
	float bottom;
};


//---------------------------------------------------------------------
// Size
//---------------------------------------------------------------------
struct Size
{
	int32_t width;
	int32_t height;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



#endif



