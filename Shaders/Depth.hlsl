struct MeshData
{
	float4x4 ltw;
	float4 pos;
	float4 color;
    float4 specular;
	float smoothness;
	float pad[3];
};

struct ShadowMap
{
	float4x4 _ViewProj;
};

cbuffer MESH_DATA : register(b0)
{
	MeshData meshData;
}

cbuffer LIGHT_DATA : register(b1)
{
	ShadowMap shadowMap;
}

struct VS_IN
{
	float4 pos : POSITION0;
	float4 normal : NORMAL0;
	//float4 tangent : TANGENT0;
	float4 uv : TEXCOORD0;
};

struct PS_IN
{
    float4 pos : SV_POSITION;
};

PS_IN VSMain(VS_IN input)
{
    PS_IN output = (PS_IN)0;

	float4 posWS = mul(meshData.ltw, float4(input.pos.xyz, 1));
    output.pos = mul(shadowMap._ViewProj, posWS);

    return output;
}

float4 PSMain(PS_IN input) : SV_TARGET
{
	return input.pos;
}
