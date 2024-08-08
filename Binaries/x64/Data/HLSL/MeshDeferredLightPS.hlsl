#include"Common.hlsli"

struct PS_OUTPUT
{
    float4 Gbuffer : SV_Target0;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_TARGET
{
    PS_OUTPUT output;
    
    float4 position = gPosition.Sample(samLinear, input.tex);   
    float4 N = gNormal.Sample(samLinear, input.tex);
    
    float3 result = float3(0, 0, 0);
    
    //View
    float3 V = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - position.xyz);
    
    //texture sampling
    float3 albedoColor = pow(gAlbedo.Sample(samLinear, input.tex).rgb, float3(gamma, gamma, gamma)); //Gamma Correction ���⼭�� ������
    float metallicValue = gMetalic.Sample(samLinear, input.tex).r; //quad�� �ؽ�ó�� ���ø��ؼ� �̻��ϰ� �����°ſ���
    float roughnessValue = gRoughness.Sample(samLinear, input.tex).r;
    float aoValue = gAO.Sample(samLinear, input.tex).r;
    float3 EmissiveValue = pow(gEmissive.Sample(samLinear, input.tex).rgb, float3(gamma, gamma, gamma));
    float3 Depth = gDepth.Sample(samLinear, input.tex);
    
        
     //���� �Ի� ���� �ݻ��� - ��ݼ��̸� 0.04 �ݼ��̸� metalic RGB �𸮾�4�� �̷��� ����
    float3 F0 = Fdielectric;
    F0 = lerp(F0, albedoColor, max(0.04, metallicValue));
    
    for (int i = 0; i < DirIndex; i++)
    {
        result += CalcDir(array[i], V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);
    }

    // Calculate Spot Light    
    for (int k = DirIndex; k < DirIndex + SpotIndex; k++)
    {        
        result += CalcSpot(array[k], position, V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);
    }
    // Calculate Point Light
    for (int j = DirIndex + SpotIndex; j < DirIndex + SpotIndex + PointIndex; j++)
    {
        result += CalcPoint(array[j], position, V, N.xyz, F0, albedoColor, roughnessValue, metallicValue, Depth);

    }
    

    // ambient lighting (constant factor for simplicity)
    float3 ambient = aoValue * albedoColor;

    result = ambient + result;
 
    // HDR tonemapping
    result = result / (result + float3(1.0, 1.0, 1.0)) + EmissiveValue;
    
    // gamma correct
    result = pow(result, float3(1.0 / gamma, 1.0 / gamma, 1.0 / gamma));
        
    output.Gbuffer = float4(result, 1);
    
    return output;
    //return float4(albedoColor, 1);
}