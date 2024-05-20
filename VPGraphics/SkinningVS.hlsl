
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

cbuffer MatrixPallete : register(b3)
{
    matrix MatrixPallete[128];
}


struct VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
    float4 boneindex : BONEINDEX;
    float4 boneweight : BONEWEIGHT;
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
    //float4 boneindex : BONEINDEX;
    //float4 boneweight : BONEWEIGHT;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT test;
    
    int index[4];
    index[0] = input.boneindex.x;
    index[1] = input.boneindex.y;
    index[2] = input.boneindex.z;
    index[3] = input.boneindex.w;
    
    float weight[4];
    weight[0] = input.boneweight.x;
    weight[1] = input.boneweight.y;
    weight[2] = input.boneweight.z;
    weight[3] = input.boneweight.w;
    
    float4x4 skinning;
    
    skinning = mul(saturate(weight[0]), MatrixPallete[index[0]]);
    skinning += mul(saturate(weight[1]), MatrixPallete[index[1]]);
    skinning += mul(saturate(weight[2]), MatrixPallete[index[2]]);
    skinning += mul(saturate(weight[3]), MatrixPallete[index[3]]);
    
    
    //Bone¿« world
    test.posWorld = mul(mul(input.pos, skinning), gWorld);
    
    test.pos = mul(test.posWorld, gWorldViewProj);
    test.color = input.color;
    test.normal = input.normal;
    test.tangent = input.tangent;
    test.bitangent = input.bitangent, test.pos;
    test.tex = input.tex;
    
    
    return test;
}
