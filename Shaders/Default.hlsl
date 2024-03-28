
struct WorldData
{
	float4x4 _WorldViewProj;
	float4 _Time;
};

struct MeshData
{
	float4x4 ltw;
	float4 pos;
	float4 color;
    float4 specular;
	float smoothness;
	float pad[3];
};

struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
 	float4 uv : TEXCOORD0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
 	float4 uv : TEXCOORD0;
};

cbuffer WORLD_DATA : register(b0)
{
	WorldData worldData;
}

cbuffer MESH_DATA : register(b1)
{
	MeshData meshData;
}

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = mul(meshData.ltw, float4(input.pos.xyz, 1));
	output.col = input.col * meshData.color;
	output.uv = input.uv;
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	return 1;
}