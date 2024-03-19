#pragma once

#include <string>
#include <d3d11.h>

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
	Shader(std::wstring path);

	void PrepareDraw();

	void SetActive();

	void Release();
};