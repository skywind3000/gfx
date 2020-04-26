//=====================================================================
//
// GFX.h - 
//
// Last Modified: 2020/04/26 16:33:07
//
//=====================================================================
#ifndef _GFX_H_
#define _GFX_H_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <vector>
#include <string>


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
// Utility
//---------------------------------------------------------------------

bool LoadFile(const char *filename, std::string &content);



//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif


