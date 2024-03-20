#include "TextureImporter.h"

#include <iostream>
#include <wrl.h>

#include "WICTextureLoader.h"

#include "Application/Application.h"

void TextureImporter::Init()
{
    const HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(result))
        std::cerr << "Failed to init WIC!" << std::endl;
}

void TextureImporter::LoadTexture(const std::wstring& path, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView)
{
    const auto result = DirectX::CreateWICTextureFromFile(Application::GetDevicePtr(), Application::GetDeviceContext(), path.c_str(), texture, textureView, 0);
    if (result != S_OK)
        std::cerr << "Failed to load texture!" << std::endl;
}