struct VS_IN
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
};

struct PS_IN
{
	float4 pos : SV_POSITION;
 	float4 col : COLOR;
};

struct Offset
{
	float4 value;
};

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	Offset offset;
}

PS_IN VSMain(VS_IN input)
{
	PS_IN output = (PS_IN)0;
	
	output.pos = input.pos + offset.value;
	output.col = input.col;
	
	return output;
}

float4 PSMain(PS_IN input) : SV_Target
{
	return input.col;
}