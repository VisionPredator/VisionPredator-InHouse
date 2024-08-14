Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

cbuffer ColorCB : register(b1)
{
	float4 gColor;
}

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 color;

	color = shaderTexture.Sample(SampleType, input.uv);
	color *= gColor;	// 왜 값이 제대로 안들어가지..

	return color;
}