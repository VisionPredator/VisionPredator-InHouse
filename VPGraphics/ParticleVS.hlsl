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

// 게임 시간과 직접 전달한 offset을 기반으로 랜덤 벡터를 샘플링해준다.
float3 RandUnitVec3(float offset)
{
	// 게임 시간 더하기 오프셋을 무작위 텍스처 추출을 위한
	// 텍스처 좌표로 사용한다.
	float u = (gGameTime + offset);

	// 벡터 성분들의 범위는 [-1, 1] 이다.
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

	// 단위 구로 투영한다.
	return normalize(v);
}

VertexOut DrawVS(Particle vin)
{
	VertexOut vout;

	float3 gAccelW = { 0.0f, 7.8f, 0.0f };
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

// The stream-out GS is just responsible for emitting 
// new particles and destroying old particles.  The logic
// programed here will generally vary from particle system
// to particle system, as the destroy/spawn rules will be 
// different.
[maxvertexcount(2)]
void StreamOutGS(point Particle gin[1],
	inout PointStream<Particle> ptStream)
{
	gin[0].Age += gTimeStep;

	if (gin[0].Type == PT_EMITTER)
	{
		// time to emit a new particle?
		if (gin[0].Age > 0.005f)
		{
			float3 vRandom = RandUnitVec3(0.0f);
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;

			Particle p;
			p.InitialPosW = gEmitPosW.xyz;
			p.InitialVelW = 4.0f * vRandom;
			p.SizeW = float2(3.0f, 3.0f);
			p.Age = 0.0f;
			p.Type = PT_FLARE;

			ptStream.Append(p);

			// reset the time to emit
			gin[0].Age = 0.0f;
		}

		// always keep emitters
		ptStream.Append(gin[0]);
	}
	else
	{
		// Specify conditions to keep particle; this may vary from system to system.
		if (gin[0].Age <= 1.0f)
			ptStream.Append(gin[0]);
	}
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}