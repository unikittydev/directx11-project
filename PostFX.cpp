#include "PostFX.h"

#include <d3d11.h>
#include <Application/Application.h>
#include <Graphics/Shader.h>
#include <Graphics/Shaders.h>

void PostFX::TryResize()
{
    if (Application::GetWindow().Width() == width && Application::GetWindow().Height() == height)
        return;

    width = Application::GetWindow().Width();
    height = Application::GetWindow().Height();

    rtvTex.Release();
    rtvTex = Texture2D(width, height, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
}

PostFX::PostFX()
{
    width = Application::GetWindow().Width();
    height = Application::GetWindow().Height();
    rtvTex = Texture2D(width, height, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
    shader = Shaders::Get(L"./Shaders/PostFX.hlsl", (InputLayoutOption)0, RastState::Solid);

    const float4 quad[4] =
    {
        float4{ -1, -1, 0, 0 },
        float4{  1, -1, 0, 0 },
        float4{ -1,  1, 0, 0 },
        float4{  1,  1, 0, 0 },
    };

    auto* ctx = Application::GetDeviceContext();

    // Create vertex buffer
    auto* device = Application::GetDevicePtr();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    bufferDesc.ByteWidth = sizeof(float4) * 4;

    D3D11_SUBRESOURCE_DATA bufferData = {};
    bufferData.pSysMem = quad;
    bufferData.SysMemPitch = 0;
    bufferData.SysMemSlicePitch = 0;

    device->CreateBuffer(&bufferDesc, &bufferData, &postFXBuffer);
}

void PostFX::Draw()
{
    TryResize();

    auto* ctx = Application::GetDeviceContext();

    shader->PrepareDraw();

    const UINT strides[] = { sizeof(float4) };
    const UINT offsets[] = { 0 };
    ctx->IASetVertexBuffers(0, 1, &postFXBuffer, strides, offsets);
    ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

    auto* rtv = Application::GetSwapchain().GetRenderTargetView();
    ID3D11Resource* rtvRes;
    rtv->GetResource(&rtvRes);

    ctx->CopyResource(rtvTex.GetTexture(), rtvRes);

    ctx->PSSetShaderResources(0, 1, rtvTex.GetSRV());
    ctx->PSSetSamplers(0, 1, rtvTex.GetSampler());

    shader->SetActive();

    ctx->Draw(4, 0);
}