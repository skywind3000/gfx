//=====================================================================
//
// GFXTransform.h - 
//
// Last Modified: 2020/04/26 10:31:08
//
//=====================================================================
#ifndef _GFX_TRANSFORM_H_
#define _GFX_TRANSFORM_H_

#include <vector>

#include "GFXMatrix.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// Global
//---------------------------------------------------------------------
const extern Matrix2 Matrix2Unit;
const extern Matrix2 Matrix2Zero;
const extern Matrix3 Matrix3Unit;
const extern Matrix3 Matrix3Zero;
const extern Matrix4 Matrix4Unit;
const extern Matrix4 Matrix4Zero;
const extern Matrix4 Matrix4GL2DX;


//---------------------------------------------------------------------
// Transform state
//---------------------------------------------------------------------
enum TransformState {
	TS_WORLD = 0,
	TS_VIEW,
	TS_PROJECTION,
};


//---------------------------------------------------------------------
// Transform
//---------------------------------------------------------------------
class Transform
{
public:
	virtual ~Transform();
	Transform();

public:
	void SetTransform(TransformState state, const Matrix4* matrix);
	void Reset();

	void GetTransform(TransformState state, Matrix4* matrix);

	void UpdateMvp();

	Matrix4* GetMvp();

	void SetMvp(const Matrix4 *matrix);

protected:
	bool m_opengl;
	bool m_dirty;
	bool m_update;

protected:
	Matrix4 m_world;
	Matrix4 m_view;
	Matrix4 m_projection;
	Matrix4 m_mvp;
};


//---------------------------------------------------------------------
// Matrix Utils
//---------------------------------------------------------------------

void Matrix4_SetIdentity(Matrix4& m);

void Matrix4_SetZero(Matrix4& m);

void Matrix4_SetTranslate(Matrix4& m, float x, float y, float z);

void Matrix4_SetScale(Matrix4& m, float x, float y, float z);

void Matrix4_SetRotate(Matrix4& m, float x, float y, float z, float theta);

void Matrix4_LookAt(Matrix4& m, const Vector4& eye, const Vector4& at, const Vector4& up);

// D3DXMatrixPerspectiveFovLH
void Matrix4_SetPerspective(Matrix4& m, float fovy, float aspect, float zn, float zf);

// D3DXMatrixOrthoOffCenterLH
void Matrix4_SetOrtho2D(Matrix4& m, float l, float r, float b, float t, float zn, float zf);


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);

#endif


