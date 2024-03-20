#pragma once

#include <d3d11.h>
#include <string>

class TextureImporter
{
public:
    static void Init();

    static void LoadTexture(const std::wstring& path, ID3D11Resource** texture, ID3D11ShaderResourceView** textureView);
};
