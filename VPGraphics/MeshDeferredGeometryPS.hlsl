#include "Common.hlsli"

/// <summary>
///
/// The pixel shader for the geometry pass in the first pass of "Deferred Shading".
///
/// </summary>


/*
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


//Material
cbuffer Material : register(b0)
{
    float4 AMRO;
    float4 useNEO;
    float4 albedo;
    float metalness;
    float roughness;
    float ao; // Ambient Occlusion
    float pad;
    float4 lightmapdata; //index, offset(x,y),scale
    float2 lightmaptiling; // x y
};
// Render to texture.
// Using Multi Render Target.


//TEXTURE
Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
Texture2D gDepth : register(t3);
Texture2D gMetalic : register(t4);
Texture2D gRoughness : register(t5);
Texture2D gAO : register(t6);
Texture2D gEmissive : register(t7);
Texture2D gOpacity : register(t8);
Texture2D gLightMap : register(t9);

//sampler
SamplerState samLinear : register(s0);

*/



struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Depth : SV_Target3;
    float4 Metalic_Roughness : SV_Target4;
    float4 AO : SV_Target5;
    float4 LightMap : SV_Target6;
    float4 Emissive : SV_Target7;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    output.Position = input.posWorld;

    float d = input.pos.z / input.pos.w;
    d *= 10;
    output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);
    output.Albedo = input.color;
       
    if (AMRO.x > 0)
    {
        output.Albedo = gAlbedo.Sample(samLinear, input.tex);
    }
    
    output.Metalic_Roughness = float4(0, 0, 0, 1);
    output.Metalic_Roughness.r = 0.04f;
    if (AMRO.y >= 1)
    {
        output.Metalic_Roughness.r = gMetalic.Sample(samLinear, input.tex).r;
    }
    
    output.Metalic_Roughness.g = 1.f;
    
    if (AMRO.z >= 1)
    {
        output.Metalic_Roughness.g = gRoughness.Sample(samLinear, input.tex).r;
    }
    
    output.AO = 0.f;
    
    if (AMRO.w >= 1)
    {
        output.AO = gAO.Sample(samLinear, input.tex);
    }
    
    output.Normal = input.normal;
    if (useNEOL.x >= 1)
    {
        float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb;
        NormalTangentSpace = NormalTangentSpace * 2.0f - 1.0f; //-1~1
        NormalTangentSpace = normalize(NormalTangentSpace);
        
        float3x3 WorldTransform = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz); //면의 공간으로 옮기기위한 행렬
        output.Normal.xyz = normalize(mul(NormalTangentSpace, (WorldTransform)));
    }
	
    output.Emissive = 0;
    if (useNEOL.y >= 1)
    {
        output.Emissive = gEmissive.Sample(samLinear, input.tex);
    }
    
    if(useNEOL.z >= 1)
    {
        output.Albedo.a = gOpacity.Sample(samLinear, input.tex).r;
    }
    //반대로 저장되어있어서 일단 이거쓰자
    //float x = (input.lightuv.x * lightmapdata.y) + lightmaptiling.x;
    //float y = ((1 - input.lightuv.y) * lightmapdata.z) +lightmaptiling.y;
    
    //float x = (input.lightuv.x * lightmaptiling.x) + lightmapdata.y;
    //float y = ((1 - input.lightuv.y) * lightmaptiling.y) + lightmapdata.z;
    
    
    ////Unity light map은 좌하단이 0,0 dx11 좌상단이 0,0
    ////y 반전 필요
    //float2 uv = float2(x, 1 - y);    
    
    output.LightMap = pow(gLightMap.Sample(samLinear, input.lightuv), float4(gamma, gamma, gamma,1));
    //output.LightMap = (gLightMap.Sample(samLinear, uv));
    
    return output;
    
}