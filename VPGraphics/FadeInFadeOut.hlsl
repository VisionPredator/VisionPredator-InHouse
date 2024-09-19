
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : WORLDPOSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
    float2 lightuv : LIGHTMAPUV;
};


Texture2D gGuffer : register(t0);

sampler gLinear : register(s0);


float4 main(VS_OUTPUT input) : SV_TARGET
{
	return float4(1.0f, .0f, .0f, .0f);
}