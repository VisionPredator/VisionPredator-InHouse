struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : TEXCOORD0;
    float4 tex : TEXCOORD1;
};

struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
};


float4 main() : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}