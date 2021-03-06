//=====================================================================
//
// GFXShader.h - 
//
// Last Modified: 2020/04/28 12:09:50
//
//=====================================================================
#ifndef _GFX_SHADER_H_
#define _GFX_SHADER_H_

#include "GFX.h"
#include "GFXTypes.h"
#include "GFXUtil.h"
#include "GFXDriver.h"


//---------------------------------------------------------------------
// Namespace Begin
//---------------------------------------------------------------------
NAMESPACE_BEGIN(GFX);


//---------------------------------------------------------------------
// ShaderMode
//---------------------------------------------------------------------
enum ShaderMode {
	GSM_VERTEX_SHADER = 0,
	GSM_PIXEL_SHADER,
	GSM_GEOMETRY_SHADER,
	GSM_COMPUTER_SHADER,
	GSM_TESSELLATION_SHADER,
	GSM_UNKNOWN,
};


//---------------------------------------------------------------------
// CShaderProgram
//---------------------------------------------------------------------
class CShaderProgram
{
public:
	virtual ~CShaderProgram();
	CShaderProgram();

public:
	virtual bool BeginShader();
	virtual bool EndShader();

public:
	inline ShaderMode GetMode() const { return m_mode; }
	inline std::string GetErrorMessage() const { return m_error_message; }

protected:
	std::string m_error_message;
	ShaderMode m_mode;
};


//---------------------------------------------------------------------
// Namespace End
//---------------------------------------------------------------------
NAMESPACE_END(GFX);


#endif



