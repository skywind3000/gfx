//=====================================================================
//
// CD3D9Driver.h - 
//
// Last Modified: 2020/04/26 16:59:22
//
//=====================================================================
#ifndef _CD3D9_DRIVER_H_
#define _CD3D9_DRIVER_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <d3d9.h>

#include "GFX.h"
#include "GFXDriver.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// D3D9 Driver
//---------------------------------------------------------------------
class CD3D9Driver : public VideoDriver
{
public:
	virtual ~CD3D9Driver();
	CD3D9Driver();

public:

	virtual int Create(const CreationParameter *params);

	virtual int Release();

public:

	virtual bool Clear(bool target = true, bool zBuffer = true, bool stencil = true);

	virtual bool BeginScene();
	virtual bool EndScene();
	virtual bool Present();

	virtual bool CheckReset();

public:
	IDirect3DDevice9* GetDevice() { return m_device; }
	const IDirect3DDevice9* GetDevice() const { return m_device; }

protected:

	static int CreateInterfaceD3D9(IDirect3D9** d3d9);

	bool GetDeviceInfo();
	int SetDevice(IDirect3DDevice9 *device);

protected:
	D3DPOOL m_pool;

protected:
	HWND m_hwnd;
	BOOL m_assignment;
	IDirect3D9 *m_d3d9;
	IDirect3DDevice9 *m_device;
	D3DADAPTER_IDENTIFIER9 m_identifier;
	D3DPRESENT_PARAMETERS m_params;
	D3DCAPS9 m_caps;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



#endif


