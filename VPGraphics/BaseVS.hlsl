
cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
};

cbuffer Transform : register(b1)
{
    float4x4 gLocal;
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;

};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.pos = mul(input.pos, gLocal);
    output.pos = mul(output.pos, gWorldViewProj);
    output.color = input.color;
    
    return output;
    
}

