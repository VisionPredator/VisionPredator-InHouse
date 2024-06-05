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

cbuffer Light : register(b2)
{
    LightData gDirLight;
    LightData gPointLight;
    LightData gSpotLight;
};


Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samAnisotropic : register(s0)
{
    //Filter = ANISOTROPIC;
    //MaxAnisotropy = 4;
    //AddressU = WRAP;
    //AddressV = WRAP;
};

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
    //float4 boneindex : BONEINDEX;
    //float4 boneweight : BONEWEIGHT;
};

float4 main(PS_INPUT input) : SV_TARGET
{   
    //Direction Light
    /*
    */
    float4 ambient = { 0.1, 0.1, 0.1, 0 };
    float4 diffuse = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };
        
    //����ġ�� FPS�ϱ� ī�޶�� �Ȱ��� - view�� ī�޶� ����� ������̴� viewinverse == camera
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    //ǥ�������� ���������� ���� 
    float3 lightVec = -normalize(gDirLight.Direction); //directionlight�� ��� ǥ�������� ������ �������� ������ ���̹Ƿ� ���� ������ ������ ����
    //float3 lightVec = normalize(gDirLight.Direction)- input.pos;  //point light�� spot light�� ��ġ������ ���� ������ �ٲ�
    
    //���� ����
    ambient = gDirLight.Ambient;
    
    //�ؽ�ó ����
    float4 albedo = gDiffuseMap.Sample(samAnisotropic, input.tex);
    //float4 albedo = input.color;
    
    //tangentspace�� ����� normal�� �����
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormalMap.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //���� �������� �ű������ ���
    vNormal.xyz = mul(NormalTangentSpace, WorldTransform);
    vNormal.xyz = normalize(vNormal.xyz);
    
    
    //N dot L (0~1) normal dot light - ���� ������ 0���� ũ��, specular�� ����� �� �ְ� �ȴ�
    float diffusefactor = saturate(dot(lightVec, vNormal.xyz));
     
    //R dot V (reflect dot view)
    //float3 v = reflect(-lightVec, input.normal.xyz);
    float3 v = reflect(-lightVec, vNormal.xyz);
        
    //������ ���� ���
    float materialCoefficient = 64.f;
    
    //�ŵ������� ���� ������ ���������� ǥ��
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);
        
    diffuse = diffusefactor * gDirLight.Diffuse;
    specular = specularfactor * gDirLight.Specular;
        
    float4 litColor;
    //ambient�� albedo�� ���� �� ������ ������ ����� ���� �ִ�
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
    //����ġ�� FPS�ϱ� ī�޶�� �Ȱ��� - view�� ī�޶� ����� ������̴� viewinverse == camera
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    float4 litColor = float4(0, 0, 0, 0);
    
    //�� ����
    float3 lightVec = gPointLight.pos - input.posWorld;
    
    //������ ǥ���� �Ÿ�
    float d = length(lightVec);
    
    if(d > gPointLight.Range)
        return litColor;
    
    //�� ���� ����ȭ
    lightVec /= d;
    
    float4 ambient = gPointLight.Ambient;
 
     //�ؽ�ó ����
    float4 albedo = gDiffuseMap.Sample(samAnisotropic, input.tex);
    
    //tangentspace�� ����� normal�� �����
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormalMap.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //���� �������� �ű������ ���
    vNormal.xyz = mul(NormalTangentSpace, WorldTransform);
    vNormal.xyz = normalize(vNormal.xyz);
    
    
    //N dot L (0~1) normal dot light - ���� ������ 0���� ũ��, specular�� ����� �� �ְ� �ȴ�
    float diffusefactor = saturate(dot(lightVec, vNormal.xyz));
 
    //������ ���� ���
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
    
    //����ġ�� FPS�ϱ� ī�޶�� �Ȱ��� - view�� ī�޶� ����� ������̴� viewinverse == camera
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    float4 litColor = float4(0, 0, 0, 0);
    
    //�� ����
    float3 lightVec = gSpotLight.pos - input.posWorld;
    
    //������ ǥ���� �Ÿ�
    float d = length(lightVec);
    
    if (d > gSpotLight.Range)
        return litColor;
    
    //�� ���� ����ȭ
    lightVec /= d;
    
    float4 ambient = gSpotLight.Ambient;
 
     //�ؽ�ó ����
    float4 albedo = gDiffuseMap.Sample(samAnisotropic, input.tex);
    
    //tangentspace�� ����� normal�� �����
    float4 vNormal = normalize(input.normal);
    float4 vTangent = normalize(input.tangent);
    float4 vBitangent = normalize(input.bitangent);
        
    float3 NormalTangentSpace = gNormalMap.Sample(samLinear, input.tex).rgb * 2.0f - 1.0f; //-1~1
    float3x3 WorldTransform = float3x3(vTangent.xyz, vBitangent.xyz, vNormal.xyz); //���� �������� �ű������ ���
    vNormal.xyz = mul(NormalTangentSpace, WorldTransform);
    vNormal.xyz = normalize(vNormal.xyz);
    
    
    //N dot L (0~1) normal dot light - ���� ������ 0���� ũ��, specular�� ����� �� �ְ� �ȴ�
    float diffusefactor = saturate(dot(lightVec, vNormal.xyz));
 
    //������ ���� ���
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