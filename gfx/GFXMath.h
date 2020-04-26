//=====================================================================
//
// GFXMath.h - 
//
// Last Modified: 2020/04/22 01:18:43
//
//=====================================================================

#ifndef _GFX_MATH_H_
#define _GFX_MATH_H_

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>


#ifndef __cplusplus
#error This file must be compiled in C++ mode !!
#endif


//---------------------------------------------------------------------
// Platform Detect
//---------------------------------------------------------------------
#define GFX_PLATFORM_NONE		0
#define GFX_PLATFORM_SSE2		1
#define GFX_PLATFORM_AVX		2
#define GFX_PLATFORM_AVX2		3
#define GFX_PLATFORM_NEON		4

#ifndef GFX_PLATFORM
#define GFX_PLATFORM		GFX_PLATFORM_NONE
#endif

#ifndef GFX_ENABLE_FAST_MATH
#define GFX_ENABLE_FAST_MATH	0
#endif


#if GFX_PLATFORM == GFX_PLATFORM_SSE2
#include <xmmintrin.h>
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
// Alignment
//---------------------------------------------------------------------
#ifdef __GNUC__
#define GFX_ALIGNED_DATA(x) __attribute__ ((aligned(x)))
#define GFX_ALIGNED_STRUCT(x) struct __attribute__ ((aligned(x)))
#else
#define GFX_ALIGNED_DATA(x) __declspec(align(x))
#define GFX_ALIGNED_STRUCT(x) __declspec(align(x)) struct
#endif


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);
NAMESPACE_BEGIN(Core);


//---------------------------------------------------------------------
// consts
//---------------------------------------------------------------------
const float PI       = 3.141592654f;
const float PI2      = 6.283185307f;
const float PI_DIV_2 = 1.570796327f;
const float PI_DIV_4 = 0.785398163f;
const float PI_INV   = 0.318309886f;

const float EPSILON_E4 = ((float)1E-4);
const float EPSILON_E5 = ((float)1E-5);
const float EPSILON_E6 = ((float)1E-6);

const float DEG_TO_RAD = PI / 180.0f;
const float RAD_TO_DEG = 180.0f / PI;


//---------------------------------------------------------------------
// inlines
//---------------------------------------------------------------------
template<class T>
inline T Abs(const T& x) {
	return (x < ((T)0))? (-x) : x;
}

template<class T>
inline const T& Min(const T& a, const T& b) {
	return (a < b)? a : b;
}

template<class T>
inline const T& Max(const T& a, const T& b) {
	return (a > b)? a : b;
}

template<class T>
inline const T& Clamp(const T& value, const T& low, const T& high) {
	return Min(high, Max(value, low));
}

template<class T>
inline bool IsBetween(const T& value, const T& low, const T& high) {
	return (value >= low) && (value <= high);
}

template<class T>
inline bool IsZero(const T& x) {
	return x == ((T)0);
}

template<class T>
inline bool IsOne(const T& x) {
	return x == ((T)1);
}

template<class T>
inline bool Equal(const T& x, const T& y) {
	return x == y;
}

// linear interpolation
template<class T>
inline T Lerp(const T& a, const T& b, float t) {
	return (T)(a + (b - a) * t);
}

// returns if x equals y, taking possible rounding errors into account
inline bool NearEqual(float x, float y, float error = EPSILON_E6) {
	return (x >= y - error) && (x <= y + error);
}

// returns if x equals zero, taking possible rounding errors into account
inline bool NearZero(float x, float error = EPSILON_E6) {
	return (x >= -error) && (x <= error);
}

inline float RadToDeg(float radius) {
	return RAD_TO_DEG * radius;
}

inline float DegToRad(float degree) {
	return DEG_TO_RAD * degree;
}


//---------------------------------------------------------------------
// Platform
//---------------------------------------------------------------------
inline float SquareRoot(float x) {
#if GFX_PLATFORM == GFX_PLATFORM_NONE
	return sqrtf(x);
#elif GFX_PLATFORM == GFX_PLATFORM_SSE2
	float y;
	__m128 in = _mm_load_ss(&x);
	_mm_store_ss(&y, _mm_sqrt_ss(in));
	return y;
#endif
}

inline float InverseSquareRoot(float x) {
#if GFX_PLATFORM == GFX_PLATFORM_NONE
	#if GFX_ENABLE_FAST_MATH == 0
	return 1.0f / SquareRoot(x);
	#else
	union { int32_t intpart; float floatpart; } convert;
	float xhalf = 0.5f * x, y;
	convert.floatpart = x;
	convert.intpart = 0x5f3759df - (convert.intpart >> 1);
	y = convert.floatpart;
	y = y * (1.5f - xhalf * y * y);
	y = y * (1.5f - xhalf * y * y);
	return y;
	#endif
#elif GFX_PLATFORM == GFX_PLATFORM_SSE2
	#if GFX_ENABLE_FAST_MATH == 0
	float y, x2;
	x2 = x * 0.5F;
	__m128 in = _mm_load_ss(&x);
	_mm_store_ss( &y, _mm_rsqrt_ss(in));
	return y * (1.5f - (x2 * y * y));
	#else
	float y;
	__m128 in = _mm_load_ss( &pIn );
	_mm_store_ss( &y, _mm_rsqrt_ss( in ) );
	return y;
	#endif
#endif
}



//---------------------------------------------------------------------
// namespace endup
//---------------------------------------------------------------------
NAMESPACE_END(Core);
NAMESPACE_END(GFX);



#endif



