#pragma once
#include <vector>

#include "Core/Types.h"

class Light;

class LightingData
{
private:
    Light* main;
    std::vector<Light*> additional{};
    
public:
    struct MainLight
    {
        float4 direction;
        float4 color;
    };
    
    struct AdditionalLight
    {
        float4 position;
        float4 color;
    };

    void SetMainLight(Light* light);
    int AddAdditionalLight(Light* light);
    
    MainLight GetMainLightData() const;
    std::vector<AdditionalLight> GetAdditionalLightData() const;
};
