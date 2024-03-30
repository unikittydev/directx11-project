#pragma once

#include <d3d11.h>
#include <string>

#include "Core/Types.h"

class Texture2D final
{
private:
    ID3D11Texture2D* texture;
    ID3D11ShaderResourceView* textureView;
    ID3D11SamplerState* sampler;

public:
    Texture2D();

    Texture2D(uint width, uint height, UINT flags);
    
    void Release();

    static Texture2D LoadFromFile(const std::wstring& path);

    uint GetWidth() const;

    uint GetHeight() const;

    ID3D11ShaderResourceView* const* GetSRV() const;

    ID3D11SamplerState* const* GetSampler() const;
    
    ID3D11Texture2D* GetTexture() const;
};
