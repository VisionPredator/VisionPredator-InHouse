#include "Common.hlsli"


float4 main(VS_OUTPUT input) : SV_TARGET
{   
    //Direction Light
    /*
    */
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
    float4 albedo = gAlbedo.Sample(samAnisotropic, input.tex);
    //float4 albedo = input.color;
    
    //tangentspace를 계산해 normal을 만든다
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormal.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
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
    
    
    //Point Light
    /*
    //눈위치는 FPS니까 카메라랑 똑같다 - view는 카메라 행렬의 역행렬이다 viewinverse == camera
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    float4 litColor = float4(0, 0, 0, 0);
    
    //빛 벡터
    float3 lightVec = gPointLight.pos - input.posWorld;
    
    //광원과 표면의 거리
    float d = length(lightVec);
    
    if(d > gPointLight.Range)
        return litColor;
    
    //빛 벡터 정규화
    lightVec /= d;
    
    float4 ambient = gPointLight.Ambient;
 
     //텍스처 색상
    float4 albedo = gDiffuseMap.Sample(samAnisotropic, input.tex);
    
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
 
    //물질의 광택 계수
    float materialCoefficient = 64.f;
 
    float3 v = reflect(-lightVec, vNormal.xyz);
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);

    
    float att = 1.0f / dot(gPointLight.Attenuation, float3(1.0f, d, d * d));
   
    
    
    float4 diffuse = diffusefactor * gPointLight.Diffuse;
    float4 specular = specularfactor * gPointLight.Specular;
    
    diffuse *= att;
    specular *= att;
    
    //litColor = ambient * albedo + diffuse * albedo;
    litColor = diffuse;
    
    return litColor;
    */
    
    /*
    
    //Spot Light
    
    //눈위치는 FPS니까 카메라랑 똑같다 - view는 카메라 행렬의 역행렬이다 viewinverse == camera
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    float4 litColor = float4(0, 0, 0, 0);
    
    //빛 벡터
    float3 lightVec = gSpotLight.pos - input.posWorld;
    
    //광원과 표면의 거리
    float d = length(lightVec);
    
    if (d > gSpotLight.Range)
        return litColor;
    
    //빛 벡터 정규화
    lightVec /= d;
    
    float4 ambient = gSpotLight.Ambient;
 
     //텍스처 색상
    float4 albedo = gDiffuseMap.Sample(samAnisotropic, input.tex);
    
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
 
    //물질의 광택 계수
    float materialCoefficient = 64.f;
 
    float3 v = reflect(-lightVec, vNormal.xyz);
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);

    float spot = pow(max(dot(-lightVec, gSpotLight.Direction), 0.0f), gSpotLight.spot);

    float att = spot / dot(gSpotLight.Attenuation, float3(1.0f, d, d * d));
   
    
    
    float4 diffuse = diffusefactor * gSpotLight.Diffuse;
    float4 specular = specularfactor * gSpotLight.Specular;
    
    diffuse *= att;
    specular *= att;
    
    litColor = ambient * albedo + diffuse * albedo;
    //litColor = diffuse;
    
    return litColor;
    */
}