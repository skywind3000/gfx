//=====================================================================
//
// GFXVector.h - 
//
// Last Modified: 2020/04/25 22:25:10
//
//=====================================================================
#ifndef _GFX_VECTOR_H_
#define _GFX_VECTOR_H_

#include <sstream>
#include <ostream>
#include <string>

#include "GFXMath.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// Vector2
//---------------------------------------------------------------------
struct Vector2
{
	union {
		struct { float x, y; };
		float m[2];
	};

	inline Vector2() {}
	inline Vector2(const Vector2& v): x(v.x), y(v.y) {}
	inline Vector2(float ix, float iy): x(ix), y(iy) {}
	inline Vector2(const float *v): x(v[0]), y(v[1]) {}

	inline Vector2& operator = (const Vector2& v) {
		x = v.x, y = v.y;
		return *this;
	}

	inline bool operator == (const Vector2& v) const {
		return x == v.x && y == v.y;
	}

	inline bool operator != (const Vector2& v) const {
		return x != v.x || y != v.y;
	}

	inline Vector2 operator + () const {
		return *this;
	}

	inline Vector2 operator - () const {
		return Vector2(-x, -y);
	}

	inline Vector2 operator + (const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}

	inline Vector2 operator - (const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	inline Vector2 operator * (const Vector2& v) const {
		return Vector2(x * v.x, y * v.y);
	}

	inline Vector2 operator * (float scale) const {
		return Vector2(x * scale, y * scale);
	}

	inline Vector2 operator / (float scale) const {
		float inv = 1.0f / scale;
		return Vector2(x * inv, y * inv);
	}

	inline Vector2 operator += (const Vector2& v) {
		x += v.x, y += v.y;
		return *this;
	}

	inline Vector2 operator -= (const Vector2& v) {
		x -= v.x, y -= v.y;
		return *this;
	}

	inline Vector2 operator *= (const Vector2& v) {
		x *= v.x, y *= v.y;
		return *this;
	}

	inline Vector2& operator *= (float scale) {
		x *= scale, y *= scale;
		return *this;
	}

	inline Vector2& operator /= (float scale) {
		float inv = 1.0f / scale;
		x *= inv, y *= inv;
		return *this;
	}

	inline Vector2& Zero() {
		x = y = 0.0f;
		return *this;
	}

	inline Vector2& One() {
		x = y = 1.0f;
		return *this;
	}

	inline Vector2& Set(float x, float y) {
		m[0] = x, m[1] = y;
		return *this;
	}

	inline float LengthSq() const {
		return x * x + y * y;
	}

	inline float Length() const {
		return sqrtf(LengthSq());
	}

	inline Vector2& Normalize() {
		(*this) /= Length();
		return *this;
	}

	inline float Dot(const Vector2& v) const {
		return x * v.x + y * v.y;
	}

	inline Vector2 Cross(const Vector2& v) const {
		Vector2 a;
		a.x = x * v.y - y * v.x;
		a.y = a.x;
		return a;
	}

	inline float Distance(const Vector2& v) const {
		Vector2 a = (*this) - v;
		return a.Length();
	}

	inline float DistanceSq(const Vector2& v) const {
		Vector2 a = (*this) - v;
		return a.LengthSq();
	}

	inline bool NearEqual(const Vector2& v) const {
		return ::GFX::Core::NearZero(DistanceSq(v));
	}

	inline bool NearZero() const {
		return ::GFX::Core::NearZero(LengthSq());
	}

	inline Vector2 Interp(const Vector2& v, float t) const {
		return (*this) + (v - (*this)) * t;
	}

	inline Vector2 LengthClamp(float LengthMin, float LengthMax) const {
		float length = Length();
		if (length >= LengthMin && length <= LengthMax) {
			return (*this);
		}
		else if (length > LengthMax) {
			Vector2 n = (*this) / length;
			return (*this) - (n * (length - LengthMax));
		}	
		else {
			Vector2 n = (*this) / length;
			return (*this) + (n * (LengthMin - length));
		}
	}

	inline std::ostream& Trace(std::ostream& os) const {
		os << "Vector2(" << x << "," << y << ")";
		return os;
	}

	inline std::string ToString() const {
		std::stringstream ss;
		Trace(ss);
		return ss.str();
	}
};


//---------------------------------------------------------------------
// Vector3
//---------------------------------------------------------------------
struct Vector3
{
	union {
		struct { float x, y, z; };
		struct { float r, g, b; };
		float m[3];
	};

	inline Vector3() {}
	inline Vector3(const Vector3& v): x(v.x), y(v.y), z(v.z) {}
	inline Vector3(float ix, float iy, float iz): x(ix), y(iy), z(iz) {}
	inline Vector3(const float *v): x(v[0]), y(v[1]), z(v[2]) {}
	inline Vector3(const Vector2& v, float iz): x(v.x), y(v.y), z(iz) {}

	inline Vector3& operator = (const Vector3& v) {
		x = v.x, y = v.y, z = v.z;
		return *this;
	}

	inline bool operator == (const Vector3& v) {
		return x == v.x && y == v.y && z == v.z;
	}

	inline bool operator != (const Vector3& v) {
		return x != v.x || y != v.y || z != v.z;
	}

	inline Vector3 operator + () {
		return *this;
	}

	inline Vector3 operator - () {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 operator + (const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3 operator - (const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator * (const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	inline Vector3 operator * (float scale) const {
		return Vector3(x * scale, y * scale, z * scale);
	}

	inline Vector3 operator / (float scale) const {
		float inv = 1.0f / scale;
		return Vector3(x * inv, y * inv, z * inv);
	}

	inline Vector3 operator += (const Vector3& v) {
		x += v.x, y += v.y, z += v.z;
		return *this;
	}

	inline Vector3 operator -= (const Vector3& v) {
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}

	inline Vector3 operator *= (const Vector3& v) {
		x *= v.x, y *= v.y, z *= v.z;
		return *this;
	}

	inline Vector3& operator *= (float scale) {
		x *= scale, y *= scale, z *= scale;
		return *this;
	}

	inline Vector3& operator /= (float scale) {
		float inv = 1.0f / scale;
		x *= inv, y *= inv, z *= inv;
		return *this;
	}

	inline Vector3& Zero() {
		x = y = z = 0.0f;
		return *this;
	}

	inline Vector3& One() {
		x = y = z = 1.0f;
		return *this;
	}

	inline Vector3& Set(float x, float y, float z) {
		m[0] = x, m[1] = y, m[2] = z;
		return *this;
	}

	inline float LengthSq() const {
		return x * x + y * y + z * z;
	}

	inline float Length() const {
		return sqrtf(LengthSq());
	}

	inline Vector3& Normalize() {
		(*this) /= Length();
		return *this;
	}

	inline float Dot(const Vector3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline Vector3 Cross(const Vector3& v) const {
		Vector3 a;
		a.x = m[1] * v.m[2] - m[2] * v.m[1];
		a.y = m[2] * v.m[0] - m[0] * v.m[2];
		a.z = m[0] * v.m[1] - m[1] * v.m[0];
		return a;
	}

	inline float Distance(const Vector3& v) const {
		Vector3 a = (*this) - v;
		return a.Length();
	}

	inline float DistanceSq(const Vector3& v) const {
		Vector3 a = (*this) - v;
		return a.LengthSq();
	}

	inline bool NearEqual(const Vector3& v) const {
		return ::GFX::Core::NearZero(DistanceSq(v));
	}

	inline bool NearZero() const {
		return ::GFX::Core::NearZero(LengthSq());
	}

	inline Vector3 Interp(const Vector3& v, float t) const {
		return (*this) + (v - (*this)) * t;
	}

	inline Vector3 LengthClamp(float LengthMin, float LengthMax) const {
		float length = Length();
		if (length >= LengthMin && length <= LengthMax) {
			return (*this);
		}
		else if (length > LengthMax) {
			Vector3 n = (*this) / length;
			return (*this) - (n * (length - LengthMax));
		}	
		else {
			Vector3 n = (*this) / length;
			return (*this) + (n * (LengthMin - length));
		}
	}

	inline std::ostream& Trace(std::ostream& os) const {
		os << "Vector3(" << x << "," << y << "," << z << ")";
		return os;
	}

	inline std::string ToString() const {
		std::stringstream ss;
		Trace(ss);
		return ss.str();
	}
};


//---------------------------------------------------------------------
// Vector4
//---------------------------------------------------------------------
struct Vector4
{
	union {
		struct { float x, y, z, w; };
		struct { float r, g, b, a; };
		float m[4];
	};

	inline Vector4() {}
	inline Vector4(const Vector4& v): x(v.x), y(v.y), z(v.z), w(v.w) {}
	inline Vector4(float ix, float iy, float iz, float iw): x(ix), y(iy), z(iz), w(iw) {}
	inline Vector4(const float *v): x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}
	inline Vector4(const Vector3& v, float iw): x(v.x), y(v.y), z(v.z), w(iw) {}

	inline Vector4& operator = (const Vector4& v) {
		x = v.x, y = v.y, z = v.z, w = v.w;
		return *this;
	}

	inline bool operator == (const Vector4& v) {
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	inline bool operator != (const Vector4& v) {
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}

	inline Vector4 operator + () {
		return *this;
	}

	inline Vector4 operator - () {
		return Vector4(-x, -y, -z, -w);
	}

	inline Vector4 operator + (const Vector4& v) const {
		return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	inline Vector4 operator - (const Vector4& v) const {
		return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	inline Vector4 operator * (const Vector4& v) const {
		return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
	}

	inline Vector4 operator * (float scale) const {
		return Vector4(x * scale, y * scale, z * scale, w * scale);
	}

	inline Vector4 operator / (float scale) const {
		float inv = 1.0f / scale;
		return Vector4(x * inv, y * inv, z * inv, w * inv);
	}

	inline Vector4 operator += (const Vector4& v) {
		x += v.x, y += v.y, z += v.z, w += v.w;
		return *this;
	}

	inline Vector4 operator -= (const Vector4& v) {
		x -= v.x, y -= v.y, z -= v.z, w -= v.w;
		return *this;
	}

	inline Vector4 operator *= (const Vector4& v) {
		x *= v.x, y *= v.y, z *= v.z, w *= v.w;
		return *this;
	}

	inline Vector4& operator *= (float scale) {
		x *= scale, y *= scale, z *= scale, w *= scale;
		return *this;
	}

	inline Vector4& operator /= (float scale) {
		float inv = 1.0f / scale;
		x *= inv, y *= inv, z *= inv, w *= inv;
		return *this;
	}

	inline Vector4& Zero() {
		x = y = z = w = 0.0f;
		return *this;
	}

	inline Vector4& One() {
		x = y = z = w = 1.0f;
		return *this;
	}

	inline Vector4& Set(float x, float y, float z, float w) {
		m[0] = x, m[1] = y, m[2] = z, m[3] = w;
		return *this;
	}

	inline float LengthSq() const {
		return x * x + y * y + z * z + w * w;
	}

	inline float Length() const {
		return sqrtf(LengthSq());
	}

	inline Vector4& Normalize() {
		(*this) /= Length();
		return *this;
	}

	inline float Dot(const Vector4& v) const {
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	inline Vector4 Cross(const Vector4& v) const {
		Vector4 a;
		a.x = m[1] * v.m[2] - m[3] * v.m[2];
		a.y = m[2] * v.m[3] - m[0] * v.m[3];
		a.z = m[3] * v.m[0] - m[1] * v.m[0];
		a.w = m[0] * v.m[1] - m[2] * v.m[1];
		return a;
	}

	inline float Distance(const Vector4& v) const {
		Vector4 a = (*this) - v;
		return a.Length();
	}

	inline float DistanceSq(const Vector4& v) const {
		Vector4 a = (*this) - v;
		return a.LengthSq();
	}

	inline bool NearEqual(const Vector4& v) const {
		return ::GFX::Core::NearZero(DistanceSq(v));
	}

	inline bool NearZero() const {
		return ::GFX::Core::NearZero(LengthSq());
	}

	inline Vector4 Interp(const Vector4& v, float t) const {
		return (*this) + (v - (*this)) * t;
	}

	inline Vector4 LengthClamp(float LengthMin, float LengthMax) const {
		float length = Length();
		if (length >= LengthMin && length <= LengthMax) {
			return (*this);
		}
		else if (length > LengthMax) {
			Vector4 n = (*this) / length;
			return (*this) - (n * (length - LengthMax));
		}	
		else {
			Vector4 n = (*this) / length;
			return (*this) + (n * (LengthMin - length));
		}
	}

	inline std::ostream& Trace(std::ostream& os) const {
		os << "Vector4(" << x << "," << y << "," << z << "," << z << ")";
		return os;
	}

	inline std::string ToString() const {
		std::stringstream ss;
		Trace(ss);
		return ss.str();
	}
};


//---------------------------------------------------------------------
// operators
//---------------------------------------------------------------------
inline Vector2 operator * (float k, const Vector2& v) { return v.operator*(k); }
inline Vector3 operator * (float k, const Vector3& v) { return v.operator*(k); }
inline Vector4 operator * (float k, const Vector4& v) { return v.operator*(k); }

inline std::ostream& operator << (std::ostream& os, const Vector2& v) { return v.Trace(os); }
inline std::ostream& operator << (std::ostream& os, const Vector3& v) { return v.Trace(os); }
inline std::ostream& operator << (std::ostream& os, const Vector4& v) { return v.Trace(os); }


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);


#endif


