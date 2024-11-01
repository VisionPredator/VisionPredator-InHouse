#define PT_EMITTER 0
#define PT_FLARE 1

Texture2DArray gTexArray : register(t0);
Texture1D gRandomTex : register(t1);
SamplerState samLinear : register(s0);

cbuffer cbPerFrame : register(b0)
{
	float4x4 gViewProj;
	float3 gEyePosW;
	float gGameTime;
	float3 gEmitPosW;
	float gTimeStep;
	float3 gEmitDirW;
};

struct Particle
{
	float3 InitialPosW : POSITION;
	float3 InitialVelW : VELOCITY;
	float2 SizeW       : SIZE;
	float Age : AGE;
	uint Type          : TYPE;
};

struct VertexOut
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
	float4 Color : COLOR;
	uint   Type  : TYPE;
};

VertexOut DrawVS(Particle vin)
{
	VertexOut vout;

	// 중력 설정.
	float3 gAccelW = { 0.0f, 0.0f, 0.0f };
	//float3 gAccelW = { 0.0f, 7.8f, 0.0f };
	float2 gQuadTexC[4] =
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};

	float t = vin.Age;
	vout.PosW = 0.5f * t * t * gAccelW + t * vin.InitialVelW + vin.InitialPosW;
	float opacity = 1.0f - smoothstep(0.0f, 1.0f, t / 1.0f);
	vout.Color = float4(1.0f, 1.0f, 1.0f, opacity);
	vout.SizeW = vin.SizeW;
	vout.Type = vin.Type;

	return vout;
}

Particle StreamOutVS(Particle vin)
{
	return vin;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}