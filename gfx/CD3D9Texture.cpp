//=====================================================================
//
// CD3D9Texture.cpp - 
//
// Last Modified: 2020/04/26 16:59:09
//
//=====================================================================
#include "CD3D9Texture.h"
#include "GFXWin32.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// get format
//---------------------------------------------------------------------
D3DFORMAT CD3D9Texture::GetD3DFormat(PixelFormat fmt)
{
	D3DFORMAT f = D3DFMT_A8R8G8B8;
	switch (fmt) {
		case FMT_A8R8G8B8:
			f = D3DFMT_A8R8G8B8;
			break;
		case FMT_A8B8G8R8:
			f = D3DFMT_A8B8G8R8;
			break;
		case FMT_X8R8G8B8:
			f = D3DFMT_X8R8G8B8;
			break;
		case FMT_A1R5G5B5:
			f = D3DFMT_A1R5G5B5;
			break;
		case FMT_A4R4G4B4:
			f = D3DFMT_A4R4G4B4;
			break;
		case FMT_R5G6B5:
			f = D3DFMT_R5G6B5;
			break;
		case FMT_R8G8B8:
		case FMT_B8G8R8:
			f = D3DFMT_R8G8B8;
			break;
		case FMT_G8:
			f = D3DFMT_L8;
			break;
		default:
			f = D3DFMT_UNKNOWN;
			break;
	}
	return f;
}


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
CD3D9Texture::CD3D9Texture()
{
	m_texture = NULL;
	m_device = NULL;
	m_surface = NULL;
}


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
CD3D9Texture::~CD3D9Texture()
{
	Release();
}


//---------------------------------------------------------------------
// release
//---------------------------------------------------------------------
int CD3D9Texture::Release()
{
	if (m_surface) {
		m_surface->Release();
		m_surface = NULL;
	}
	if (m_texture) {
		m_texture->Release();
		m_texture = NULL;
		if (m_device) {
			m_device->Release();
			m_device = NULL;
		}
	}
	return 0;
}


//---------------------------------------------------------------------
// create with parameters
//---------------------------------------------------------------------
int CD3D9Texture::Create(CD3D9Driver *drv, int w, int h, PixelFormat fmt, int flag, int mipmap)
{
	Release();
	int hr = CreateTexture(drv->GetDevice(), w, h, fmt, flag, mipmap);
	return hr;
}


//---------------------------------------------------------------------
// create with parameters
//---------------------------------------------------------------------
int CD3D9Texture::Create(CD3D9Driver *drv, const Image *image, int flag, int mipmap)
{
	Release();
	int hr = CreateTexture(drv->GetDevice(), image->GetWidth(), image->GetHeight(),
			image->GetFormat(), flag, mipmap);
	if (hr == 0) {
		this->RestoreFromImage(0, image);
	}
	return hr;
}


//---------------------------------------------------------------------
// create from file
//---------------------------------------------------------------------
int CD3D9Texture::Create(CD3D9Driver *drv, const char *filename, int flag, int mipmap)
{
	GFX::Image *img = Win32::GdiPlus_LoadFile(filename);
	if (img == NULL) {
		// printf("Load failed\n");
		return -1;
	}
	// printf("fmt=%d bpp=%d\n", (int)img->GetFormat(), (int)img->GetBpp());
	int hr = Create(drv, img, flag, mipmap);
	delete img;
	return hr;
}


//---------------------------------------------------------------------
// create texture
//---------------------------------------------------------------------
int CD3D9Texture::CreateTexture(IDirect3DDevice9 *device, int w, int h, 
		PixelFormat fmt, int flag, int mipmap)
{
	D3DFORMAT d3d_fmt = GetD3DFormat(fmt);
	DWORD usage = 0;
	D3DPOOL pool = D3DPOOL_MANAGED;
	m_texture = NULL;
	if (flag == 0xffff) {
		usage = D3DUSAGE_RENDERTARGET;
		pool = D3DPOOL_DEFAULT;
	}
	HRESULT hr = device->CreateTexture(w, h, mipmap, usage, d3d_fmt, pool, &m_texture, NULL);
	if (SUCCEEDED(hr)) {
		InitSize(w, h, fmt, true);
		device->AddRef();
		m_device = device;
		m_levels = m_texture->GetLevelCount();
	}
	else {
		m_texture = NULL;
		return -1;
	}
	m_locked_pitch = 0;
	m_locked_bits = NULL;
	return 0;
}


//---------------------------------------------------------------------
// lock
//---------------------------------------------------------------------
void* CD3D9Texture::Lock(int mip, const Rect *rect, bool readOnly)
{
	if (m_texture == NULL) {
		return NULL;
	}

	if (m_locked_bits) {
		return m_locked_bits;
	}

	RECT rc;

	if (rect == NULL) {
		rc.left = 0;
		rc.top = 0;
		rc.right = GetLevelWidth(mip);
		rc.bottom = GetLevelHeight(mip);
	}
	else {
		rc.left = rect->left;
		rc.top = rect->top;
		rc.right = rect->right;
		rc.bottom = rect->bottom;
	}

	D3DLOCKED_RECT locked;
	HRESULT hr = m_texture->LockRect(mip, &locked, &rc, 
			readOnly? D3DLOCK_READONLY : 0);

	if (SUCCEEDED(hr)) {
		m_locked_pitch = (int32_t)locked.Pitch;
		m_locked_bits = (uint8_t*)locked.pBits;
		m_locked_w = rc.right - rc.left;
		m_locked_h = rc.bottom - rc.top;
	}

	return m_locked_bits;
}


//---------------------------------------------------------------------
// unlock
//---------------------------------------------------------------------
void CD3D9Texture::Unlock(int mip)
{
	if (m_texture) {
		if (m_locked_bits) {
			m_texture->UnlockRect(mip);
		}
	}
	m_locked_bits = NULL;
	m_locked_pitch = 0;
}


//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
IDirect3DSurface9* CD3D9Texture::FetchSurface()
{
	if (m_texture == NULL) return NULL;
	if (m_surface) return m_surface;
	HRESULT hr = m_texture->GetSurfaceLevel(0, &m_surface);
	if (FAILED(hr)) {
		m_surface = NULL;
		return NULL;
	}
	return m_surface;
}


//---------------------------------------------------------------------
// 
//---------------------------------------------------------------------
void CD3D9Texture::ReleaseSurface()
{
	if (m_surface) {
		m_surface->Release();
		m_surface = NULL;
	}
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



