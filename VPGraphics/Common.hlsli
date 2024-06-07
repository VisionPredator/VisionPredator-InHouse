//CONSTANT

///Camera
cbuffer Camera : register(b0)
{
    float4x4 gWorldViewProj;
    float4x4 gView;
    float4x4 gProj;
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
Texture2D gDiffuseMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);


//SAMPLER
SamplerState samAnisotropic : register(s0)
{
    //Filter = ANISOTROPIC;
    //MaxAnisotropy = 4;
    //AddressU = WRAP;
    //AddressV = WRAP;
};


SamplerState samLinear : register(s1);


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
