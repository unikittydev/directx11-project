#pragma once
#include <d3d11.h>

#include "ConstantBuffer.h"
#include "Core/Types.h"

class IComponent;
class Transform;
class Shader;
class Mesh;

class ShadowMap
{
private:
    struct ShadowMapData
    {
        matrix _ViewProj;
    } shadowMapData;

    ConstantBuffer<ShadowMapData> shadowMapBuffer;
    
    uint resolution; 
    
    ID3D11Texture2D* depthStencilTexture;
    ID3D11DepthStencilView* depthStencilView;
    ID3D11ShaderResourceView* srv;
    ID3D11DepthStencilState* dsState;
	ID3D11SamplerState* sampler;

	Transform* tr;
    
    const matrix projMatrix = DirectX::XMMatrixOrthographicLH(70.0f, 70.0f, 0.0001f, 1000.0f);

    Shader* depthShader;

public:
    ShadowMap(uint resolution, Shader* depthShader);
    ~ShadowMap();

    void Prepare();
    void PrepareMesh(Mesh* mesh);
    void Bind(uint slot);
    
    void SetLightTransform(Transform* tr);
    
    matrix GetViewProjMatrix() const;
    ID3D11ShaderResourceView* const* GetSRV() const;
    ID3D11SamplerState* const* GetSampler() const;
};
