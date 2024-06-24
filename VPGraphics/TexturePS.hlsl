struct LightData
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;

    float3 Direction;
    float Range;

    float3 Attenuation;
    float pad;
  
    float3 pos;
    float spot;
};


cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
};

cbuffer Transform : register(b1)
{
    float4x4 gWorld;
    float4x4 glocal;
}



cbuffer LightArray : register(b2)
{
    LightData Dir[100];
    LightData Point[100];
    LightData Spot[100];
    float DirIndex;
    float PointIndex;
    float SpotIndex;
    float pad;
};

Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samAnisotropic : register(s0);
SamplerState samLinear : register(s1);

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

    float4 ambient = { 0.1, 0.1, 0.1, 0 };
    float4 diffuse = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };
        
    //눈위치는 FPS니까 카메라랑 똑같다 - view는 카메라 행렬의 역행렬이다 viewinverse == camera
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    LightData gDirLight = Dir[0];
    
    
    //표면점에서 광원으로의 벡터 
    float3 lightVec = -normalize(gDirLight.Direction); //directionlight는 모든 표면점에서 일정한 방향으로 들어오는 빛이므로 빛의 방향을 역으로 쓰자
    //float3 lightVec = normalize(gDirLight.Direction)- input.pos;  //point light나 spot light는 위치에따라 빛의 방향이 바뀜
    
    
    //빛의 색상
    ambient = gDirLight.Ambient;
    
    //텍스처 색상
    float4 albedo = gDiffuseMap.Sample(samAnisotropic, input.tex);
    //float4 albedo = input.color;
    
    //tangentspace를 계산해 normal을 만든다
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormalMap.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //면의 공간으로 옮기기위한 행렬
    vNormal.xyz = mul(NormalTangentSpace, WorldTransform);
    vNormal.xyz = normalize(vNormal.xyz);
    
    
    //N dot L (0~1) normal dot light - 빛이 들어오면 0보다 크다, specular를 계산할 수 있게 된다
    float diffusefactor = saturate(dot(lightVec, vNormal.xyz));
     
    //R dot V (reflect dot view)
    //float3 v = reflect(-lightVec, input.normal.xyz);
    float3 v = reflect(-lightVec, vNormal.xyz);
        
    //물질의 광택 계수
    float materialCoefficient = 64.f;
    
    //거듭제곱을 통해 광택을 간접적으로 표현
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);
        
    diffuse = diffusefactor * gDirLight.Diffuse;
    specular = specularfactor * gDirLight.Specular;
        
    float4 litColor;

    //ambient에 albedo를 곱해 더 선명한 색상을 출력해 낼수 있다
    litColor = ambient * albedo + diffuse * albedo + specular;
    
    //litColor = diffuse * specular;
    //litColor = ambient * specular;
    //litColor = ambient * diffuse;
    //litColor = ambient;
    //litColor = diffuse;
    //litColor = specular;
    //litColor = gDiffuseMap.Sample(samAnisotropic, input.tex);

    //litColor = float4(1.f, 0.f, 0.f, 1.f);
    //litColor = input.normal;
    //litColor = input.tangent;
    //litColor = input.bitangent;
    //litColor = vNormal;
    //litColor = albedo;
    
 
    return litColor;
    

}