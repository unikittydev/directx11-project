#pragma once

#include <string>
#include <DirectXMath.h>
#include <d3d11.h>
#include <iostream>

#include "../../../ThirdParty/SimpleMath.h"
#include "../../Graphics/Shader.h"
#include "../IComponent.h"

class QuadComponent : public IComponent
{
private:
	Shader shader{ L"./Shaders/Racket.hlsl" };

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	struct Vertex
	{
		float4 position;
		float4 color;
	};

	Vertex vertices[4] =
	{
		Vertex{ float4(1, 1, 0, 1), float4(1, 0, 0, 1) },
		Vertex{ float4(1, -1, 0, 1), float4(0, 1, 0, 1) },
		Vertex{ float4(-1, -1, 0, 1), float4(0, 0, 1, 1) },
		Vertex{ float4(-1, 1, 0, 1), float4(1, 0, 1, 1) },
	};

	UINT indices[6] = { 0, 1, 2, 2, 3, 0 };

protected:
	ID3D11Buffer* constantBuffer;

	float4 size;
	float4 location;

public:
	void Init() override;
	void Draw() override;
	void Update() override;
	void Destroy() override;

	void SetRect(float4 center, float4 extends)
	{
		const float4 rect[4] = {
			float4(1, 1, 0, 1),
			float4(1, -1, 0, 1),
			float4(-1, -1, 0, 1),
			float4(-1, 1, 0, 1)
		};

		for (int i = 0; i < 4; i++)
			vertices[i].position = rect[i] * extends;

		size = extends;
		location = center;
	}

	void SetColor(float4 color)
	{
		for (int i = 0; i < 4; i++)
			vertices[i].color = color;
	}
};