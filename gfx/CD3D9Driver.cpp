//=====================================================================
//
// CD3D9Driver.cpp - 
//
// Last Modified: 2020/04/27 00:11:50
//
//=====================================================================
#include "GFXTransform.h"
#include "CD3D9Driver.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
CD3D9Driver::CD3D9Driver(): VideoDriver()
{
	m_d3d9 = NULL;
	m_device = NULL;
	m_assignment = FALSE;
	m_pool = D3DPOOL_MANAGED;
	m_background_color = D3DCOLOR_XRGB(25, 25, 112);
	m_video_width = 0;
	m_video_height = 0;
	m_hwnd = NULL;
}


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
CD3D9Driver::~CD3D9Driver()
{
	Release();
}


//---------------------------------------------------------------------
// create d3d9
//---------------------------------------------------------------------
int CD3D9Driver::CreateInterfaceD3D9(IDirect3D9** d3d9)
{
	typedef IDirect3D9* (WINAPI *Direct3DCreate9Fn)(UINT SDKVersion);
	static Direct3DCreate9Fn Dll_Direct3DCreate9 = NULL;
	static HINSTANCE hDLL = NULL;

	if (hDLL == NULL) {
		hDLL = LoadLibraryA("d3d9.dll");
		if (hDLL == NULL) {
			return -1;
		}
	}

	if (Dll_Direct3DCreate9 == NULL) {
		Dll_Direct3DCreate9 = (Direct3DCreate9Fn)GetProcAddress(hDLL, "Direct3DCreate9");
		if (Dll_Direct3DCreate9 == NULL) 
			return -2;
	}

	assert(d3d9);

	IDirect3D9 *s_d3d9 = NULL;

	if (s_d3d9 == NULL) {
		s_d3d9 = Dll_Direct3DCreate9(D3D_SDK_VERSION);
		if (s_d3d9 == NULL) {
			return -3;
		}
	}

	if (d3d9) *d3d9 = s_d3d9;

	return 0;
}


//---------------------------------------------------------------------
// release
//---------------------------------------------------------------------
int CD3D9Driver::Release()
{
	if (m_device) {
		m_device->Release();
		m_device = NULL;
	}
	if (m_d3d9) {
		m_d3d9->Release();
		m_d3d9 = NULL;
	}
	m_assignment = FALSE;
	m_pool = D3DPOOL_DEFAULT;
	return 0;
}


//---------------------------------------------------------------------
// create
//---------------------------------------------------------------------
int CD3D9Driver::Create(const CreationParameter *params)
{
	Release();
	
	int w = params->width;
	int h = params->height;
	int nBackBuffer = params->n_back_buffer;
	int adapter = params->adapter;
	HWND hwnd = (HWND)params->window_handle;

	m_hwnd = hwnd;

	if (params->existing_device != NULL) {
		IDirect3DDevice9 *dev = (IDirect3DDevice9*)params->existing_device;
		return SetDevice(dev);
	}

	// prepare parameters
	memset(&m_params, 0, sizeof(m_params));

	m_params.BackBufferWidth = (w >= 0)? w : 0;
	m_params.BackBufferHeight = (h >= 0)? h : 0;
	m_params.BackBufferFormat = D3DFMT_X8R8G8B8;
	m_params.BackBufferCount = nBackBuffer;
	m_params.MultiSampleType = D3DMULTISAMPLE_NONE;
	m_params.MultiSampleQuality = 0;
	m_params.EnableAutoDepthStencil = TRUE;
	m_params.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
	m_params.AutoDepthStencilFormat     = D3DFMT_D24S8;

	m_params.FullScreen_RefreshRateInHz = 0;
	m_params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	m_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_params.hDeviceWindow = hwnd;
	m_params.Windowed = TRUE;

	if (hwnd && (w < 0 || h < 0)) {
		RECT rc;
		::GetClientRect(hwnd, &rc);
		m_params.BackBufferWidth = rc.right - rc.left;
		m_params.BackBufferHeight = rc.bottom - rc.top;
		// printf("adjust: %d %d\n", m_params.BackBufferWidth, m_params.BackBufferHeight);
	}

	// query interface
	CreateInterfaceD3D9(&m_d3d9);

	if (m_d3d9 == NULL) {
		return -1;
	}

	HRESULT hr = D3D_OK;

	for (int i = 0; i < (2 * 2); i++) {
		D3DDEVTYPE deviceType = ((i / 2) == 0)? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
		D3DFORMAT format = ((i % 2) == 0)? D3DFMT_D24S8 : D3DFMT_D32;
		D3DCAPS9 caps;

		m_params.AutoDepthStencilFormat = format;

		hr = m_d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

		if (FAILED(hr)) {
			fprintf(stderr, "Failed to gather Direct3D 9 device caps.");
			return -2;
		}

		DWORD flags = D3DCREATE_FPU_PRESERVE;

		if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
			flags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
			assert(caps.VertexProcessingCaps != 0);
		} else {
			flags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;
			assert(caps.VertexProcessingCaps);
		}

		if (caps.DevCaps & D3DDEVCAPS_PUREDEVICE) {
			flags |= D3DCREATE_PUREDEVICE;
		}

		if (m_d3d9 != NULL) {
			hr = m_d3d9->CreateDevice(adapter,
					deviceType,
					hwnd,
					flags,
					&m_params,
					&m_device);
			if (SUCCEEDED(hr)) {
				break;
			}
		}
	}

	if (m_device == NULL) {
		fprintf(stderr, "failed to create device: 0x%x\n", (unsigned int)hr);
		return -3;
	}

	hr = m_device->GetDeviceCaps(&m_caps);

	if (FAILED(hr)) {
		return -4;
	}

	if (!GetDeviceInfo()) {
		return -5;
	}

	return 0;
}


//---------------------------------------------------------------------
// assign device
//---------------------------------------------------------------------
int CD3D9Driver::SetDevice(IDirect3DDevice9 *device)
{
	Release();

	m_device = device;

	if (m_device == NULL) {
		return -1;
	}

	m_device->AddRef();

	HRESULT hr = D3D_OK;

#ifdef GFX_USE_D3DEX
	hr = m_device->QueryInterface(IID_IDirect3DDevice9Ex, (void**)&m_device_ex);
	if (FAILED(hr)) {
		m_device_ex = NULL;
	}
#endif

	IDirect3DSwapChain9* pSwapChain;

	hr = m_device->GetSwapChain(0, &pSwapChain);

	if (FAILED(hr)) {
		return -2;
	}

	hr = pSwapChain->GetPresentParameters(&m_params);

	if (FAILED(hr)) {
		return -3;
	}

	pSwapChain->Release();

	if (FAILED(m_device->GetDeviceCaps(&m_caps))) {
		m_device = NULL;
		return -4;
	}

	if (GetDeviceInfo() == false) {
		return -5;
	}

	m_assignment = TRUE;

	return 0;
}



//---------------------------------------------------------------------
// GetDeviceInfo
//---------------------------------------------------------------------
bool CD3D9Driver::GetDeviceInfo()
{
	InitSize(m_params.BackBufferWidth, m_params.BackBufferHeight);

	m_video_capacity.texture_size_pow2 = false;
	m_video_capacity.texture_square_only = false;
	m_video_capacity.texture_has_alpha = true;
	m_video_capacity.texture_has_mipmap = true;

	if ((D3DPTEXTURECAPS_POW2 & m_caps.TextureCaps) != 0) {
		m_video_capacity.texture_size_pow2 = true;
		fprintf(stderr, "texture size must be power of 2\n");
	}

	if ((D3DPTEXTURECAPS_SQUAREONLY & m_caps.TextureCaps) != 0) {
		m_video_capacity.texture_square_only = true;
		fprintf(stderr, "texture is square only\n");
	}

	if ((D3DPTEXTURECAPS_ALPHA & m_caps.TextureCaps) == 0) {
		m_video_capacity.texture_has_alpha = false;
		fprintf(stderr, "texture doesn't support alpha channel\n");
	}

	if ((D3DPTEXTURECAPS_MIPMAP & m_caps.TextureCaps) == 0) {
		m_video_capacity.texture_has_mipmap = false;
		fprintf(stderr, "texture doesn't support mipmap\n");
	}

	m_video_capacity.texture_max_width = (int)m_caps.MaxTextureWidth;
	m_video_capacity.texture_max_height = (int)m_caps.MaxTextureHeight;

	return true;
}


//---------------------------------------------------------------------
// clear buffers
//---------------------------------------------------------------------
bool CD3D9Driver::Clear(bool target, bool zBuffer, bool stencil)
{
	if (m_device) {
		DWORD flags = 0;
		if (target) flags |= D3DCLEAR_TARGET;
		if (zBuffer) flags |= D3DCLEAR_ZBUFFER;
		if (stencil) flags |= D3DCLEAR_STENCIL;
		m_device->Clear(0, NULL, flags, m_background_color.color, 1.0f, 0);
		return true;
	}
	return false;
}


//---------------------------------------------------------------------
// begin scene
//---------------------------------------------------------------------
bool CD3D9Driver::BeginScene()
{
	if (m_device) {
		HRESULT hr = m_device->BeginScene();
		if (SUCCEEDED(hr)) {
			return true;
		}
	}
	return false;
}


//---------------------------------------------------------------------
// end scene
//---------------------------------------------------------------------
bool CD3D9Driver::EndScene()
{
	if (m_device) {
		HRESULT hr = m_device->EndScene();
		if (SUCCEEDED(hr)) {
			return true;
		}
	}
	return false;
}


//---------------------------------------------------------------------
// present
//---------------------------------------------------------------------
bool CD3D9Driver::Present()
{
	if (m_device) {
		HRESULT hr = m_device->Present(NULL, NULL, NULL, NULL);
		if (SUCCEEDED(hr)) {
			return true;
		}
	}
	return false;
}


//---------------------------------------------------------------------
// check reset
//---------------------------------------------------------------------
bool CD3D9Driver::CheckReset()
{
	if (m_device == NULL) return false;
	HRESULT hr = m_device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST) {
		for (int i = 0; i < 10; i++) {
			Sleep(20);
			hr = m_device->TestCooperativeLevel();
			if (hr == D3DERR_DEVICENOTRESET) 
				break;
		}
		if (hr == D3DERR_DEVICELOST) {
			return false;
		}
	}
	if (hr == D3DERR_DRIVERINTERNALERROR) {
		fprintf(stderr, "d3d9 device internal error\n");
		exit(1);
	}
	if (OnPreReset) OnPreReset();
	hr = m_device->Reset(&m_params);
	if (FAILED(hr)) {
		return false;
	}
	if (OnPostReset) OnPostReset();
	return true;
}


//---------------------------------------------------------------------
// Easy Set Transform
//---------------------------------------------------------------------
HRESULT CD3D9Driver::SetTransform(D3DTRANSFORMSTATETYPE state, const D3DMATRIX *pMatrix)
{
	if (pMatrix == NULL) {
		pMatrix = (const D3DMATRIX*)(&Core::Matrix4Unit);
	}
	return m_device->SetTransform(state, pMatrix);
}


//---------------------------------------------------------------------
// Easy Set Transform
//---------------------------------------------------------------------
HRESULT CD3D9Driver::SetTransform(D3DTRANSFORMSTATETYPE state, const Core::Matrix4& matrix)
{
	return SetTransform(state, (const D3DMATRIX*)&matrix);
}


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


