//=====================================================================
//
// GFXVertex.h - 
//
// Last Modified: 2020/04/26 17:44:29
//
//=====================================================================
#ifndef _GFX_VERTEX_H_
#define _GFX_VERTEX_H_

#include "GFXVector.h"
#include "GFXColor.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// VertexType
//---------------------------------------------------------------------
enum VertexType {
	EVT_STANDARD = 0,
};


//---------------------------------------------------------------------
// Standard
//---------------------------------------------------------------------
struct VertexSt
{
	Vector3 pos;
	Vector3 normal;
	Color color;
	Vector2 tuv;

	inline VertexSt() {}
	inline VertexSt(float x, float y, float z, 
			float nx, float ny, float nz,
			uint32_t col, float u, float v): 
		pos(x, y, z), normal(nx, ny, nz), color(col), tuv(u, v) {}
	inline VertexSt(const Vector3& Pos, const Vector3& Normal,
			const Color cc, const Vector2& Tuv):
		pos(Pos), normal(Normal), color(cc), tuv(Tuv) {}
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);



#endif



