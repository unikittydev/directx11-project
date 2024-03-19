
struct VertexIn
{
	float4 position : POSITION;
	float4 color : COLOR;
}

struct PixelIn
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
}

PixelIn VSMain(VertexIn in)
{
	PixelIn out = (PixelIn)0;
	out.position = in.position;
	out.color = in.color;
	return out;
}

float4 PSMain(PixelIn in) : SV_TARGET
{
	return in.color;
}
