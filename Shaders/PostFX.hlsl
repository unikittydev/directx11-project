
Texture2D blit : register(t0);
SamplerState blit_Sampler : register(s0);

struct Vertex
{
	float4 pos : SV_POSITION;
	float4 uv : TEXCOORD0;
};

Vertex VSMain(uint vI : SV_VERTEXID)
{
    float2 texcoord = float2(vI & 1,vI >> 1);
	
	Vertex v = (Vertex)0;
	v.pos = float4((texcoord.x - 0.5f) * 2, -(texcoord.y - 0.5f) * 2, 0, 1);
	v.uv = float4(texcoord.x, texcoord.y, 0, 0);
    return v;
}

float4 PSMain(Vertex v) : SV_TARGET
{
	float4 color = blit.Sample(blit_Sampler, v.uv);
	float3 col = color.rgb;
	col = dot(col.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    return float4(col, 1);// float4(dot(color.rgb, float3(0.2126f, 0.7152f, 0.0722f)), 1); //the red color
}