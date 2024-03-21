
/*struct MainLight
{
    float4 dir;
    float4 color;
};

struct PointLight
{
    float4 pos;
    float4 color;
}*/

/* STRUCTS */
struct WorldData
{
	float4x4 _WorldViewProj;
	float4 _Time;

	//MainLight _MainLight;
    //PointLight _AdditionalLights[8];
    //int _AdditionalLightsCount;
};

struct MeshData
{
	float4 color;
	float4x4 ltw;
};

struct VS_IN
{
	float4 pos : POSITION0;
	float4 normal : NORMAL0;
	//float4 tangent : TANGENT0;
	float4 uv : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION0;
	float4 normal : NORMAL0;
	//float4 tangent : TANGENT0;
	float4 uv : TEXCOORD0;
};

/* BUFFERS */

cbuffer WORLD_DATA : register(b0)
{
	WorldData worldData;
}

cbuffer MESH_DATA : register(b1)
{
	MeshData meshData;
}

/* TEXTURES */

Texture2D _mainTex : register(t0);
SamplerState _mainTex_Sampler : register(s0);

/* SHADER */

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(float4(input.pos.xyz, 1), worldData._WorldViewProj);
	output.normal = mul(meshData.ltw, input.normal);
	//output.tangent = input.tangent;
	output.uv = input.uv;
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	float4 color = _mainTex.Sample(_mainTex_Sampler, input.uv);
	input.normal = normalize(input.normal);
	return color * dot(input.normal, normalize(float3(1, 1, 1))) + .3f;
}