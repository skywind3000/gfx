//=====================================================================
//
// GFXDriver.h - 
//
// Last Modified: 2020/04/27 00:01:59
//
//=====================================================================
#ifndef _GFX_DRIVER_H_
#define _GFX_DRIVER_H_

#include "GFX.h"
#include "GFXColor.h"
#include "GFXImage.h"

#include <functional>


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// CreationParameter
//---------------------------------------------------------------------
struct CreationParameter
{
	int adapter;              // adapter index
	int width;                // width of backbuffer
	int height;               // height of backbuffer
	int n_back_buffer;        // number of backbuffer
	void *window_handle;      // window handle
	void *existing_device;    // create from existing device
};


//---------------------------------------------------------------------
// capacity
//---------------------------------------------------------------------
struct VideoCapacity
{
	bool texture_square_only;    // texture must be a square
	bool texture_size_pow2;      // texture size is power of 2
	bool texture_has_alpha;      // texture has alpha
	bool texture_has_mipmap;     // texture has mipmaps
	int texture_max_width;       // texture maximum width
	int texture_max_height;      // texture maximum height
};


//---------------------------------------------------------------------
// VideoDriver
//---------------------------------------------------------------------
class VideoDriver
{
public:
	virtual ~VideoDriver();
	VideoDriver();

public:
	virtual int Create(const CreationParameter *params);

	virtual int Release();

public:

	virtual bool Clear(bool target = true, bool zBuffer = true, bool stencil = true);

	virtual bool BeginScene();
	virtual bool EndScene();
	virtual bool Present();

	virtual bool CheckReset();

	virtual void SetColor(Core::Color color);

	// fit texture size
	virtual bool TextureFit(int w, int h, PixelFormat fmt, int *tw, int *th);

public:
	inline bool IsTextureSquareOnly() const { return m_video_capacity.texture_square_only; }
	inline bool IsTextureHasAlpha() const { return m_video_capacity.texture_has_alpha; }
	inline bool IsTextureHasMipmap() const { return m_video_capacity.texture_has_mipmap; }
	inline bool IsTexturePow2() const { return m_video_capacity.texture_size_pow2; }
	inline int GetTextureMaxW() const { return m_video_capacity.texture_max_width; }
	inline int GetTextureMaxH() const { return m_video_capacity.texture_max_height; }

	inline int GetWidth() const { return m_video_width; }
	inline int GetHeight() const { return m_video_height; }
	inline float GetInvWidth() const { return m_inv_width; }
	inline float GetInvHeight() const { return m_inv_height; }

public:
	std::function<void()> OnPreReset = NULL;
	std::function<void()> OnPostReset = NULL;

protected:
	void InitSize(int width, int height);

protected:
	CreationParameter m_creation_parameter;
	VideoCapacity m_video_capacity;
	Core::Color m_background_color;
	int m_video_width;
	int m_video_height;
	float m_inv_width;
	float m_inv_height;
	bool m_initialized;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif



