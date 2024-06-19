#include "Common.hlsli"

/// <summary>
///
/// The pixel shader for the geometry pass in the first pass of "Deferred Shading".
///
/// </summary>

// Render to texture.
// Using Multi Render Target.
struct PS_OUTPUT
{
	float4 Albedo : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Position : SV_Target2;
	float4 Depth : SV_Target3;
	float4 Metalic : SV_Target4;
	float4 Roughness : SV_Target5;
	float4 AO : SV_Target6;
};

PS_OUTPUT main(VS_OUTPUT input) : SV_TARGET
{
	PS_OUTPUT output;

	// TODO: If the material is not present, default values should be set.
	output.Albedo = gAlbedo.Sample(samLinear, input.tex);
	output.Normal = gNormal.Sample(samLinear, input.tex);
	output.Position = input.posWorld;
    output.Metalic = gMetalic.Sample(samLinear, input.tex);
    output.Roughness = gRoughness.Sample(samLinear, input.tex);
    output.AO = gAO.Sample(samLinear, input.tex);

	float d = input.pos.z / input.pos.w;
	d *= 10;
	output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);
	return output;
}