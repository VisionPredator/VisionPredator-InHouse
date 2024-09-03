cbuffer ColorBuffer : register(b5)
{
	float4 gColor;
}

struct VertexOut
{
	float4 PosWorld : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(VertexOut input) : SV_TARGET
{
	return gColor;
	//return input.Color;
}
