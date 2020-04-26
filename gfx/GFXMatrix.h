//=====================================================================
//
// GFXMatrix.h - 
//
// Last Modified: 2020/04/26 05:48:03
//
//=====================================================================
#ifndef _GFX_MATRIX_H_
#define _GFX_MATRIX_H_

#include "GFXMath.h"
#include "GFXVector.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// Matrix2
//---------------------------------------------------------------------
struct Matrix2
{
	union {
		struct {
			float m00, m01;
			float m10, m11;
		};
		float m[2][2];
	};
	
	inline Matrix2() {}

	inline Matrix2(const Matrix2& s) {
		m00 = s.m00, m01 = s.m01;
		m10 = s.m10, m11 = s.m11;
	}

	inline Matrix2(const float *m) {
		m00 = m[0], m01 = m[1];
		m10 = m[2], m11 = m[3];
	}

	inline Matrix2& operator = (const Matrix2& s) {
		m00 = s.m00, m01 = s.m01;
		m10 = s.m10, m11 = s.m11;
		return *this;
	}

	inline Matrix2& Load(const float *m) {
		m00 = m[0], m01 = m[1];
		m10 = m[2], m11 = m[3];
		return *this;
	}

	inline Matrix2& SetZero() {
		m00 = m01 = 0.0f;
		m10 = m11 = 0.0f;
		return *this;
	}

	inline Matrix2& SetIdentity() {
		m00 = 1.0f, m01 = 0.0f;
		m10 = 0.0f, m11 = 1.0f;
		return *this;
	}

	inline Matrix2& operator += (const Matrix2& s) {
		m00 += s.m00; m01 += s.m01;
		m10 += s.m10; m11 += s.m11;
		return *this;
	}

	inline Matrix2& operator -= (const Matrix2& s) {
		m00 -= s.m00; m01 -= s.m01;
		m10 -= s.m10; m11 -= s.m11;
		return *this;
	}

	inline Matrix2& operator *= (float k) {
		m00 *= k; m01 *= k;
		m10 *= k; m11 *= k;
		return *this;
	}

	inline Matrix2& operator /= (float k) {
		m00 /= k; m01 /= k;
		m10 /= k; m11 /= k;
		return *this;
	}

	inline Matrix2 operator + (const Matrix2& s) {
		Matrix2 t(*this);
		t += s;
		return t;
	}

	inline Matrix2 operator - (const Matrix2& s) {
		Matrix2 t(*this);
		t -= s;
		return t;
	}

	inline Matrix2 operator * (float k) {
		Matrix2 t(*this);
		t *= k;
		return t;
	}

	inline Matrix2 operator / (float k) {
		Matrix2 t(*this);
		t /= k;
		return t;
	}

	inline Matrix2 operator * (const Matrix2& s) const {
		Matrix2 t;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				t.m[j][i] = (m[j][0] * s.m[0][i]) +
							(m[j][1] * s.m[1][i]);
			}
		}
		return t;
	}

	inline Matrix2& operator *= (const Matrix2& s) {
		Matrix2 t(*this);
		this->operator=(t.operator*(s));
		return *this;
	}

	inline Vector2 Transform(const Vector2& v) const {
		Vector2 t;
		t.m[0] = v.m[0] * m00 + v.m[1] * m10;
		t.m[1] = v.m[0] * m01 + v.m[1] * m11;
		return t;
	}
};


//---------------------------------------------------------------------
// Matrix3
//---------------------------------------------------------------------
struct Matrix3
{
	union {
		struct {
			float m00, m01, m02;
			float m10, m11, m12;
			float m20, m21, m22;
		};
		float m[3][3];
	};

	inline Matrix3() {}

	inline Matrix3(const Matrix3& s) {
		m00 = s.m00, m01 = s.m01, m02 = s.m02;
		m10 = s.m10, m11 = s.m11, m12 = s.m12;
		m20 = s.m20, m21 = s.m21, m22 = s.m22;
	}

	inline Matrix3(const float *m) {
		m00 = m[0], m01 = m[1], m02 = m[2];
		m10 = m[3], m11 = m[4], m12 = m[5];
		m20 = m[6], m21 = m[7], m22 = m[8];
	}

	inline Matrix3& operator = (const Matrix3& s) {
		m00 = s.m00, m01 = s.m01, m02 = s.m02;
		m10 = s.m10, m11 = s.m11, m12 = s.m12;
		m20 = s.m20, m21 = s.m21, m22 = s.m22;
		return *this;
	}

	inline Matrix3& Load(const float *m) {
		m00 = m[0], m01 = m[1], m02 = m[2];
		m10 = m[3], m11 = m[4], m12 = m[5];
		m20 = m[6], m21 = m[7], m22 = m[8];
		return *this;
	}

	inline Matrix3& SetZero() {
		m00 = m01 = m02 = 0.0f;
		m10 = m11 = m12 = 0.0f;
		m20 = m21 = m22 = 0.0f;
		return *this;
	}

	inline Matrix3& SetIdentity() {
		m00 = 1.0f, m01 = 0.0f, m02 = 0.0f;
		m10 = 0.0f, m11 = 1.0f, m12 = 0.0f;
		m20 = 0.0f, m21 = 0.0f, m22 = 1.0f;
		return *this;
	}

	inline Matrix3& operator += (const Matrix3& s) {
		m00 += s.m00; m01 += s.m01; m02 += s.m02;
		m10 += s.m10; m11 += s.m11; m12 += s.m12;
		m20 += s.m20; m21 += s.m21; m22 += s.m22;
		return *this;
	}

	inline Matrix3& operator -= (const Matrix3& s) {
		m00 -= s.m00; m01 -= s.m01; m02 -= s.m02;
		m10 -= s.m10; m11 -= s.m11; m12 -= s.m12;
		m20 -= s.m20; m21 -= s.m21; m22 -= s.m22;
		return *this;
	}

	inline Matrix3& operator *= (float k) {
		m00 *= k; m01 *= k; m02 *= k;
		m10 *= k; m11 *= k; m12 *= k;
		m20 *= k; m21 *= k; m22 *= k;
		return *this;
	}

	inline Matrix3& operator /= (float k) {
		m00 /= k; m01 /= k; m02 /= k;
		m10 /= k; m11 /= k; m12 /= k;
		m20 /= k; m21 /= k; m22 /= k;
		return *this;
	}

	inline Matrix3 operator + (const Matrix3& s) {
		Matrix3 t(*this);
		t += s;
		return t;
	}

	inline Matrix3 operator - (const Matrix3& s) {
		Matrix3 t(*this);
		t -= s;
		return t;
	}

	inline Matrix3 operator * (float k) {
		Matrix3 t(*this);
		t *= k;
		return t;
	}

	inline Matrix3 operator / (float k) {
		Matrix3 t(*this);
		t /= k;
		return t;
	}

	inline Matrix3 operator * (const Matrix3& s) const {
		Matrix3 t;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				t.m[j][i] = (m[j][0] * s.m[0][i]) +
							(m[j][1] * s.m[1][i]) +
							(m[j][2] * s.m[2][i]);
			}
		}
		return t;
	}

	inline Matrix3& operator *= (const Matrix3& s) {
		Matrix3 t(*this);
		this->operator=(t.operator*(s));
		return *this;
	}

	inline Vector3 Transform(const Vector3& v) const {
		Vector3 t;
		t.m[0] = v.m[0] * m00 + v.m[1] * m10 + v.m[2] * m20;
		t.m[1] = v.m[0] * m01 + v.m[1] * m11 + v.m[2] * m21;
		t.m[2] = v.m[0] * m02 + v.m[1] * m12 + v.m[2] * m22;
		return t;
	}

	inline Vector2 Transform(const Vector2& v) const {
		Vector2 t;
		t.m[0] = v.m[0] * m00 + v.m[1] * m10 + m20;
		t.m[1] = v.m[0] * m01 + v.m[1] * m11 + m21;
		return t;
	}
};


//---------------------------------------------------------------------
// Matrix4
//---------------------------------------------------------------------
struct Matrix4
{
	union {
		struct {
			float m00, m01, m02, m03;
			float m10, m11, m12, m13;
			float m20, m21, m22, m23;
			float m30, m31, m32, m33;
		};
		float m[4][4];
	};

	inline Matrix4() {}

	inline Matrix4(const Matrix4& s) {
		m00 = s.m00, m01 = s.m01, m02 = s.m02, m03 = s.m03;
		m10 = s.m10, m11 = s.m11, m12 = s.m12, m13 = s.m13;
		m20 = s.m20, m21 = s.m21, m22 = s.m22, m23 = s.m23;
		m30 = s.m30, m31 = s.m31, m32 = s.m32, m33 = s.m33;
	}

	inline Matrix4(const float *m) {
		m00 = m[ 0], m01 = m[ 1], m02 = m[ 2], m03 = m[ 3];
		m10 = m[ 4], m11 = m[ 5], m12 = m[ 6], m13 = m[ 7];
		m20 = m[ 8], m21 = m[ 9], m22 = m[10], m23 = m[11];
		m30 = m[12], m31 = m[13], m32 = m[14], m33 = m[15];
	}

	inline Matrix4& operator = (const Matrix4& s) {
		m00 = s.m00, m01 = s.m01, m02 = s.m02, m03 = s.m03;
		m10 = s.m10, m11 = s.m11, m12 = s.m12, m13 = s.m13;
		m20 = s.m20, m21 = s.m21, m22 = s.m22, m23 = s.m23;
		m30 = s.m30, m31 = s.m31, m32 = s.m32, m33 = s.m33;
		return *this;
	}

	inline Matrix4& Load(const float *m) {
		m00 = m[ 0], m01 = m[ 1], m02 = m[ 2], m03 = m[ 3];
		m10 = m[ 4], m11 = m[ 5], m12 = m[ 6], m13 = m[ 7];
		m20 = m[ 8], m21 = m[ 9], m22 = m[10], m23 = m[11];
		m30 = m[12], m31 = m[13], m32 = m[14], m33 = m[15];
		return *this;
	}

	inline Matrix4& SetZero() {
		m00 = m01 = m02 = m03 = 0.0f;
		m10 = m11 = m12 = m13 = 0.0f;
		m20 = m21 = m22 = m23 = 0.0f;
		m30 = m31 = m32 = m33 = 0.0f;
		return *this;
	}

	inline Matrix4& SetIdentity() {
		m00 = 1.0f, m01 = 0.0f, m02 = 0.0f, m03 = 0.0f;
		m10 = 0.0f, m11 = 1.0f, m12 = 0.0f, m13 = 0.0f;
		m20 = 0.0f, m21 = 0.0f, m22 = 1.0f, m23 = 0.0f;
		m30 = 0.0f, m31 = 0.0f, m32 = 0.0f, m33 = 1.0f;
		return *this;
	}

	inline Matrix4& operator += (const Matrix4& s) {
		m00 += s.m00; m01 += s.m01; m02 += s.m02; m03 += s.m03;
		m10 += s.m10; m11 += s.m11; m12 += s.m12; m13 += s.m13;
		m20 += s.m20; m21 += s.m21; m22 += s.m22; m23 += s.m23;
		m30 += s.m00; m31 += s.m31; m32 += s.m32; m33 += s.m33;
		return *this;
	}

	inline Matrix4& operator -= (const Matrix4& s) {
		m00 -= s.m00; m01 -= s.m01; m02 -= s.m02; m03 -= s.m03;
		m10 -= s.m10; m11 -= s.m11; m12 -= s.m12; m13 -= s.m13;
		m20 -= s.m20; m21 -= s.m21; m22 -= s.m22; m23 -= s.m23;
		m30 -= s.m00; m31 -= s.m31; m32 -= s.m32; m33 -= s.m33;
		return *this;
	}

	inline Matrix4& operator *= (float k) {
		m00 *= k; m01 *= k; m02 *= k; m03 *= k;
		m10 *= k; m11 *= k; m12 *= k; m13 *= k;
		m20 *= k; m21 *= k; m22 *= k; m23 *= k;
		m30 *= k; m31 *= k; m32 *= k; m33 *= k;
		return *this;
	}

	inline Matrix4& operator /= (float k) {
		m00 /= k; m01 /= k; m02 /= k; m03 /= k;
		m10 /= k; m11 /= k; m12 /= k; m13 /= k;
		m20 /= k; m21 /= k; m22 /= k; m23 /= k;
		m30 /= k; m31 /= k; m32 /= k; m33 /= k;
		return *this;
	}

	inline Matrix4 operator + (const Matrix4& s) {
		Matrix4 t(*this);
		t += s;
		return t;
	}

	inline Matrix4 operator - (const Matrix4& s) {
		Matrix4 t(*this);
		t -= s;
		return t;
	}

	inline Matrix4 operator * (float k) {
		Matrix4 t(*this);
		t *= k;
		return t;
	}

	inline Matrix4 operator / (float k) {
		Matrix4 t(*this);
		t /= k;
		return t;
	}

	inline Matrix4 operator * (const Matrix4& s) const {
		Matrix4 t;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				t.m[j][i] = (m[j][0] * s.m[0][i]) +
							(m[j][1] * s.m[1][i]) +
							(m[j][2] * s.m[2][i]) +
							(m[j][3] * s.m[3][i]);
			}
		}
		return t;
	}

	inline Matrix4& operator *= (const Matrix4& s) {
		Matrix4 t(*this);
		this->operator=(t.operator*(s));
		return *this;
	}

	inline Vector4 Transform(const Vector4& v) const {
		Vector4 t;
		t.m[0] = v.m[0] * m00 + v.m[1] * m10 + v.m[2] * m20 + v.m[3] * m30;
		t.m[1] = v.m[0] * m01 + v.m[1] * m11 + v.m[2] * m21 + v.m[3] * m31;
		t.m[2] = v.m[0] * m02 + v.m[1] * m12 + v.m[2] * m22 + v.m[3] * m32;
		t.m[3] = v.m[0] * m03 + v.m[1] * m13 + v.m[2] * m23 + v.m[3] * m33;
		return t;
	}

	inline Vector3 Transform(const Vector3& v) const {
		Vector3 t;
		t.m[0] = v.m[0] * m00 + v.m[1] * m10 + v.m[2] * m20 + m30;
		t.m[1] = v.m[0] * m01 + v.m[1] * m11 + v.m[2] * m21 + m31;
		t.m[2] = v.m[0] * m02 + v.m[1] * m12 + v.m[2] * m22 + m32;
		return t;
	}
};


//---------------------------------------------------------------------
// operators
//---------------------------------------------------------------------
inline Vector4 operator * (const Vector4& v, const Matrix4& m) { return m.Transform(v); }
inline Vector3 operator * (const Vector3& v, const Matrix4& m) { return m.Transform(v); }
inline Vector3 operator * (const Vector3& v, const Matrix3& m) { return m.Transform(v); }
inline Vector2 operator * (const Vector2& v, const Matrix3& m) { return m.Transform(v); }
inline Vector2 operator * (const Vector2& v, const Matrix2& m) { return m.Transform(v); }


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);


#endif


