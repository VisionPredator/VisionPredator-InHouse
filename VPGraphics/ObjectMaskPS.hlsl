struct VertexOut
{
	float4 PosWorld : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(VertexOut input) : SV_TARGET
{
	return input.Color;
}
