

cbuffer TexelSize : register(b0)
{
    float2 screensize;
};

//edge detection
Texture2D gNormal : register(t0);
Texture2D gDepth : register(t1);


sampler gLinear : register(s0);

float4 main() : SV_TARGET
{
    
    
    
    
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
}