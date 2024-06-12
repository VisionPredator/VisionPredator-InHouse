/// <summary>
///
/// The pixel shader for the geometry pass in the first pass of "Deferred Shading".
///
/// </summary>

// Mesh's Material resources
Texture2D gAlbedoMap : register(t0);
Texture2D gNormalMap : register(t1);
Texture2D gSpecularMap : register(t2);

SamplerState samLinear : register(s0);

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 PosWorld : WORLDPOSITION;
	float4 Color : COLOR;	// TODO: Delete
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT;
	float4 BiTangent : BITANGENT;  // TODO: Delete. This can be calculated from Normal and Tangent
	float2 UV : TEXCOORD;
};

// Render to texture.
// Using Multi Render Target.
struct PS_OUTPUT
{
	float4 Albedo : SV_Target0;
	float4 Normal : SV_Target1;
	float4 Position : SV_Target2;
	float4 Depth : SV_Target3;
};

PS_OUTPUT main(PS_INPUT input) : SV_TARGET
{
	PS_OUTPUT output;

	// TODO: If the material is not present, default values should be set.
	output.Albedo = gAlbedoMap.Sample(samLinear, input.UV);
	output.Position = input.PosWorld;
	output.Normal = gNormalMap.Sample(samLinear, input.UV);

	float d = input.Pos.z / input.Pos.w;
	d *= 10;
	output.Depth = float4(1 - d, 1 - d, 1 - d, 1.0f);

	return output;
}