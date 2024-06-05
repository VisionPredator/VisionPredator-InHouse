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

Texture2D gAlbedo : register(t0);
Texture2D gNormal : register(t1);
Texture2D gPosition : register(t2);
Texture2D gDepth : register(t3);
Texture2D gTangent : register(t4);




SamplerState samAnisotropic : register(s0)
{
    //Filter = ANISOTROPIC;
    //MaxAnisotropy = 4;
    //AddressU = WRAP;
    //AddressV = WRAP;
};

SamplerState samLinear : register(s1);

float4 main() : SV_TARGET
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}