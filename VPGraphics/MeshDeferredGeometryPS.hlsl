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
    float4 Emissive : SV_Target7;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_TARGET
{
    PS_OUTPUT output;

    output.Albedo = input.color;
    if (AMRO.x > 0)
    {
        output.Albedo = gAlbedo.Sample(samLinear, input.tex);
    }
	
    output.Metalic = 0.04;
    if (AMRO.y > 0)
    {
        output.Metalic = gMetalic.Sample(samLinear, input.tex);
    }
    
    output.Roughness = 0.f;
    
    if (AMRO.z > 0)
    {
        output.Roughness = gRoughness.Sample(samLinear, input.tex);
    }
    
    output.AO = 0.f;
    
    if (AMRO.w > 0)
    {
        output.AO = gAO.Sample(samLinear, input.tex);
    }
    
    output.Normal = input.normal;
    if (useNE.x > 0)
    {
        
        float3 NormalTangentSpace = gNormal.SampleLevel(samLinear, input.tex, 0).rgb;
        NormalTangentSpace = NormalTangentSpace * 2.0f - 1.0f; //-1~1
        NormalTangentSpace = normalize(NormalTangentSpace);
        
        float3x3 WorldTransform = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz); //면의 공간으로 옮기기위한 행렬
        output.Normal.xyz = normalize(mul(NormalTangentSpace, WorldTransform));
    }
	
    output.Emissive = 0;
    if (useNE.y > 0)
    {
        output.Emissive = gEmissive.Sample(samLinear, input.tex);
    }
	
    output.Position = input.posWorld;

    float d = input.pos.z / input.pos.w;
    d *= 10;
    output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);
        
    return output;
}