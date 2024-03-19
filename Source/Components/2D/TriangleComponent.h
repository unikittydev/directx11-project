#pragma once

#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

#include "../../../ThirdParty/SimpleMath.h"
#include "../../Graphics/Shader.h"
#include "../IComponent.h"

class TriangleComponent : public IComponent
{
private:
	Shader shader{ L"./Shaders/MyVeryFirstShader.hlsl" };

	ID3D11Buffer* vertexBuffer;

	struct Vertex
	{
		float4 position;
		float4 color;
	};

	Vertex points[3] =
	{
		Vertex{ float4(0.5, 0.5, 0, 1), float4(1, 0, 0, 1) },
		Vertex{ float4(0.5, -0.5, 0, 1), float4(0, 1, 0, 1) },
		Vertex{ float4(-0.5, -0.5, 0, 1), float4(0, 0, 1, 1) },
	};

public:
	void Init() override;
	void Draw() override;
	void Destroy() override;
};