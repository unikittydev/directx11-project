#include "LightingData.h"

#include "Components/Transform.h"
#include "Components/Light/Light.h"


void LightingData::SetMainLight(Light* light)
{
    main = light;
}

int LightingData::AddAdditionalLight(Light* light)
{
    additional.push_back(light);
    return additional.size() - 1;
}

LightingData::MainLight LightingData::GetMainLightData() const
{
    return MainLight{};//return MainLight{ main->GetTr()->GetForward(), main->GetColor() };
}

std::vector<LightingData::AdditionalLight> LightingData::GetAdditionalLightData() const
{
    std::vector<LightingData::AdditionalLight> data{};

    for (const auto light : additional)
    {
        //data.push_back({ float4{ light->GetTr()->GetWorldTranslation() }, light->GetColor() });
    }

    return data;
}