#include "Common.hlsli"

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 ambient  = { 0, 0, 0, 0 };
    float4 diffuse  = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };
    
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    
    //�ؽ�ó ����
    float4 albedo = gAlbedo.Sample(samLinear, input.tex);
    //float4 albedo = input.color;
    
    //tangentspace�� ����� normal�� �����
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //���� �������� �ű������ ���
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