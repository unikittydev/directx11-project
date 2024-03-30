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

Texture2D::Texture2D(uint width, uint height, UINT flags) : Texture2D()
{
    // Define texture parameters
    D3D11_TEXTURE2D_DESC textureDesc;
    ZeroMemory(&textureDesc, sizeof(textureDesc));
    textureDesc.Width = width;                    // Width of the texture
    textureDesc.Height = height;                   // Height of the texture
    textureDesc.MipLevels = 1;                   // Number of mip levels
    textureDesc.ArraySize = 1;                   // Number of textures in the array
    textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // Texture format
    textureDesc.SampleDesc.Count = 1;            // Number of samples per pixel
    textureDesc.SampleDesc.Quality = 0;          // Quality level of multisampling
    textureDesc.Usage = D3D11_USAGE_DEFAULT;     // Resource usage (default)
    textureDesc.BindFlags = flags; // How the texture is bound to the pipeline stages
    textureDesc.CPUAccessFlags = 0;              // CPU access flags
    textureDesc.MiscFlags = 0;                   // Miscellaneous flags

    // Initialize texture data (optional)
    D3D11_SUBRESOURCE_DATA initData;
    ZeroMemory(&initData, sizeof(initData));
    initData.pSysMem = nullptr;                  // Pointer to the texture data
    initData.SysMemPitch = 0;                    // Pitch of the texture data (for 2D textures)
    initData.SysMemSlicePitch = 0;               // Slice pitch of the texture data (for 3D textures)

    // Create the texture
    ID3D11Texture2D* texture = nullptr;
    HRESULT hr = Application::GetDevicePtr()->CreateTexture2D(&textureDesc, nullptr, &texture);
    
    ID3D11ShaderResourceView* textureSRV = nullptr;
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = textureDesc.Format;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;

    hr = Application::GetDevicePtr()->CreateShaderResourceView(texture, &srvDesc, &textureSRV);

    this->texture = texture;
    this->textureView = textureSRV;
}

void Texture2D::Release()
{
    texture->Release();
    textureView->Release();
    sampler->Release();
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

ID3D11Texture2D* Texture2D::GetTexture() const
{
    return texture;
}