﻿#pragma once
#include <vector>

#include "ConstantBuffer.h"
#include "ShadowMap.h"
#include "Core/Types.h"

class Mesh;
class Light;

class RenderPipeline
{
private:
    struct WorldData
    {
        matrix _ViewProj;
        float4 _ViewPos;
        float4 time;
    } worldData{};
    
    struct MainLight
    {
        matrix _LightViewProj;
        float4 direction;
        float4 color;
    } mainLightData{};
    
    struct AdditionalLight
    {
        float4 position;
        float4 color;
    };

    struct AdditionalLights
    {
        AdditionalLight lights[8];
        int count;
        float pad[3];
    } additionalLightsData{};
    
    struct AmbientLight
    {
        float4 color;
    } ambientLightData{};

    Light* main;
    std::vector<Light*> additional;
    
    static const uint MAIN_SHADOW_RESOLUTION = 2048;
    ShadowMap mainShadowMap;
    
    ConstantBuffer<WorldData> worldDataBuffer;
    ConstantBuffer<MainLight> mainLightBuffer;
    ConstantBuffer<AdditionalLights> additionalLightBuffer;
    ConstantBuffer<AmbientLight> ambientLightBuffer;
public:
    RenderPipeline();

    void PrepareShadowPass();
    void DrawShadowPass();
    
    void Prepare();
    void Draw();

    void SetMainLight(Light* light);
    int AddAdditionalLight(Light* light);
};
