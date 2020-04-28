//=====================================================================
//
// CD3D9Shader.h - 
//
// Last Modified: 2020/04/28 12:06:20
//
//=====================================================================
#ifndef _CD3D9_SHADER_H_
#define _CD3D9_SHADER_H_

#include "CD3D9Driver.h"
#include "GFXShader.h"

#include <d3dx9.h>


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// CD3D9Shader
//---------------------------------------------------------------------
class CD3D9Shader : public CShaderProgram
{
public:
	virtual ~CD3D9Shader();
	CD3D9Shader();

public:
	int Create(CD3D9Driver *driver, ShaderMode mode, const char *fname, const char *profile, const char *entry);
	void Release();

	bool BeginShader();
	bool EndShader();

public:

	D3DXHANDLE GetConstantByName(D3DXHANDLE hConstant, const char *name);

	int GetSamplerIndex(D3DXHANDLE hConstant);
	int GetSamplerByName(const char *name);

	HRESULT SetInt(D3DXHANDLE hConstant, INT n);
	HRESULT SetFloat(D3DXHANDLE hConstant, FLOAT f);
	HRESULT SetIntArray(D3DXHANDLE hConstant, CONST INT *pn, UINT count);
	HRESULT SetFloatArray(D3DXHANDLE hConstant, CONST FLOAT *pf, UINT count);
	HRESULT SetMatrix(D3DXHANDLE hConstant, CONST D3DXMATRIX *pMatrix);
	HRESULT SetMatrix(D3DXHANDLE hConstant, const Core::Matrix4 &matrix);

	HRESULT NameSetInt(const char *name, INT n);
	HRESULT NameSetFloat(const char *name, FLOAT f);
	HRESULT NameSetIntArray(const char *name, CONST INT *pn, UINT count);
	HRESULT NameSetFloatArray(const char *name, CONST FLOAT *pf, UINT count);
	HRESULT NameSetMatrix(const char *name, CONST D3DXMATRIX *pMatrix);
	HRESULT NameSetMatrix(const char *name, const Core::Matrix4 &matrix);

protected:
	CD3D9Driver *m_driver;
	IDirect3DDevice9 *m_device;
	ID3DXBuffer *m_shader;
	ID3DXBuffer *m_errmsg;
	ID3DXConstantTable *m_ctable;
	IDirect3DPixelShader9 *m_pshader;
	IDirect3DVertexShader9 *m_vshader;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);



#endif


