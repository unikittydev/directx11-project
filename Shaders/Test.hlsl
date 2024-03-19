
struct WorldData
{
	float4x4 _WorldViewProj;
};

struct MeshData
{
	float4 color;
};

struct VS_IN
{
	float4 pos : POSITION0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
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
	
	output.pos = mul(float4(input.pos.xyz, 1), worldData._WorldViewProj);
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	return 1;
}