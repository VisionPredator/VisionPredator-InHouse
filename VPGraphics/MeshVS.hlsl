#include "Common.hlsli"


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.posWorld = mul(input.pos, gWorld);
    output.pos = mul(output.posWorld, gWorldViewProj);
    output.color = input.color;
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.bitangent = input.bitangent, output.pos;
    output.tex = input.tex;
       
#ifdef SKINNING
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
    output.posWorld = mul(mul(input.pos, skinning), gWorld);
    
    output.pos = mul(output.posWorld, gWorldViewProj);
#endif    
    
    
    
    
    return output;
}