#include "RenderPipeline.h"

#include "Application/Application.h"
#include "Components/Transform.h"
#include "Components/Camera/Camera.h"
#include "Components/Light/Light.h"
#include "Core/Color.h"
#include "Core/Time.h"
#include "Games/Game.h"
#include <Graphics/Texture2D.h>

RenderPipeline::RenderPipeline() :
    worldDataBuffer(worldData, 1),
    mainLightBuffer(mainLightData, 1),
    additionalLightBuffer(additionalLightsData, 1),
    ambientLightBuffer(ambientLightData, 1),
    mainShadowMap(MAIN_SHADOW_RESOLUTION, Shaders::Get(L"./Shaders/Depth.hlsl", Position, RastState::DepthOnly))
{
    
}

void RenderPipeline::PrepareShadowPass()
{
    mainShadowMap.Prepare();
    
    auto* ctx = Application::GetDeviceContext();
    
    ctx->PSSetShaderResources(1, 1, mainShadowMap.GetSRV());
    ctx->PSSetSamplers(1, 1, mainShadowMap.GetSampler());
}

void RenderPipeline::DrawShadowPass()
{
    
}

void RenderPipeline::Prepare()
{
    RECT winRect;
    GetClientRect(Application::GetWindow().GetHWND(), &winRect);

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<FLOAT>(Application::WindowWidth()), static_cast<FLOAT>(Application::WindowHeight()), 0.0f, 1.0f };

    auto* ctx = Application::GetDeviceContext();
    auto& swapchain = Application::GetSwapchain();
    
    ctx->RSSetViewports(1, &viewport);

    const float clearColor[4] = { 0.5f, 0.85f, 1.0f, 1.0f };

    ctx->OMSetRenderTargets(1, &swapchain.GetRenderTargetView(), swapchain.GetDepthStencilView());
    ctx->ClearRenderTargetView(swapchain.GetRenderTargetView(), clearColor);
    ctx->ClearDepthStencilView(swapchain.GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Updating Buffer content

    const Entity camEntity = Camera::GetActive()->entity;
    worldData._ViewProj = Camera::GetActive()->GetViewProjectionMatrix();
    worldData._ViewPos = float4{ Game::getInstance().GetComponent<Transform>(camEntity)->GetWorldTranslation() }; 
    worldData.time = float4{ Time::time(), Time::deltaTime(), DirectX::XMScalarSin(Time::time()), 1 };

    mainLightData._LightViewProj = mainShadowMap.GetViewProjMatrix();
    mainLightData.color = main != nullptr ? main->GetColor() : Color::white;
    mainLightData.direction = main != nullptr ? float4 { main->GetTr()->GetForward() } : float4{ 0, 0, 0, 0 };

    for (int i = 0; i < min(additional.size(), 8); ++i)
    {
        additionalLightsData.lights[i].color = additional[i]->GetColor();

        const float3 pos = additional[i]->GetTr()->GetWorldTranslation();
        additionalLightsData.lights[i].position = float4{ pos.x, pos.y, pos.z, additional[i]->GetRange() };
    }
    std::cout << additionalLightsData.count << std::endl;
    additionalLightsData.count = min(additional.size(), 8);

    ambientLightData.color = float4{ .03f, .03f, .03f, 1.0f };
    
    // Start binding buffers from 1 since 0 is Mesh::MeshData

    worldDataBuffer.SetDataAndBind(ctx, worldData, 1);
    mainLightBuffer.SetDataAndBind(ctx, mainLightData, 2);
    additionalLightBuffer.SetDataAndBind(ctx, additionalLightsData, 3);
    ambientLightBuffer.SetDataAndBind(ctx, ambientLightData, 4);
}

void RenderPipeline::Draw()
{
    for (auto&& component : Game::getInstance().components)
        component->Draw();

    postFX.Draw();
    
    Application::GetSwapchainPtr()->Present(1, DXGI_PRESENT_DO_NOT_WAIT);
}

void RenderPipeline::SetMainLight(Light* light)
{
    main = light;

    auto* tr = Game::getInstance().GetComponent<Transform>(light->entity);
    mainShadowMap.SetLightTransform(tr);
}

int RenderPipeline::AddAdditionalLight(Light* light)
{
    additional.push_back(light);
    
    return additional.size() - 1;
}
