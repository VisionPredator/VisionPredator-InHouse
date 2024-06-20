#include "Common.hlsli"

/// <summary>
///
/// The pixel shader for the geometry pass in the first pass of "Deferred Shading".
///
/// </summary>

// Render to texture.
// Using Multi Render Target.
struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Depth : SV_Target3;
    float4 Metalic : SV_Target4;
    float4 Roughness : SV_Target5;
    float4 AO : SV_Target6;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_TARGET
{
    PS_OUTPUT output;

    output.Albedo = input.color;
    if(AMRO.x > 0)
    {
        output.Albedo = gAlbedo.Sample(samLinear, input.tex);
    }
	
	
	 //tangentspace를 계산해 normal을 만든다
    float4 N = normalize(input.normal);
    if (useN > 0)
    {
        float4 vTangent = normalize(input.tangent);
        float4 vBitangent = normalize(input.bitangent);
        
        float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
        float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, N.xyz); //면의 공간으로 옮기기위한 행렬
        N.xyz = mul(NormalTangentSpace, WorldTransform);
        N.xyz = normalize(N.xyz);
    }
	
    output.Normal = N;
	
	
	
    output.Position = input.posWorld;
    output.Metalic = gMetalic.Sample(samLinear, input.tex);
    output.Roughness = gRoughness.Sample(samLinear, input.tex);
    output.AO = gAO.Sample(samLinear, input.tex);

    float d = input.pos.z / input.pos.w;
    d *= 10;
    output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);
    return output;
}