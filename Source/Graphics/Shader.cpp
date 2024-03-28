#include <d3dcompiler.h>
#include <iostream>
#include <DirectXMath.h>

#include "Shader.h"

#include "Application/Application.h"

Shader::Shader(const std::wstring& path, InputLayoutOption layoutOptions, RastState rsState)
{
	ID3DBlob* errorCode = nullptr;

	// Compile vertex shader
	HRESULT res = D3DCompileFromFile(path.c_str(),
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexShaderBC,
		&errorCode);

	if (FAILED(res))
	{
		if (ERROR(errorCode))
		{
			char* message = (char*)errorCode->GetBufferPointer();
			std::cerr << message << std::endl;
			throw;
		}
		else
			std::cout << "Didn't found vertex shader!" << std::endl;
	}

	// Compile pixel shader
	res = res = D3DCompileFromFile(path.c_str(), nullptr /*macros*/, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShaderBC, &errorCode);

	if (FAILED(res))
	{
		if (ERROR(errorCode))
		{
			char* message = (char*)errorCode->GetBufferPointer();
			std::cerr << message << std::endl;
			throw;
		}
		else
			std::cout << "Didn't found pixel shader!" << std::endl;
	}

	// Create shaders
	auto* device = Application::GetDevicePtr();
	device->CreateVertexShader(vertexShaderBC->GetBufferPointer(), vertexShaderBC->GetBufferSize(), nullptr, &vertexShader);
	device->CreatePixelShader(pixelShaderBC->GetBufferPointer(), pixelShaderBC->GetBufferSize(), nullptr, &pixelShader);

	// Create input layout
	SetInputLayout(device, layoutOptions);

	// Create rasterizer state
	CD3D11_RASTERIZER_DESC rasterizerDesc;

	if (rsState == Solid)
	{
		rasterizerDesc = {};
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	}
	else if (rsState == Wireframe)
	{
		rasterizerDesc = {};
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	}
	else if (rsState == DepthOnly)
	{
		const INT depthBias = -3000;
		const FLOAT depthBiasClamp = 0;
		const FLOAT scaledDepthBias = 0.5;
		
		rasterizerDesc = CD3D11_RASTERIZER_DESC{
			D3D11_FILL_SOLID,
			D3D11_CULL_FRONT,
			TRUE,
			depthBias,
			depthBiasClamp,
			scaledDepthBias,
			TRUE,
			FALSE,
			FALSE,
			FALSE
		};
	}

	device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
}

void Shader::SetInputLayout(ID3D11Device* device, InputLayoutOption layoutOptions)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputs{};

	unsigned int offset = 0;
	
	if (HasFlag(layoutOptions, InputLayoutOption::Position))
	{
		inputs.push_back(
			D3D11_INPUT_ELEMENT_DESC
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0
			});
		offset = D3D11_APPEND_ALIGNED_ELEMENT;
	}
	if (HasFlag(layoutOptions, InputLayoutOption::Normal))
	{
		inputs.push_back(
			D3D11_INPUT_ELEMENT_DESC
			{
				"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0
			});
		offset = D3D11_APPEND_ALIGNED_ELEMENT;
	}
	if (HasFlag(layoutOptions, InputLayoutOption::Tangent))
	{
		inputs.push_back(
			D3D11_INPUT_ELEMENT_DESC
			{
				"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0
			});
		offset = D3D11_APPEND_ALIGNED_ELEMENT;
	}
	if (HasFlag(layoutOptions, InputLayoutOption::VertexColor))
	{
		inputs.push_back(
			D3D11_INPUT_ELEMENT_DESC
			{
				"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0
			});
		offset = D3D11_APPEND_ALIGNED_ELEMENT;
	}
	if (HasFlag(layoutOptions, InputLayoutOption::UV0))
	{
		inputs.push_back(
			D3D11_INPUT_ELEMENT_DESC
			{
				"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,
				0, offset, D3D11_INPUT_PER_VERTEX_DATA, 0
			});
		offset = D3D11_APPEND_ALIGNED_ELEMENT;
	}

	const auto result = device->CreateInputLayout(inputs.data(), inputs.size(), vertexShaderBC->GetBufferPointer(), vertexShaderBC->GetBufferSize(), &inputLayout);
	if (result != S_OK)
		std::cerr << "Failed to create input layout. Err code: " << result << std::endl;
}

void Shader::PrepareDraw()
{
	auto* ctx = Application::GetDeviceContext();

	ctx->RSSetState(rasterizerState);
	ctx->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	ctx->IASetInputLayout(inputLayout);
}

void Shader::SetActive()
{
	auto* ctx = Application::GetDeviceContext();

	ctx->VSSetShader(vertexShader, nullptr, 0);
	ctx->PSSetShader(pixelShader, nullptr, 0);
}

void Shader::Release()
{
	vertexShaderBC->Release();
	pixelShaderBC->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	rasterizerState->Release();
}