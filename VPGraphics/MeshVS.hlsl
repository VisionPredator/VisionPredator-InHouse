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
    if (useNE.x > 0)
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
    
    
    //Bone의 world
    output.posWorld = mul(mul(input.pos, skinning), gWorld);
    
    output.pos = mul(output.posWorld, gWorldViewProj);
#endif     

    return output;
}
