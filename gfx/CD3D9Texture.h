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
	CD3D9Texture(IDirect3DDevice9 *device, int w, int h, PixelFormat fmt, int flag, int mipmap = 1);
	CD3D9Texture(IDirect3DDevice9 *device, const Image *image, int flag, int mipmap = 1);

public:

	virtual void *Lock(bool readOnly = false);
	virtual void Unlock();

	inline IDirect3DTexture9 *GetD3DTexture() { return m_texture; }

	static D3DFORMAT GetD3DFormat(PixelFormat fmt);

protected:
	int CreateTexture(IDirect3DDevice9 *device, int w, int h, PixelFormat fmt, int flag, int mipmap);

protected:
	IDirect3DTexture9 *m_texture;
	IDirect3DDevice9 *m_device;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif


