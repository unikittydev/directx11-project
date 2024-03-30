#pragma once

#include <Core/Types.h>
#include <Graphics/Texture2D.h>

struct ID3D11Buffer;
class Shader;

class PostFX
{
private:
    ID3D11Buffer* postFXBuffer;

    Texture2D rtvTex;
    
    uint width, height;

    Shader* shader;

    void TryResize();

public:
    PostFX();

    void Draw();
};