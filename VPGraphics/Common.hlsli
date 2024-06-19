//CONSTANT

//PI
static const float Pi = 3.141592;
static const float Epsilon = 0.00001;

//��� ����ü�� ���� ������ ���� �Ի� ������ ���
static const float3 Fdielectric = { 0.04, 0.04, 0.04 };

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


//Material
cbuffer Material : register(b2)
{
    float4 albedo;
    float4 AMRO; // Ambient Occlusion
    float metalness;
    float roughness;
    float ao; // Ambient Occlusion
    float pad;
};


///Light
struct LightData
{
    float3 Direction;
    float Range;

    float3 Attenuation;
    float pad2;
  
    float3 pos;
    float spot;
    
    //direction ���� �� ���������ҵ� pbr�� lightvector�� lightcolor�� ����ϴϱ�
    float3 Color;
    float pad3;
};




cbuffer LightArray : register(b3)
{
    LightData Dir[100];
    LightData Point[100];
    LightData Spot[100];
    float DirIndex;
    float PointIndex;
    float SpotIndex;
    float pad4;
};



///Skinning
cbuffer MatrixPallete : register(b4)
{
    matrix MatrixPallete[128];
}



//TEXTURE
Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
Texture2D gDepth : register(t3);
Texture2D gMetalic : register(t4);
Texture2D gRoughness : register(t5);
Texture2D gAO : register(t6);

Texture2D gGBuffer : register(t7);

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

//Light Function


float3 FresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float Calc_D(float3 N, float3 H, float roughness)
{
    //DistributionGGX
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159 * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    //GeometrySchlickGGX
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float Calc_G(float3 N, float3 V, float3 L, float roughness)
{
    //GeometrySmith
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}


//�� ���� �ܼ� ������ �ƴ� �����ʿ�
float3 CalcDir(LightData lightData, float3 V, float3 N, float3 F,float3 albedo, float roughness)
{
    float3 result  = float3(0,0,0);
    
    float3 ambient  = float3(0,0,0);
    float3 diffuse  = float3(0,0,0);
    float3 specular = float3(0,0,0);
    
    //ǥ�������� ���������� ���� 
    float3 L = -normalize(lightData.Direction); //directionlight�� ��� ǥ�������� ������ �������� ������ ���̹Ƿ� ���� ������ ������ ����
    
    //Diffuse BRDF
    //kD - diffuse �ݻ���, kS - fresnel �ݻ��� -> ������ ���� ��Ģ�� ���� �����ڷ� �ݻ�Ǵ� ���� ��� ��ü�� ����� ǥ�� �ؿ��� ����� �ݻ�Ǵ� ���� ���� 1
    float3 kD = float3(1.0, 1.0, 1.0) - F; // kS is equal to Fresnel
    // multiply kD by the inverse metalness such that only non-metals have diffuse lighting, or a linear blend if partly metal (pure metals have no diffuse light)
    kD *= 1.0 - metalness;
    
    
    diffuse = kD * albedo / Pi;
    
    //Specular BRDF
    float3 H = normalize(L + V);
    
    float D = Calc_D(N, H, roughness);
    float G = Calc_G(N, V, L, roughness);
    
    float3 n = (F * D * G); //����
    float3 d = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.01; //�и� 0���� ������ ���Ϸ��� + 0.01
    
    specular = n / d;
    
    result += (specular + diffuse) * lightData.Color/*radiance ����-(����)�ֵ�*/ * max(dot(N, L), 0.0);
    
    return result;
}

float3 CalcPoint(LightData lightData, float3 V, float3 N, float3 F, float3 albedo, float roughness)
{
    float3 result  = float3(0,0,0);
        
    float3 ambient = float3(0, 0, 0);
    float3 diffuse = float3(0, 0, 0);
    float3 specular = float3(0, 0, 0);
    
    float3 L = lightData.pos - V;
    
    //������ ǥ���� �Ÿ�
    float distance = length(L);
    
    if (distance > lightData.Range)
        return float3(0, 0, 0);
    
    //�� ���� ����ȭ
    L /= distance;
        
    //Diffuse BRDF
    //kD - diffuse �ݻ���, kS - fresnel �ݻ��� -> ������ ���� ��Ģ�� ���� �����ڷ� �ݻ�Ǵ� ���� ��� ��ü�� ����� ǥ�� �ؿ��� ����� �ݻ�Ǵ� ���� ���� 1
    float3 kD = float3(1.0, 1.0, 1.0) - F; // kS is equal to Fresnel
    // multiply kD by the inverse metalness such that only non-metals have diffuse lighting, or a linear blend if partly metal (pure metals have no diffuse light)
    kD *= 1.0 - metalness;
    
    diffuse = kD * albedo / Pi;
    
    
   //Specular BRDF
    float3 H = normalize(L + V);
    
    float D = Calc_D(N, H, roughness);
    float G = Calc_G(N, V, L, roughness);
    
    float3 n = (F * D * G); //����
    float3 d = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.01; //�и� 0���� ������ ���Ϸ��� + 0.01
    
    specular = n / d;
        
    float att = 1.0f / dot(lightData.Attenuation, float3(1.0f, distance, distance * distance));
       
    diffuse *= att;
    specular *= att;
    
    result += (specular + diffuse) * lightData.Color/*radiance ����-(����)�ֵ�*/ * max(dot(N, L), 0.0);
    
    return result;
    
}

float3 CalcSpot(LightData lightData, float3 V, float3 N, float3 F, float3 albedo, float roughness)
{
    float3 result  = float3(0,0,0);
    
    
    float3 ambient = float3(0, 0, 0);
    float3 diffuse = float3(0, 0, 0);
    float3 specular = float3(0, 0, 0);
    
    //�� ����
    float3 L = lightData.pos - V;
    
    //������ ǥ���� �Ÿ�
    float distance = length(L);
    
    if (distance > lightData.Range)
        return float3(0,0,0);
    
    //�� ���� ����ȭ
    L /= distance;
    
   //Diffuse BRDF
    //kD - diffuse �ݻ���, kS - fresnel �ݻ��� -> ������ ���� ��Ģ�� ���� �����ڷ� �ݻ�Ǵ� ���� ��� ��ü�� ����� ǥ�� �ؿ��� ����� �ݻ�Ǵ� ���� ���� 1
    float3 kD = float3(1.0, 1.0, 1.0) - F; // kS is equal to Fresnel
    // multiply kD by the inverse metalness such that only non-metals have diffuse lighting, or a linear blend if partly metal (pure metals have no diffuse light)
    kD *= 1.0 - metalness;
    
    diffuse = kD * albedo / Pi;
    
 
   //Specular BRDF
    float3 H = normalize(L + V);
    
    float D = Calc_D(N, H, roughness);
    float G = Calc_G(N, V, L, roughness);
    
    float3 n = (F * D * G); //����
    float3 d = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.01; //�и� 0���� ������ ���Ϸ��� + 0.01
    
    specular = n / d;

    float spot = pow(max(dot(-L, lightData.Direction), 0.0f), lightData.spot);

    float att = spot / dot(lightData.Attenuation, float3(1.0f, distance, distance * distance));
    
    diffuse *= att;
    specular *= att;
    
    result += (specular + diffuse) * lightData.Color/*radiance ����-(����)�ֵ�*/ * max(dot(N, L), 0.0);
    return result;
}
