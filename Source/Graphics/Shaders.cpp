#include "Shaders.h"
#include "Shader.h"

Shaders* Shaders::s_instance = nullptr;

Shaders::Shaders()
{
    s_instance = this;
    shaders = {};
}

Shaders::~Shaders()
{
    //for (const auto& kv : shaders)
    //{
    //    kv.second->Release();
    //    delete kv.second;
    //}
}

Shader* Shaders::Get(std::wstring path, InputLayoutOption layoutOptions, bool solid)
{
    Shader* shader = s_instance->shaders[path];
    if (shader == nullptr)
    {
        shader = new Shader(path, layoutOptions, solid);
        s_instance->shaders[path] = shader;
    }
    return shader;
}
