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
    float4 Emissive : SV_Target6;
    float4 LightMap : SV_Target7;
};


float gamma = 2.2f;


PS_OUTPUT main(VS_OUTPUT input)     // ��� ����ü���� �̹� Semantic �� ����ϰ� �����Ƿ� �ѹ� �� �������� �ʿ�� ����.
{
    PS_OUTPUT output;
    output.Position = input.posWorld;

    float d = input.pos.z / input.pos.w;
    d *= 10;
    output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);
       
    output.Albedo = AMRO.x * gAlbedo.Sample(samLinear, input.tex.xy) + (1 - AMRO.x) * input.color;
    output.Metalic_Roughness.r = AMRO.y * gMetalic.Sample(samLinear, input.tex.xy).r + (1 - AMRO.y) * 0.04f;
    output.Metalic_Roughness.g = AMRO.z * gRoughness.Sample(samLinear, input.tex.xy).r + (1 - AMRO.z);
    
    output.Emissive = useNEOL.y * gEmissive.Sample(samLinear, input.tex.xy);
    
    output.Albedo.a = useNEOL.z * gOpacity.Sample(samLinear, input.tex.xy).r;
    
    output.AO = AMRO.w * gAO.Sample(samLinear, input.tex.xy);
    
    output.LightMap = input.tex.w * pow(gLightMap.Sample(samLinear, float3(input.lightuv, input.tex.z)), gamma); // �⺻�� ����

    float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex.xy).rgb;
    NormalTangentSpace = NormalTangentSpace * 2.0f - 1.0f; //-1~1
    NormalTangentSpace = normalize(NormalTangentSpace);

    float3x3 WorldTransform = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz); //���� �������� �ű������ ���
    output.Normal.xyz = normalize(mul(NormalTangentSpace, (WorldTransform))) * useNEOL.x + (1 - useNEOL.x) * input.normal.xyz;
    
    return output;
    
}