#include "ShadowMap.h"

#include "Texture2D.h"
#include "Application/Application.h"
#include "Components/Transform.h"

ShadowMap::ShadowMap(uint resolution, Shader* depthShader) :
	resolution(resolution),
	depthShader(depthShader),
	shadowMapBuffer(shadowMapData, 1)
{
    D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = resolution;
	textureDesc.Height = resolution;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;
	textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.SampleDesc.Count = 1;

	auto* device = Application::GetDevicePtr();
	
	if (FAILED(device->CreateTexture2D(&textureDesc, nullptr, &depthStencilTexture)))
		std::cerr << "Failed to create shadow map texture" << std::endl;

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	
	if (FAILED(device->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &depthStencilView)))
		std::cerr << "Failed to create shadow map dsv" << std::endl;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
	shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	if (FAILED(device->CreateShaderResourceView(depthStencilTexture, &shaderResourceViewDesc, &srv)))
		std::cerr << "Failed to create shadow map resource view" << std::endl;

	D3D11_DEPTH_STENCIL_DESC depthStencilStateDesc{};
	depthStencilStateDesc.DepthEnable = true;
	depthStencilStateDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilStateDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilStateDesc.StencilEnable = false;

	device->CreateDepthStencilState(&depthStencilStateDesc, &dsState);

	D3D11_SAMPLER_DESC sampDesc{};
	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	
	device->CreateSamplerState(&sampDesc, &sampler);
}

ShadowMap::~ShadowMap()
{
    depthStencilTexture->Release();
    depthStencilView->Release();
    srv->Release();
    dsState->Release();
    sampler->Release();
}

void ShadowMap::Prepare()
{
	auto* ctx = Application::GetDeviceContext();

	const D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<FLOAT>(resolution), static_cast<FLOAT>(resolution), 0.0f, 1.0f };
	ctx->RSSetViewports(1, &viewport);

	ID3D11ShaderResourceView* nullSRV[1] = { nullptr };
	//ctx->PSSetShaderResources(1, 1, nullSRV);
	ctx->OMSetRenderTargets(0, nullptr, depthStencilView);
	ctx->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	ctx->OMSetDepthStencilState(dsState, 0);

}

void ShadowMap::Draw(IComponent* component, const matrix& ltw, const matrix& _ViewProj)
{
	auto* ctx = Application::GetDeviceContext();

	shadowMapData.ltw = ltw;
	shadowMapData._ViewProj = _ViewProj;
	shadowMapBuffer.SetDataAndBind(ctx, shadowMapData, 0);
	
    component->Draw();
}
    
void ShadowMap::SetLightTransform(Transform* tr)
{
    this->tr = tr;
}
    
matrix ShadowMap::GetViewProjMatrix() const
{
    return tr->GetWorldMatrix().Invert() * projMatrix;
}


ID3D11ShaderResourceView* const* ShadowMap::GetSRV() const
{
	return &srv;
}

ID3D11SamplerState* const* ShadowMap::GetSampler() const
{
	return &sampler;
}