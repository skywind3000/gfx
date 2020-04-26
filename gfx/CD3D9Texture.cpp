//=====================================================================
//
// CD3D9Texture.cpp - 
//
// Last Modified: 2020/04/26 16:59:09
//
//=====================================================================
#include "CD3D9Texture.h"


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
	}
	return f;
}


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
CD3D9Texture::CD3D9Texture(IDirect3DDevice9 *device, int w, int h, PixelFormat fmt, int flag, int mipmap):
	Texture(w, h, fmt)
{
	int hr = CreateTexture(device, w, h, fmt, flag, mipmap);
	if (hr != 0) {
		m_texture = NULL;
	}
}


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
CD3D9Texture::~CD3D9Texture()
{
	if (m_texture) {
		m_texture->Release();
		m_texture = NULL;
		if (m_device) {
			m_device->Release();
			m_device = NULL;
		}
	}
}


//---------------------------------------------------------------------
// initialize
//---------------------------------------------------------------------
CD3D9Texture::CD3D9Texture(IDirect3DDevice9 *device, const Image *image, int flag, int mipmap):
	Texture(image->GetWidth(), image->GetHeight(), image->GetFormat())
{
	int hr = CreateTexture(device, image->GetWidth(), image->GetHeight(), 
			image->GetFormat(), flag, mipmap);
	if (hr == 0) {
		this->RestoreFromImage(image);
	}
}


//---------------------------------------------------------------------
// create texture
//---------------------------------------------------------------------
int CD3D9Texture::CreateTexture(IDirect3DDevice9 *device, int w, int h, 
		PixelFormat fmt, int flag, int mipmap)
{
	D3DFORMAT d3d_fmt = GetD3DFormat(fmt);
	DWORD usage = 0;
	m_texture = NULL;
	HRESULT hr = device->CreateTexture(w, h, mipmap, usage, d3d_fmt, D3DPOOL_MANAGED, &m_texture, NULL);
	if (SUCCEEDED(hr)) {
		device->AddRef();
		m_device = device;
	}
	else {
		m_texture = NULL;
		return -1;
	}
	m_pitch = 0;
	m_bits = NULL;
	return 0;
}


//---------------------------------------------------------------------
// lock
//---------------------------------------------------------------------
void* CD3D9Texture::Lock(bool readOnly)
{
	if (m_texture == NULL) {
		return NULL;
	}

	if (m_bits) {
		return m_bits;
	}

	D3DLOCKED_RECT rect;
	HRESULT hr = m_texture->LockRect(0, &rect, 0, 0);

	if (SUCCEEDED(hr)) {
		m_pitch = (int32_t)rect.Pitch;
		m_bits = rect.pBits;
	}

	return m_bits;
}


//---------------------------------------------------------------------
// unlock
//---------------------------------------------------------------------
void CD3D9Texture::Unlock()
{
	if (m_texture) {
		if (m_bits) {
			m_texture->UnlockRect(0);
		}
	}
	m_bits = NULL;
	m_pitch = 0;
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



