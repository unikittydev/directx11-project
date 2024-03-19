#pragma once

#include <string>
#include <d3d11.h>

#include "InputLayoutOption.h"

class Shader
{
private:
	ID3DBlob* vertexShaderBC;
	ID3DBlob* pixelShaderBC;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
	ID3D11RasterizerState* rasterizerState;
public:
	Shader(const std::wstring& path, InputLayoutOption layoutOptions);

	void SetInputLayout(ID3D11Device* device, InputLayoutOption layoutOptions);
	
	void PrepareDraw();

	void SetActive();

	void Release();
};
