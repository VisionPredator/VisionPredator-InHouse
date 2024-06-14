#include"Common.hlsli"

// Using Texture from G-Buffer
Texture2D gAlbedoMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gPositionMap : register(t2);
Texture2D gDepthMap : register(t3);

SamplerState gPointSample : register(s0);


struct SURFACE_DATA
{
	float LinearDepth;
	float3 Color;   // base color
	float3 Normal;
	float SpecPow;
	float SpecIntensity;
};


// Unpack함수 일단 보류.
SURFACE_DATA UnpackGBuffer(float2 uv)
{
	SURFACE_DATA output = {};

    float depth = gDepthMap.Sample(gPointSample, uv.xy).x;
    output.LinearDepth = depth;
    output.Color = gAlbedoMap.Sample(gPointSample, uv);
	
}


float4 main(VS_OUTPUT input) : SV_TARGET
{
	float3 albedo = gAlbedoMap.Sample(samLinear, input.tex).xyz;
    float3 normal = gNormalMap.Sample(samLinear, input.tex).xyz;
    float3 worldPos = gPositionMap.Sample(samLinear, input.tex).xyz;

    float3 finalColor = 0.f;
    float3 ambientColor = 0.f;

	Material material;
	

    // Calculate Directional Light 
    for (int i = 0; i < DirIndex; i++)
    {
		
    }    

    // Calculate Point Light


    // Calculate Spot Light


    return float4(1.f, 1.f ,1.f ,1.f);
}