#pragma once

#include <map>
#include <string>

#include "InputLayoutOption.h"

class Shader;

class Shaders
{
private:
    std::map<std::wstring, Shader*> shaders;

    static Shaders* s_instance;
public:
    Shaders();

    ~Shaders();
    
    static Shader* Get(std::wstring path, InputLayoutOption layoutOptions);
};
