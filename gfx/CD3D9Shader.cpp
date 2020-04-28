//=====================================================================
//
// CD3D9Shader.cpp - 
//
// Last Modified: 2020/04/28 12:07:19
//
//=====================================================================

#include "CD3D9Shader.h"



//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// ctor
//---------------------------------------------------------------------
CD3D9Shader::CD3D9Shader()
{
	m_pshader = NULL;
	m_vshader = NULL;
	m_ctable = NULL;
	m_shader = NULL;
	m_errmsg = NULL;
	m_driver = NULL;
	m_device = NULL;
};


//---------------------------------------------------------------------
// dtor
//---------------------------------------------------------------------
CD3D9Shader::~CD3D9Shader()
{
	Release();
}


//---------------------------------------------------------------------
// create program
//---------------------------------------------------------------------
int CD3D9Shader::Create(CD3D9Driver *driver, ShaderMode mode, const char *fname, const char *profile, const char *entry)
{
	Release();
	
	HRESULT hr = D3DXCompileShaderFromFile(
			fname,
			NULL,
			NULL,
			entry,
			profile,
			D3DXSHADER_DEBUG,
			&m_shader,
			&m_errmsg,
			&m_ctable);

	if (FAILED(hr)) {
		m_error_message = (char*)(m_errmsg->GetBufferPointer());
		Release();
		return -1;
	}

	m_driver = driver;
	IDirect3DDevice9 *device = driver->GetDevice();

	if (mode == GSM_VERTEX_SHADER) {
		hr = device->CreateVertexShader((DWORD*)m_shader->GetBufferPointer(), &m_vshader);
	}
	else if (mode == GSM_PIXEL_SHADER) {
		hr = device->CreatePixelShader((DWORD*)m_shader->GetBufferPointer(), &m_pshader);
	}

	if (FAILED(hr)) {
		m_error_message = "create shader failed: " + hr;
		return -2;
	}

	m_device = device;
	m_device->AddRef();

	m_mode = mode;

	return 0;
}


//---------------------------------------------------------------------
// release program
//---------------------------------------------------------------------
void CD3D9Shader::Release()
{
	if (m_ctable) m_ctable->Release();
	if (m_pshader) m_pshader->Release();
	if (m_vshader) m_vshader->Release();
	m_pshader = NULL;
	m_vshader = NULL;
	m_ctable = NULL;
	if (m_shader) m_shader->Release();
	if (m_errmsg) m_errmsg->Release();
	m_shader = NULL;
	m_errmsg = NULL;
	if (m_device) {
		m_device->Release();
		m_device = NULL;
	}
}


//---------------------------------------------------------------------
// begin shader
//---------------------------------------------------------------------
bool CD3D9Shader::BeginShader()
{
	if (m_mode == GSM_VERTEX_SHADER) {
		if (m_vshader) {
			HRESULT hr = m_device->SetVertexShader(m_vshader);
			if (SUCCEEDED(hr)) {
				return true;
			}
		}
	}
	else if (m_mode == GSM_PIXEL_SHADER) {
		if (m_pshader) {
			HRESULT hr = m_device->SetPixelShader(m_pshader);
			if (SUCCEEDED(hr)) {
				return true;
			}
		}
	}
	return false;
}


//---------------------------------------------------------------------
// end shader
//---------------------------------------------------------------------
bool CD3D9Shader::EndShader()
{
	if (m_mode == GSM_VERTEX_SHADER) {
		if (m_device) {
			HRESULT hr = m_device->SetVertexShader(NULL);
			if (SUCCEEDED(hr)) {
				return true;
			}
		}
	}
	else if (m_mode == GSM_PIXEL_SHADER) {
		if (m_device) {
			HRESULT hr = m_device->SetPixelShader(NULL);
			if (SUCCEEDED(hr)) {
				return true;
			}
		}
	}
	return false;
}



//---------------------------------------------------------------------
// Get uniform id by name
//---------------------------------------------------------------------
D3DXHANDLE CD3D9Shader::GetConstantByName(D3DXHANDLE hConstant, const char *name)
{
	if (m_ctable != NULL) {
		return m_ctable->GetConstantByName(hConstant, name);
	}
	return NULL;
}


//---------------------------------------------------------------------
// get sampler index
//---------------------------------------------------------------------
int CD3D9Shader::GetSamplerIndex(D3DXHANDLE hConstant)
{
	if (m_ctable != NULL) {
		return (int)m_ctable->GetSamplerIndex(hConstant);
	}
	return -1;
}


//---------------------------------------------------------------------
// get sampler index
//---------------------------------------------------------------------
int CD3D9Shader::GetSamplerByName(const char *name)
{
	D3DXHANDLE ii = GetConstantByName(NULL, name);
	if (ii != NULL) {
		return GetSamplerIndex(ii);
	}
	return -1;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::SetInt(D3DXHANDLE hConstant, INT n)
{
	if (m_device && m_ctable) {
		return m_ctable->SetInt(m_device, hConstant, n);
	}
	return D3D_OK;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::SetFloat(D3DXHANDLE hConstant, FLOAT f)
{
	if (m_device && m_ctable) {
		return m_ctable->SetFloat(m_device, hConstant, f);
	}
	return D3D_OK;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::SetIntArray(D3DXHANDLE hConstant, CONST INT *pn, UINT count)
{
	if (m_device && m_ctable) {
		return m_ctable->SetIntArray(m_device, hConstant, pn, count);
	}
	return D3D_OK;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::SetFloatArray(D3DXHANDLE hConstant, CONST FLOAT *pf, UINT count)
{
	if (m_device && m_ctable) {
		return m_ctable->SetFloatArray(m_device, hConstant, pf, count);
	}
	return D3D_OK;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::SetMatrix(D3DXHANDLE hConstant, CONST D3DXMATRIX *pMatrix)
{
	if (m_device && m_ctable) {
		return m_ctable->SetMatrix(m_device, hConstant, pMatrix);
	}
	return D3D_OK;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::SetMatrix(D3DXHANDLE hConstant, const Core::Matrix4 &matrix)
{
	if (m_device && m_ctable) {
		D3DXMATRIX mm(&matrix.m[0][0]);
		return m_ctable->SetMatrix(m_device, hConstant, &mm);
	}
	return D3D_OK;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::NameSetInt(const char *name, INT n)
{
	D3DXHANDLE h = GetConstantByName(NULL, name);
	if (h != NULL) {
		return SetInt(h, n);
	}
	return D3DERR_NOTFOUND;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::NameSetFloat(const char *name, FLOAT f)
{
	D3DXHANDLE h = GetConstantByName(NULL, name);
	if (h != NULL) {
		return SetFloat(h, f);
	}
	return D3DERR_NOTFOUND;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::NameSetIntArray(const char *name, CONST INT *pn, UINT count)
{
	D3DXHANDLE h = GetConstantByName(NULL, name);
	if (h != NULL) {
		return SetIntArray(h, pn, count);
	}
	return D3DERR_NOTFOUND;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::NameSetFloatArray(const char *name, CONST FLOAT *pf, UINT count)
{
	D3DXHANDLE h = GetConstantByName(NULL, name);
	if (h != NULL) {
		return SetFloatArray(h, pf, count);
	}
	return D3DERR_NOTFOUND;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::NameSetMatrix(const char *name, CONST D3DXMATRIX *pMatrix)
{
	D3DXHANDLE h = GetConstantByName(NULL, name);
	if (h != NULL) {
		return SetMatrix(h, pMatrix);
	}
	return D3DERR_NOTFOUND;
}


//---------------------------------------------------------------------
// uniform
//---------------------------------------------------------------------
HRESULT CD3D9Shader::NameSetMatrix(const char *name, const Core::Matrix4 &matrix)
{
	D3DXHANDLE h = GetConstantByName(NULL, name);
	if (h != NULL) {
		return SetMatrix(h, matrix);
	}
	return D3DERR_NOTFOUND;
}



//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



