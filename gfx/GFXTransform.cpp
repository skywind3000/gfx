//=====================================================================
//
// GFXTransform.cpp - 
//
// Last Modified: 2020/04/27 01:31:12
//
//=====================================================================
#include "GFXTransform.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// matrixs
//---------------------------------------------------------------------
static float s_zeros[] = {0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f};

static float s_unit2[] = {1.0f, 0.0f,  0.0f, 1.0f};

static float s_unit3[] = {1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  
	0.0f, 0.0f, 1.0f};

static float s_unit4[] = {1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f};

static float s_gl2dx[] = {1.0f, 0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 2.0f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f};

const Matrix2 Matrix2Unit(s_unit2);
const Matrix2 Matrix2Zero(s_zeros);
const Matrix3 Matrix3Unit(s_unit3);
const Matrix3 Matrix3Zero(s_zeros);
const Matrix4 Matrix4Unit(s_unit4);
const Matrix4 Matrix4Zero(s_zeros);
const Matrix4 Matrix4GL2DX(s_gl2dx);


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
Transform::Transform()
{
	m_opengl = false;
	m_dirty = true;
	m_update = true;
	Reset();
}


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
Transform::~Transform()
{
}


//---------------------------------------------------------------------
// reset
//---------------------------------------------------------------------
void Transform::Reset()
{
	m_world = Matrix4Unit;
	m_view = Matrix4Unit;
	m_projection = Matrix4Unit;
	m_mvp = Matrix4Unit;
	m_dirty = false;
}


//---------------------------------------------------------------------
// set transform
//---------------------------------------------------------------------
void Transform::SetTransform(TransformState state, const Matrix4* matrix)
{
	if (matrix == NULL) matrix = &Matrix4Unit;
	m_dirty = true;
	switch (state) {
	case TS_WORLD:
		m_world = *matrix;
		break;
	case TS_VIEW:
		m_view = *matrix;
		break;
	case TS_PROJECTION:
		m_view = *matrix;
		break;
	}
}


//---------------------------------------------------------------------
// get transform
//---------------------------------------------------------------------
void Transform::GetTransform(TransformState state, Matrix4* matrix)
{
	if (matrix == NULL) {
		return;
	}
	switch (state) {
	case TS_WORLD:
		*matrix = m_world;
		break;
	case TS_VIEW:
		*matrix = m_view;
		break;
	case TS_PROJECTION:
		*matrix = m_projection;
		break;
	}
}


//---------------------------------------------------------------------
// Update matrix
//---------------------------------------------------------------------
void Transform::UpdateMvp()
{
	Matrix4 mvp;
	mvp = m_world;
	mvp *= m_view;
	mvp *= m_projection;
	if (m_opengl) {
		mvp *= Matrix4GL2DX;
	}
	m_mvp = mvp;
	m_dirty = false;
}


//---------------------------------------------------------------------
// Get Matrix
//---------------------------------------------------------------------
Matrix4* Transform::GetMvp()
{
	if (m_dirty) {
		if (m_update) {
			UpdateMvp();
			m_dirty = true;
		}
	}
	return &m_mvp;
}


//---------------------------------------------------------------------
// Set Matrix
//---------------------------------------------------------------------
void Transform::SetMvp(const Matrix4 *matrix)
{
	m_mvp = *matrix;
	m_dirty = false;
}



//=====================================================================
// matrix utils
//=====================================================================

void Matrix4_SetIdentity(Matrix4& m) {
	m = Matrix4Unit;
}

void Matrix4_SetZero(Matrix4& m) {
	m.m[0][0] = m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
	m.m[1][0] = m.m[1][1] = m.m[1][2] = m.m[1][3] = 0.0f;
	m.m[2][0] = m.m[2][1] = m.m[2][2] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = m.m[3][3] = 0.0f;
}

void Matrix4_SetTranslate(Matrix4& m, float x, float y, float z) {
	Matrix4_SetIdentity(m);
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
}

void Matrix4_SetScale(Matrix4& m, float x, float y, float z) {
	Matrix4_SetIdentity(m);
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
}

void Matrix4_SetRotate(Matrix4& m, float x, float y, float z, float theta) {
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

void Matrix4_LookAt(Matrix4& m, const Vector4& eye, const Vector4& at, const Vector4& up) {
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
void Matrix4_SetPerspective(Matrix4& m, float fovy, float aspect, float zn, float zf) {
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	Matrix4_SetZero(m);
	m.m[0][0] = (float)(fax / aspect);
	m.m[1][1] = (float)(fax);
	m.m[2][2] = zf / (zf - zn);
	m.m[3][2] = - zn * zf / (zf - zn);
	m.m[2][3] = 1.0f;
}

// D3DXMatrixOrthoOffCenterLH
void Matrix4_SetOrtho2D(Matrix4& m, float l, float r, float b, float t, float zn, float zf)
{
	Matrix4_SetZero(m);
	m.m[0][0] = 2.0f / (r - l);
	m.m[1][1] = 2.0f / (t - b);
	m.m[2][2] = 1.0f / (zf - zn);
	m.m[3][3] = 1.0f;
	m.m[3][0] = (l + r) / (l - r);
	m.m[3][1] = (t + b) / (b - t);
	m.m[3][2] = zn / (zn - zf);
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);



