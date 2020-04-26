//=====================================================================
//
// GFX.cpp - 
//
// Last Modified: 2020/04/26 23:43:28
//
//=====================================================================
#include "GFX.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// Utility
//---------------------------------------------------------------------
bool LoadFile(const char *filename, std::string &content) 
{
	long size = 0;
	FILE *fp = fopen(filename, "rb");
	if (fp == NULL) return false;
	fseek(fp, 0, SEEK_END);
	size = (long)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	content.resize(size);
	if (size > 0) {
		for (char *ptr = &content[0]; size > 0; ) {
			long hr = (long)fread(ptr, 1, size, fp);
			if (hr <= 0) break;
			ptr += hr;
			size -= hr;
		}
	}
	fclose(fp);
	return true;
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


