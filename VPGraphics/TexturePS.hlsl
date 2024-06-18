#include "Common.hlsli"

struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : WORLDPOSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
    
};

float4 main(PS_INPUT input) : SV_TARGET
{

     //// Calculate Directional Light 
 
    float4 albedo = gAlbedo.Sample(samLinear, input.tex);
    float4 position = gPosition.Sample(samLinear, input.tex);
    float4 normal = gNormal.Sample(samLinear, input.tex);
    
    
    float4 N = gNormal.Sample(samLinear, input.tex);
    
    float3 result = float3(0, 0, 0);
    
    //View
    float3 V = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
   
    /*
    //tangentspace�� ����� normal�� �����
    float4 N = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, N.xyz); //���� �������� �ű������ ���
    N.xyz = mul(NormalTangentSpace, WorldTransform);
    N.xyz = normalize(N.xyz);
    */
  
    //texture sampling
    float3 albedoColor = gMeshAlbedo.Sample(samLinear, input.tex).rgb;
    float metallicValue = gMeshMetalic.Sample(samLinear, input.tex).r;
    float roughnessValue = gMeshRoughness.Sample(samLinear, input.tex).r;
    float aoValue = gMeshAO.Sample(samLinear, input.tex).r;
    
     //���� �Ի� ���� �ݻ��� - ��ݼ��̸� 0.04 �ݼ��̸� metalic RGB �𸮾�4�� �̷��� ����
    float3 F0 = Fdielectric;
    F0 = lerp(F0, albedoColor, metallicValue);
    
    
    for (int i = 0; i < DirIndex; i++)
    {
        result += CalcDir(Dir[i], V, N.xyz, F0, albedoColor, roughnessValue);
    }

    // Calculate Point Light


    // Calculate Spot Light    
    for (int j = 0; j < PointIndex; j++)
    {
        result += CalcPoint(Point[i], V, N.xyz, F0, albedoColor, roughnessValue);

    }
    
    for (int k = 0; k < SpotIndex; k++)
    {
        result += CalcSpot(Spot[i], V, N.xyz, F0, albedoColor, roughnessValue);
    }
    
    float3 lightColor = float3(1.0, 1.0, 1.0); // Assuming white light

    // ambient lighting (constant factor for simplicity)
    float3 ambient = aoValue * albedoColor;

    float3 color = ambient + result;

    // HDR tonemapping
    //color = color / (color + float3(1.0, 1.0, 1.0));
    // gamma correct
    //color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
 
    // HDR tonemapping
    result = result / (result + float3(1.0, 1.0, 1.0));
    
    // gamma correct
    result = pow(result, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
    
    return float4(result, 1);
    

}