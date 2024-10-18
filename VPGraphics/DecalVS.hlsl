
cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
    float4x4 gProjInverse;
};


struct VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
    float2 lightuv : LIGHTMAPUV;
    
    //instancing data
    float4x4 world : WORLD;
    float4x4 worldinverse : WORLDINVERSE;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : TEXCOORD0;
    float4 tex : TEXCOORD1; 
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
            
    output.posWorld = mul(input.pos, input.world);
    output.pos = mul(output.posWorld, gWorldViewProj);
    output.tex.xy = input.tex.xy;
    
    
    return output;
}