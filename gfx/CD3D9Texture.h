//=====================================================================
//
// CD3D9Texture.h - 
//
// Created by skywind on 2020/04/26
// Last Modified: 2020/04/26 16:58:39
//
//=====================================================================
#ifndef _CD3D9_TEXTURE_H_
#define _CD3D9_TEXTURE_H_

#include "CD3D9Driver.h"
#include "GFXTexture.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// D3D9 Texture
//---------------------------------------------------------------------
class CD3D9Texture : public Texture
{
public:
	virtual ~CD3D9Texture();
	CD3D9Texture();

public:

	virtual int Create(CD3D9Driver *drv, int w, int h, PixelFormat fmt, int flag, int mipmap);
	virtual int Create(CD3D9Driver *drv, const Image *image, int flag, int mipmap = 1);
	virtual int Create(CD3D9Driver *drv, const char *filename, int flag, int mipmap = 1);
	virtual int Release();

	virtual void *Lock(int mip, const Rect *rect, bool readOnly = false);
	virtual void Unlock(int mip);

	inline bool Available() const { return m_texture? true : false; }

	inline IDirect3DTexture9 *GetD3DTexture() { return m_texture; }

	IDirect3DSurface9 *FetchSurface();
	void ReleaseSurface();

	static D3DFORMAT GetD3DFormat(PixelFormat fmt);

protected:
	int CreateTexture(IDirect3DDevice9 *device, int w, int h, PixelFormat fmt, int flag, int mipmap);

protected:
	IDirect3DSurface9 *m_surface;
	IDirect3DTexture9 *m_texture;
	IDirect3DDevice9 *m_device;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif



