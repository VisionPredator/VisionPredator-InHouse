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

Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
Texture2D gDepth : register(t3);
Texture2D gTangent : register(t4);


SamplerState samAnisotropic : register(s0);
SamplerState samLinear : register(s1);


//빛 들어갈때 단순 대입이 아닌 연산필요
void CalcDir(LightData light, float3 eyepos, float4 normal, out float4 ambient, out float4 diffuse, out float4 specular)
{
    //표면점에서 광원으로의 벡터 
    float3 lightVec = -normalize(light.Direction); //directionlight는 모든 표면점에서 일정한 방향으로 들어오는 빛이므로 빛의 방향을 역으로 쓰자
    
    //빛의 색상
    ambient = light.Ambient;
    
    
    //N dot L (0~1) normal dot light - 빛이 들어오면 0보다 크다, specular를 계산할 수 있게 된다
    float diffusefactor = saturate(dot(lightVec, normal.xyz));
     
    //R dot V (reflect dot view)
    //float3 v = reflect(-lightVec, input.normal.xyz);
    float3 v = reflect(-lightVec, normal.xyz);
        
    //물질의 광택 계수
    float materialCoefficient = 64.f;
    
    //거듭제곱을 통해 광택을 간접적으로 표현
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);
        
    diffuse = diffusefactor * light.Diffuse;
    specular = specularfactor * light.Specular;
}

void CalcPoint(LightData light,float3 eyepos , float3 posWorld, float4 normal, out float4 ambient, out float4 diffuse, out float4 specular)
{
    float3 lightVec = light.pos - posWorld;
    
    
    //광원과 표면의 거리
    float d = length(lightVec);
    
    if (d > light.Range)
        return;
    
    //빛 벡터 정규화
    lightVec /= d;
    
    ambient = light.Ambient;
 
    
    //N dot L (0~1) normal dot light - 빛이 들어오면 0보다 크다, specular를 계산할 수 있게 된다
    float diffusefactor = saturate(dot(lightVec, normal.xyz));
 
    //물질의 광택 계수
    float materialCoefficient = 64.f;
 
    float3 v = reflect(-lightVec, normal.xyz);
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);

    
    float att = 1.0f / dot(light.Attenuation, float3(1.0f, d, d * d));
   
    
    
    diffuse = diffusefactor * light.Diffuse;
    specular = specularfactor * light.Specular;
    
    diffuse *= att;
    specular *= att;
}

void CalcSpot(LightData light, float3 eyepos, float3 posWorld, float4 normal, out float4 ambient, out float4 diffuse, out float4 specular)
{
    //빛 벡터
    float3 lightVec = light.pos - posWorld;
    
    //광원과 표면의 거리
    float d = length(lightVec);
    
    if (d > light.Range)
        return;
    
    //빛 벡터 정규화
    lightVec /= d;
    
    ambient = light.Ambient;
    
    //N dot L (0~1) normal dot light - 빛이 들어오면 0보다 크다, specular를 계산할 수 있게 된다
    float diffusefactor = saturate(dot(lightVec, normal.xyz));
 
    //물질의 광택 계수
    float materialCoefficient = 64.f;
 
    float3 v = reflect(-lightVec, normal.xyz);
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);

    float spot = pow(max(dot(-lightVec, light.Direction), 0.0f), light.spot);

    float att = spot / dot(light.Attenuation, float3(1.0f, d, d * d));
   
    
    
    diffuse = diffusefactor * light.Diffuse;
    specular = specularfactor * light.Specular;
    
    diffuse *= att;
    specular *= att;
}


struct VS_OUTPUT
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

float4 main(VS_OUTPUT input) : SV_TARGET
{
    float4 ambient = { 0, 0, 0, 0 };
    float4 diffuse = { 0, 0, 0, 0 };
    float4 specular = { 0, 0, 0, 0 };
    
    float3 eyepos = normalize(float3(gViewInverse._41, gViewInverse._42, gViewInverse._43) - input.posWorld.xyz);
    
    
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