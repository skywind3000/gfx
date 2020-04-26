//=====================================================================
//
// GFXTransform.h - 
//
// Last Modified: 2020/04/26 10:31:08
//
//=====================================================================
#ifndef _GFX_TRANSFORM_H_
#define _GFX_TRANSFORM_H_

#include "GFXMatrix.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);

//---------------------------------------------------------------------
// Transform
//---------------------------------------------------------------------

static inline void Matrix4_SetIdentity(Matrix4& m) {
	m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f; 
	m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
	m.m[1][0] = m.m[1][2] = m.m[1][3] = 0.0f;
	m.m[2][0] = m.m[2][1] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
}

static inline void Matrix4_SetZero(Matrix4& m) {
	m.m[0][0] = m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
	m.m[1][0] = m.m[1][1] = m.m[1][2] = m.m[1][3] = 0.0f;
	m.m[2][0] = m.m[2][1] = m.m[2][2] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = m.m[3][3] = 0.0f;
}

static inline void Matrix4_SetTranslate(Matrix4& m, float x, float y, float z) {
	Matrix4_SetIdentity(m);
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
}

static inline void Matrix4_SetScale(Matrix4& m, float x, float y, float z) {
	Matrix4_SetIdentity(m);
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
}

static inline void Matrix4_SetRotate(Matrix4& m, float x, float y, float z, float theta) {
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	Vector4 vec(x, y, z, 1.0f);
	float w = qcos;
	vec.Normalize();
	x = vec.x * qsin;
	y = vec.y * qsin;
	z = vec.z * qsin;
	m.m[0][0] = 1 - 2 * y * y - 2 * z * z;
	m.m[1][0] = 2 * x * y - 2 * w * z;
	m.m[2][0] = 2 * x * z + 2 * w * y;
	m.m[0][1] = 2 * x * y + 2 * w * z;
	m.m[1][1] = 1 - 2 * x * x - 2 * z * z;
	m.m[2][1] = 2 * y * z - 2 * w * x;
	m.m[0][2] = 2 * x * z - 2 * w * y;
	m.m[1][2] = 2 * y * z + 2 * w * x;
	m.m[2][2] = 1 - 2 * x * x - 2 * y * y;
	m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;	
	m.m[3][3] = 1.0f;
}

static inline void Matrix4_LookAt(Matrix4& m, const Vector4& eye, const Vector4& at, const Vector4& up) {
	Vector4 xaxis, yaxis, zaxis;

	zaxis = at - eye;
	zaxis.Normalize();
	xaxis = up.Cross(zaxis);
	xaxis.Normalize();
	yaxis = zaxis.Cross(xaxis);
	// yaxis.Normalize();

	m.m[0][0] = xaxis.x;
	m.m[1][0] = xaxis.y;
	m.m[2][0] = xaxis.z;
	m.m[3][0] = -xaxis.Dot(eye);

	m.m[0][1] = yaxis.x;
	m.m[1][1] = yaxis.y;
	m.m[2][1] = yaxis.z;
	m.m[3][1] = -yaxis.Dot(eye);

	m.m[0][2] = zaxis.x;
	m.m[1][2] = zaxis.y;
	m.m[2][2] = zaxis.z;
	m.m[3][2] = -zaxis.Dot(eye);
	
	m.m[0][3] = m.m[1][3] = m.m[2][3] = 0.0f;
	m.m[3][3] = 1.0f;
}

// D3DXMatrixPerspectiveFovLH
static inline void Matrix4_SetPerspective(Matrix4& m, float fovy, float aspect, float zn, float zf) {
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	Matrix4_SetZero(m);
	m.m[0][0] = (float)(fax / aspect);
	m.m[1][1] = (float)(fax);
	m.m[2][2] = zf / (zf - zn);
	m.m[3][2] = - zn * zf / (zf - zn);
	m.m[2][3] = 1;
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);

#endif


