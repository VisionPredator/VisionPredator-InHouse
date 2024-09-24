
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


Texture2D gBuffer : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
Texture2D gMask : register(t3);

sampler gLinear : register(s0);


cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
    float4x4 gProjInverse;
};


float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 Mask = gMask.Sample(gLinear, input.tex);
    float4 result = gBuffer.Sample(gLinear, input.tex);
    
    if(Mask.x + Mask.y + Mask.z > 0)
    {
        float4 pos = gPosition.Sample(gLinear, input.tex);
        float4 N = gNormal.Sample(gLinear, input.tex);
        float3 V = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - pos.xyz);

        float rim = 1 - saturate(dot(N, V));
        rim = smoothstep(0, 1.0f, rim);
    
        return rim * float4(1, 0, 0, 1) + result;
    }
    
    
    
    return result;

}