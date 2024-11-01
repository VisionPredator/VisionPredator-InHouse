#define PT_EMITTER 0
#define PT_FLARE 1

#define SHAPE_CONE 0
#define SHAPE_SPHERE 1
#define SHAPE_BOX 2
#define SHAPE_CIRCLE 3

#define BILLBOARD 0
#define STRETCHED_BILLBOARD 1

cbuffer cbPerFrame : register(b0)
{
	float4x4 gViewProj;
	float3 gEyePosW;
	float gGameTime;
	float gTimeStep;
};

cbuffer cbData : register(b1)
{
	float3 gEmitPosW;
	float3 gEmitDirW;

	float2 gStartSize;
	float gStartSpeed;
	float gDuration;
	int gIsLoop;

	int gRestart;
	float gStartLifetime;

	float gAngle;
	float gRadius;

	unsigned int gParticleShape;
	unsigned int gRenderMode;
}

Texture2DArray gTexArray : register(t0);
Texture1D gRandomTex : register(t1);
SamplerState samLinear : register(s0);

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

struct Particle
{
	float3	InitialPosW		: POSITION;
	float3	InitialVelW		: VELOCITY;
	float2	SizeW			: SIZE;
	float	Age				: AGE;
	uint	Type			: TYPE;
};

// The stream-out GS is just responsible for emitting 
// new particles and destroying old particles.  The logic
// programed here will generally vary from particle system
// to particle system, as the destroy/spawn rules will be 
// different.
[maxvertexcount(2)]
void StreamOutGS(point Particle gin[1],
	inout PointStream<Particle> ptStream)
{
	// 현재 파티클의 나이 업데이트
	gin[0].Age += gTimeStep;

	// emitter 파티클인 경우
	if (gin[0].Type == PT_EMITTER)
	{
		if (gRestart)
		{
			gin[0].Age = 0.0f;        // emitter 나이를 초기화
		}

		// gIsLoop가 false일 때 Duration 동안만 파티클 생성
		if ((gin[0].Age >= gDuration && !gIsLoop))
		{
			// Emitter 파티클을 스트림에 유지하면서 더 이상 새 파티클 생성 안 함
			ptStream.Append(gin[0]);
			// emitter 파티클 나이 초기화
			gin[0].Age = 0.0f;

			return;
		}

		// 새로운 파티클을 생성하여 스트림에 추가
		//if (gin[0].Age > 0.005f)
		if (gin[0].Age < gDuration || gIsLoop)
		{
			Particle p;

			// 3D 공간에서 무작위 방향 벡터 생성
			float3 vRandom = RandUnitVec3(0.0f);
			// x및 y성분을 축소하여 범위를 제한
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;


			// 파티클 모양 설정
			if (gParticleShape == SHAPE_SPHERE)
			{
				p.InitialPosW = gEmitPosW.xyz;
				p.InitialVelW = gStartSpeed * vRandom;
			}
			else if (gParticleShape == SHAPE_CIRCLE)
			{
				// XZ 평면에서 무작위 방향 벡터를 생성 및 정규화
				float2 randomDir = normalize(float2(vRandom.x, vRandom.z)); // 방향 벡터를 정규화
				float radius = gRadius * sqrt(saturate((vRandom.y + 1.0f) * 0.5f)); // 반지름을 랜덤하게 설정, sqrt를 사용하여 균일한 분포

				// 원의 중심에서 반지름까지 랜덤한 위치 계산
				float2 pos = randomDir * radius;

				// 파티클의 초기 위치를 XZ 평면에 설정 (Y축은 고정)
				p.InitialPosW = gEmitPosW + float3(pos.x, 0.0f, pos.y);

				// 속도 벡터를 바깥쪽으로 설정 (Y축 성분은 0)
				p.InitialVelW = normalize(float3(randomDir.x, 0.0f, randomDir.y)) * gStartSpeed;
			}
			else if (gParticleShape == SHAPE_BOX)
			{
				p.InitialPosW = gEmitPosW.xyz;
				p.InitialVelW = gStartSpeed * vRandom;
			}
			else if (gParticleShape == SHAPE_CONE)
			{
				p.InitialPosW = gEmitPosW.xyz;
				p.InitialVelW = gStartSpeed * vRandom;
			}

			p.SizeW = gStartSize;	// 크기 설정
			p.Age = 0.0f;					// 나이 초기화
			p.Type = PT_FLARE;				// 파티클 유형 설정
			ptStream.Append(p);
		}

		// 방출기 입자 하나는 항상 유지한다.
		ptStream.Append(gin[0]);
	}
	else
	{
		// emitter 가 아닌 경우, 파티클의 나이가 Lifetime 보다 작거나 같은 경우에만 현재 스트림에 추가한다.
		if (gin[0].Age <= gStartLifetime)
			ptStream.Append(gin[0]);
	}
}


struct GeoOut
{
	float4 PosH  : SV_Position;
	float4 Color : COLOR;
	float2 Tex   : TEXCOORD;
};

struct VertexOut
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
	float4 Color : COLOR;
	uint   Type  : TYPE;
};

[maxvertexcount(4)]
void DrawGS(point VertexOut gin[1],
	inout TriangleStream<GeoOut> triStream)
{
	float3 gAccelW = { 0.0f, 7.8f, 0.0f };
	float2 gQuadTexC[4] =
	{
		float2(0.0f, 1.0f),
		float2(1.0f, 1.0f),
		float2(0.0f, 0.0f),
		float2(1.0f, 0.0f)
	};

	// do not draw emitter particles.
	if (gin[0].Type != PT_EMITTER)
	{
		//
		// Compute world matrix so that billboard faces the camera.
		//
		float3 look = normalize(gEyePosW.xyz - gin[0].PosW);
		float3 right = normalize(cross(float3(0, 1, 0), look));
		float3 up = cross(look, right);

		//
		// Computer triangle strip vertices (quad) in world space.
		//
		float halfWidth = 0.5f * gin[0].SizeW.x;
		float halfHeight = 0.5f * gin[0].SizeW.y;

		float4 v[4];
		v[0] = float4(gin[0].PosW + halfWidth * right - halfHeight * up, 1.0f);
		v[1] = float4(gin[0].PosW + halfWidth * right + halfHeight * up, 1.0f);
		v[2] = float4(gin[0].PosW - halfWidth * right - halfHeight * up, 1.0f);
		v[3] = float4(gin[0].PosW - halfWidth * right + halfHeight * up, 1.0f);

		//
		// Transform quad vertices to world space and output
		// them as a triangle strip.
		//
		GeoOut gout;
		[unroll]
		for (int i = 0; i < 4; ++i)
		{
			gout.PosH = mul(v[i], gViewProj);
			gout.PosH /= gout.PosH.w;
			gout.Tex = gQuadTexC[i];
			gout.Color = gin[0].Color;
			triStream.Append(gout);
		}
	}
}

[maxvertexcount(3)]
void main(
	triangle float4 input[3] : SV_POSITION, 
	inout TriangleStream< GeoOut > output
)
{
	for (uint i = 0; i < 3; i++)
	{
		GeoOut element;
	}
}

