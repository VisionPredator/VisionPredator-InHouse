#include "Common.hlsli"

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float3 albedoColor;
     
    float metallicValue;
    float roughnessValue;
    float aoValue;
    //���� �Ի� ���� �ݻ��� - ��ݼ��̸� 0.04 �ݼ��̸� metalic RGB �𸮾�4�� �̷��� ����
    float3 F0 = Fdielectric;
    
     //// Calculate Directional Light 
 
    float4 albedo = pow(gAlbedo.Sample(samLinear, input.tex),gamma);
    float3 Depth = gDepth.Sample(samLinear, input.tex);
    
    float3 result = float3(0, 0, 0);
    float4 emissive;    
    
    //View
    float3 V = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
   
    //tangentspace�� ����� normal�� �����
    float4x4 meshWorld = gWorldInverse;
    float4 vTangent = (input.tangent);
    float4 vBitangent = (input.bitangent);
    float4 vNormal = (input.normal);
        
    float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb; //-1~1
    NormalTangentSpace = NormalTangentSpace * 2.0f - 1.0f; //-1~1
    
    //float3x3 WorldTransform = float3x3(input.tangent.xyz, input.bitangent.xyz, input.normal.xyz); //���� �������� �ű������ ���
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //���� �������� �ű������ ���
        
    float4 N;
    N.xyz = normalize(mul(NormalTangentSpace, WorldTransform));
    /*
    */
  
    if (AMRO.x > 0)
    {
        albedoColor = gAlbedo.Sample(samLinear, input.tex).rgb;
    }
    else
    {
        albedoColor = input.color;
    }
    
    if (AMRO.y > 0)
    {
        metallicValue = gMetalic.Sample(samLinear, input.tex).r;
        F0 = lerp(F0, albedoColor, metallicValue);
        
    }
    else
    {
        metallicValue = 0.04f; //��ݼ��̸� 0.04 �ݼ��̸� metalic RGB �𸮾�4�� �̷��� ����
    }
    
    if (AMRO.z > 0)
    {
        roughnessValue = gRoughness.Sample(samLinear, input.tex).r;
    }
    else
    {
        roughnessValue = 1;
    }
    
    if (AMRO.w > 0)
    {
        aoValue = gAO.Sample(samLinear, input.tex).r;
    }
    else
    {
        aoValue = 0;
    }
        
    if(useNE.y > 0)
    {
     emissive = gEmissive.Sample(samLinear, input.tex);    
        
    }
    else
    {
        emissive = 0;
    }
    
    
    for (int i = 0; i < DirIndex; i++)
    {
        result += CalcDir(array[i], V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);
    }

    // Calculate Spot Light    
    for (int k = DirIndex; k < SpotIndex; k++)
    {
        result += CalcSpot(array[k], input.posWorld, V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);
    }
    // Calculate Point Light
    for (int j = DirIndex + SpotIndex; j < DirIndex + SpotIndex + PointIndex; j++)
    {
        result += CalcPoint(array[j], input.posWorld, V, N.xyz, F0, albedoColor, roughnessValue, metallicValue, Depth);

    }
    
    float3 lightColor = float3(1.0, 1.0, 1.0); // Assuming white light

    // ambient lighting (constant factor for simplicity)
    float3 ambient = aoValue * albedoColor;

    float3 color = ambient + result;

    
    // HDR tonemapping
    color = color / (color + float3(1.0, 1.0, 1.0));
    
    // gamma correct
    color = pow(color, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2));
       
    return float4(color, 1);

}