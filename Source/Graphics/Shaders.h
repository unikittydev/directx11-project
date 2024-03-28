#pragma once

#include <map>
#include <string>

#include "InputLayoutOption.h"
#include "RastState.h"

class Shader;

class Shaders
{
private:
    std::map<std::wstring, Shader*> shaders;

    static Shaders* s_instance;
public:
    Shaders();

    ~Shaders();
    
    static Shader* Get(const std::wstring& path, InputLayoutOption layoutOptions, RastState rsState);
};
