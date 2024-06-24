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
    float3 albedoColor = gAlbedo.Sample(samLinear, input.tex).rgb;
    float metallicValue = gMetalic.Sample(samLinear, input.tex).r;  //quad에 텍스처를 샘플링해서 이상하게 나오는거였음
    float roughnessValue = gRoughness.Sample(samLinear, input.tex).r;
    float aoValue = gAO.Sample(samLinear, input.tex).r;
    float3 EmissiveValue = gEmissive.Sample(samLinear, input.tex).r;
    
     //수직 입사 시의 반사율 - 비금속이면 0.04 금속이면 metalic RGB 언리얼4는 이렇게 쓴다
    float3 F0 = Fdielectric;
    F0 = lerp(F0, albedoColor, metallicValue);
    
    
    for (int i = 0; i < DirIndex; i++)
    {
        result += CalcDir(array[i], V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);
    }

    // Calculate Spot Light    
    for (int k = DirIndex; k < SpotIndex; k++)
    {
        result += CalcSpot(array[i], position, V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);
    }
    // Calculate Point Light
    for (int j = SpotIndex; j < PointIndex; j++)
    {
        result += CalcPoint(array[i], position, V, N.xyz, F0, albedoColor, roughnessValue, metallicValue);

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
    result = pow(result, float3(1.0 / 2.2, 1.0 / 2.2, 1.0 / 2.2)) + EmissiveValue;
        
    output.Gbuffer = float4(result, 1);
    //output.Gbuffer = float4(albedoColor, 1);
    
    return output;
    //return float4(albedoColor, 1);
}