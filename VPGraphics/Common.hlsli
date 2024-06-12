//CONSTANT

///Camera
cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
    float4x4 gViewInverse;
};

///Transform
cbuffer Transform : register(b1)
{
    float4x4 gWorld;
    float4x4 gLocal;
};

///Skinning
cbuffer MatrixPallete : register(b3)
{
    matrix MatrixPallete[128];
}

///Light
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


cbuffer LightArray : register(b4)
{
    LightData Dir[100];
    LightData Point[100];
    LightData Spot[100];
    float DirIndex;
    float PointIndex;
    float SpotIndex;
    float pad;
};

//TEXTURE
Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
Texture2D gDepth : register(t3);
Texture2D gTangent : register(t4);


Texture2D gMeshAlbedo : register(t5);
Texture2D gMeshNormal : register(t6);




SamplerState samLinear : register(s0);


//STRUCT

struct VS_INPUT
{
    float4 pos : POSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
 #ifdef SKINNING
    float4 boneindex : BONEINDEX;
    float4 boneweight : BONEWEIGHT;
 #endif
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float4 posWorld : WORLDPOSITION;
    float4 color : COLOR;
    float4 normal : NORMAL;
    float4 tangent : TANGENT;
    float4 bitangent : BITANGENT;
    float2 tex : TEXCOORD;
};

struct Quad
{
    float4 pos : POSITION;
    float2 tex : TEXCOORD;
};


struct PS_OUTPUT
{
    float4 Albedo : SV_Target0;
    float4 Normal : SV_Target1;
    float4 Position : SV_Target2;
    float4 Depth : SV_Target3;
    //float4 Tangent : SV_Target4;
};



//Light Function

//�� ���� �ܼ� ������ �ƴ� �����ʿ�
void CalcDir(LightData light, float3 eyepos, float4 normal, out float4 ambient, out float4 diffuse, out float4 specular)
{
    //ǥ�������� ���������� ���� 
    float3 lightVec = -normalize(light.Direction); //directionlight�� ��� ǥ�������� ������ �������� ������ ���̹Ƿ� ���� ������ ������ ����
    
    //���� ����
    ambient = light.Ambient;
    
    
    //N dot L (0~1) normal dot light - ���� ������ 0���� ũ��, specular�� ����� �� �ְ� �ȴ�
    float diffusefactor = saturate(dot(lightVec, normal.xyz));
     
    //R dot V (reflect dot view)
    //float3 v = reflect(-lightVec, input.normal.xyz);
    float3 v = reflect(-lightVec, normal.xyz);
        
    //������ ���� ���
    float materialCoefficient = 64.f;
    
    //�ŵ������� ���� ������ ���������� ǥ��
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);
        
    diffuse = (diffusefactor * light.Diffuse);
    specular = (specularfactor * light.Specular);
}

void CalcPoint(LightData light, float3 eyepos, float3 posWorld, float4 normal, out float4 ambient, out float4 diffuse, out float4 specular)
{
    float3 lightVec = light.pos - posWorld;
    
    
    //������ ǥ���� �Ÿ�
    float d = length(lightVec);
    
    if (d > light.Range)
        return;
    
    //�� ���� ����ȭ
    lightVec /= d;
    
    ambient = light.Ambient;
 
    
    //N dot L (0~1) normal dot light - ���� ������ 0���� ũ��, specular�� ����� �� �ְ� �ȴ�
    float diffusefactor = saturate(dot(lightVec, normal.xyz));
 
    //������ ���� ���
    float materialCoefficient = 64.f;
 
    float3 v = reflect(-lightVec, normal.xyz);
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);

    
    float att = 1.0f / dot(light.Attenuation, float3(1.0f, d, d * d));
   
    
    
    diffuse = (diffusefactor * light.Diffuse);
    specular = (specularfactor * light.Specular);
    
    diffuse *= att;
    specular *= att;
}

void CalcSpot(LightData light, float3 eyepos, float3 posWorld, float4 normal, out float4 ambient, out float4 diffuse, out float4 specular)
{
    //�� ����
    float3 lightVec = light.pos - posWorld;
    
    //������ ǥ���� �Ÿ�
    float d = length(lightVec);
    
    if (d > light.Range)
        return;
    
    //�� ���� ����ȭ
    lightVec /= d;
    
    ambient = light.Ambient;
    
    //N dot L (0~1) normal dot light - ���� ������ 0���� ũ��, specular�� ����� �� �ְ� �ȴ�
    float diffusefactor = saturate(dot(lightVec, normal.xyz));
 
    //������ ���� ���
    float materialCoefficient = 64.f;
 
    float3 v = reflect(-lightVec, normal.xyz);
    float specularfactor = pow(max(dot(v, eyepos), 0.0f), materialCoefficient);

    float spot = pow(max(dot(-lightVec, light.Direction), 0.0f), light.spot);

    float att = spot / dot(light.Attenuation, float3(1.0f, d, d * d));
   
    
    
    diffuse = (diffusefactor * light.Diffuse);
    specular = (specularfactor * light.Specular);
    
    diffuse *= att;
    specular *= att;
}

