struct VS_OUTPUT
{
    float4 pos : SV_POSITION; //clip space
    float4 posWorld : TEXCOORD0;
    float4 posClip : TEXCOORD1;
    float4 min : TEXCOORD2;
    float4 max : TEXCOORD3;
    float4 decalInverse0 : TEXCOORD4;
    float4 decalInverse1 : TEXCOORD5;
    float4 decalInverse2 : TEXCOORD6;
    float4 decalInverse3 : TEXCOORD7;
};

struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
};

cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
    float4x4 gProjInverse;
};

cbuffer RenderTargetSize: register(b1)
{
    float4 width_height;
}

//Texture2D Depth : register (t0);
Texture2D Position : register (t0);
Texture2D Decal : register (t1);


SamplerState samLinear : register(s0);

float4 main(VS_OUTPUT input) : SV_TARGET
{
    
    float2 screenPos = input.posClip.xy / input.posClip.w;
    float2 Pos_UV = screenPos * float2(0.5f, -0.5f) + float2(0.5f, 0.5f);
    
    float3 worldPos = Position.Sample(samLinear,Pos_UV);
    
    //float4 decalBoxMin = input.posWorld - (0.5f * input.scale * float4(1, 1, 1, 0));
    //float4 decalBoxMax = input.posWorld + (0.5f * input.scale * float4(1, 1, 1, 0));
    
    float4 decalBoxMin = input.min;

    float4 decalBoxMax = input.max;
    
    if (worldPos.x >= decalBoxMin.x && worldPos.x <= decalBoxMax.x &&
        worldPos.y >= decalBoxMin.y && worldPos.y <= decalBoxMax.y &&
        worldPos.z >= decalBoxMin.z && worldPos.z <= decalBoxMax.z)
    {

        float4x4 inversedecal;
        inversedecal[0] = input.decalInverse0;
        inversedecal[1] = input.decalInverse1;
        inversedecal[2] = input.decalInverse2;
        inversedecal[3] = input.decalInverse3;
        
        float4 posInDecal = mul(worldPos, inversedecal);  
        //clip(0.5 - abs(posInDecal.xyz));
        
        float2 textureCoordinate = posInDecal.xz + 0.5;
        return Decal.Sample(samLinear, textureCoordinate);
    }
    
    discard;
    return float4(0, 1, 0, 1);
}