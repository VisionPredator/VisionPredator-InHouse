
cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
}

cbuffer Transform : register(b1)
{
    float4x4 gWorld;
};

cbuffer Transform : register(b2)
{
    float4x4 gLocal;
};


struct VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : WORLDPOSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT test;
        
    test.posWorld = mul(mul(input.pos, gLocal), gWorld);
    //test.posWorld = mul(input.pos, gWorld);
    test.pos = mul(test.posWorld, gWorldViewProj);
    test.color = input.color;
    test.normal = input.normal;
    test.tangent = input.tangent;
    test.bitangent = input.bitangent, test.pos;
    test.tex = input.tex;
    
    return test;
}
