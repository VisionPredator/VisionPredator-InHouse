#include"Common.hlsli"



float4 main(VS_OUTPUT input) : SV_TARGET
{
 
    float4 albedo = gAlbedo.Sample(samLinear, input.tex);
    float4 position = gPosition.Sample(samLinear, input.tex);
    float4 normal = gNormal.Sample(samLinear, input.tex);
    
    
    for (int i = 0; i < DirIndex; i++)
    {
	
    }    
	
    return albedo;
}