#include "Common.hlsli"

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 ambient  = { 0, 0, 0, 0 };
    float4 diffuse  = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };
    
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    
    //텍스처 색상
    float4 albedo = gAlbedo.Sample(samLinear, input.tex);
    //float4 albedo = input.color;
    
    //tangentspace를 계산해 normal을 만든다
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //면의 공간으로 옮기기위한 행렬
    vNormal.xyz = mul(NormalTangentSpace, WorldTransform);
    vNormal.xyz = normalize(vNormal.xyz);
    
    for (int i = 0; i < DirIndex; i++)
    {
        CalcDir(Dir[i], eyepos, vNormal, ambient, diffuse, specular);
    }
    
    for (int i = 0; i < PointIndex; i++)
    {
        CalcPoint(Point[i], eyepos, input.posWorld.xyz, vNormal, ambient, diffuse, specular);
    }
    
    for (int i = 0; i < SpotIndex; i++)
    {
        CalcSpot(Spot[i], eyepos, input.posWorld.xyz, vNormal, ambient, diffuse, specular);
    }
    
    
    
    float4 litColor = { 0, 0, 0, 0 };
    litColor = ambient * albedo + diffuse * albedo + specular;
    
	return litColor;
}