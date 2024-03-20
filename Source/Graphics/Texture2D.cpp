#include "Texture2D.h"
#include "TextureImporter.h"
#include "Application/Application.h"

Texture2D::Texture2D() : texture(nullptr), textureView(nullptr)
{
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    Application::GetDevicePtr()->CreateSamplerState(&sampDesc, &sampler);
}

Texture2D Texture2D::LoadFromFile(const std::wstring& path)
{
    Texture2D tex;
    TextureImporter::LoadTexture(path, reinterpret_cast<ID3D11Resource**>(&tex.texture), &tex.textureView);
		
    return tex;
}

uint Texture2D::GetWidth() const
{
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);
    return desc.Width;
}

uint Texture2D::GetHeight() const
{
    D3D11_TEXTURE2D_DESC desc;
    texture->GetDesc(&desc);
    return desc.Height;
}

ID3D11ShaderResourceView* const* Texture2D::GetSRV() const
{
    return &textureView;
}

ID3D11SamplerState* const* Texture2D::GetSampler() const
{
    return &sampler;
}