#include "Common.hlsli"


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.posWorld = mul(input.pos, gWorld);
    output.pos = mul(output.posWorld, gWorldViewProj);
    output.color = input.color;
    output.normal = input.normal;
    output.tangent = input.tangent;
    output.bitangent = input.bitangent;
    output.tex = input.tex;
           
    /*
    */
    if (useNEO.x > 0)
    {
        float4x4 meshWorld = gWorldInverse; //메쉬의 월드 공간
        
        //방향 vector
        float3 vTangent = normalize(mul(float4(input.tangent.xyz, 0), meshWorld));
        float3 vBitangent = normalize(mul(float4(input.bitangent.xyz, 0), meshWorld));
        float3 vNormal = normalize(mul(float4(input.normal.xyz, 0), meshWorld));
                        
        //색상 표현을 위해 점으로 저장 w == 1
        output.normal = float4(vNormal.xyz, 1);        
        output.tangent = float4(vTangent.xyz, 0);
        output.bitangent = float4(vBitangent.xyz, 0);        
    }
    
    
    
#ifdef SKINNING
    int index[8];
    index[0] = input.boneindex[0].x;
    index[1] = input.boneindex[0].y;
    index[2] = input.boneindex[0].z;
    index[3] = input.boneindex[0].w;
    
    index[4] = input.boneindex[1].x;
    index[5] = input.boneindex[1].y;
    index[6] = input.boneindex[1].z;
    index[7] = input.boneindex[1].w;
    
    
    float weight[8];
    weight[0] = input.boneweight[0].x;
    weight[1] = input.boneweight[0].y;
    weight[2] = input.boneweight[0].z;
    weight[3] = input.boneweight[0].w;
    
    weight[4] = input.boneweight[1].x;
    weight[5] = input.boneweight[1].y;
    weight[6] = input.boneweight[1].z;
    weight[7] = input.boneweight[1].w;
    
    float4x4 skinning;
    
    skinning =  mul(saturate(weight[0]), MatrixPallete[index[0]]);
    skinning += mul(saturate(weight[1]), MatrixPallete[index[1]]);
    skinning += mul(saturate(weight[2]), MatrixPallete[index[2]]);
    skinning += mul(saturate(weight[3]), MatrixPallete[index[3]]);
    skinning += mul(saturate(weight[4]), MatrixPallete[index[4]]);
    skinning += mul(saturate(weight[5]), MatrixPallete[index[5]]);
    skinning += mul(saturate(weight[6]), MatrixPallete[index[6]]);
    skinning += mul(saturate(weight[7]), MatrixPallete[index[7]]);
    
    
    //Bone의 world
    output.posWorld = mul(mul(input.pos, skinning), gWorld);
    
    output.pos = mul(output.posWorld, gWorldViewProj);
#endif     

    return output;
}
