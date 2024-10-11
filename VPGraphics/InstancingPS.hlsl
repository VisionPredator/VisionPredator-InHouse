cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
    float4x4 gProjInverse;
};



//Material
cbuffer Material : register(b1)
{
    float4 AMRO;
    float4 useNEOL; //normal, Emissive, opacity, LightMap
    float4 albedo;
    float metalness;
    float roughness;
    float ao; // Ambient Occlusion
    float pad;
    float4 lightmapdata; //index, offset(x,y),scale
    float2 lightmaptiling; // x y
}; 



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
Texture2DArray gLightMap : register(t9);


SamplerState samLinear : register(s0);
    
struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TEXCOORD0;
    float4 bitangent : TEXCOORD1;
    float4 tex : TEXCOORD2;
    float2 lightuv : TEXCOORD3;
    float4 posWorld : TEXCOORD4;
};

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

PS_OUTPUT main(VS_OUTPUT input)     // 출력 구조체에서 이미 Semantic 을 사용하고 있으므로 한번 더 지정해줄 필요는 없다.
{
    PS_OUTPUT output;
    output.Position = input.posWorld;

    float d = input.pos.z / input.pos.w;
    d *= 10;
    output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);
    output.Albedo = input.color;
       
    if (AMRO.x > 0)
    {
        output.Albedo = gAlbedo.Sample(samLinear, input.tex.xy);
    }
    
    output.Metalic_Roughness = float4(0, 0, 0, 1);
    output.Metalic_Roughness.r = 0.04f;
    if (AMRO.y >= 1)
    {
        output.Metalic_Roughness.r = gMetalic.Sample(samLinear, input.tex.xy).r;
    }
    
    output.Metalic_Roughness.g = 1.f;
    
    if (AMRO.z >= 1)
    {
        output.Metalic_Roughness.g = gRoughness.Sample(samLinear, input.tex.xy).r;
    }
    
    output.AO = 0.f;
    
    if (AMRO.w >= 1)
    {
        output.AO = gAO.Sample(samLinear, input.tex.xy);
    }
    
    output.Normal = input.normal;
    if (useNEOL.x >= 1)
    {
        float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex.xy).rgb;
        NormalTangentSpace = NormalTangentSpace * 2.0f - 1.0f; //-1~1
        NormalTangentSpace = normalize(NormalTangentSpace);

        float3x3 WorldTransform = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz); //면의 공간으로 옮기기위한 행렬
        output.Normal.xyz = normalize(mul(NormalTangentSpace, (WorldTransform)));
    }
	
    output.Emissive = 0;
    if (useNEOL.y >= 1)
    {
        output.Emissive = gEmissive.Sample(samLinear, input.tex.xy);
    }
    
    if (useNEOL.z >= 1)
    {
        output.Albedo.a = gOpacity.Sample(samLinear, input.tex.xy).r;
    }
    
    float gamma = 2.2f;
    uint index = (uint(input.tex.z)); // texZ를 uint로 변환
    output.LightMap = pow(gLightMap.Sample(samLinear, float3(input.lightuv, index)),gamma); // 기본값 설정
    return output;
    
}