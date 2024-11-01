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

// ���� �ð��� ���� ������ offset�� ������� ���� ���͸� ���ø����ش�.
float3 RandUnitVec3(float offset)
{
	// ���� �ð� ���ϱ� �������� ������ �ؽ�ó ������ ����
	// �ؽ�ó ��ǥ�� ����Ѵ�.
	float u = (gGameTime + offset);

	// ���� ���е��� ������ [-1, 1] �̴�.
	float3 v = gRandomTex.SampleLevel(samLinear, u, 0).xyz;

	// ���� ���� �����Ѵ�.
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
	// ���� ��ƼŬ�� ���� ������Ʈ
	gin[0].Age += gTimeStep;

	// emitter ��ƼŬ�� ���
	if (gin[0].Type == PT_EMITTER)
	{
		if (gRestart)
		{
			gin[0].Age = 0.0f;        // emitter ���̸� �ʱ�ȭ
		}

		// gIsLoop�� false�� �� Duration ���ȸ� ��ƼŬ ����
		if ((gin[0].Age >= gDuration && !gIsLoop))
		{
			// Emitter ��ƼŬ�� ��Ʈ���� �����ϸ鼭 �� �̻� �� ��ƼŬ ���� �� ��
			ptStream.Append(gin[0]);
			// emitter ��ƼŬ ���� �ʱ�ȭ
			gin[0].Age = 0.0f;

			return;
		}

		// ���ο� ��ƼŬ�� �����Ͽ� ��Ʈ���� �߰�
		//if (gin[0].Age > 0.005f)
		if (gin[0].Age < gDuration || gIsLoop)
		{
			Particle p;

			// 3D �������� ������ ���� ���� ����
			float3 vRandom = RandUnitVec3(0.0f);
			// x�� y������ ����Ͽ� ������ ����
			vRandom.x *= 0.5f;
			vRandom.z *= 0.5f;


			// ��ƼŬ ��� ����
			if (gParticleShape == SHAPE_SPHERE)
			{
				p.InitialPosW = gEmitPosW.xyz;
				p.InitialVelW = gStartSpeed * vRandom;
			}
			else if (gParticleShape == SHAPE_CIRCLE)
			{
				// XZ ��鿡�� ������ ���� ���͸� ���� �� ����ȭ
				float2 randomDir = normalize(float2(vRandom.x, vRandom.z)); // ���� ���͸� ����ȭ
				float radius = gRadius * sqrt(saturate((vRandom.y + 1.0f) * 0.5f)); // �������� �����ϰ� ����, sqrt�� ����Ͽ� ������ ����

				// ���� �߽ɿ��� ���������� ������ ��ġ ���
				float2 pos = randomDir * radius;

				// ��ƼŬ�� �ʱ� ��ġ�� XZ ��鿡 ���� (Y���� ����)
				p.InitialPosW = gEmitPosW + float3(pos.x, 0.0f, pos.y);

				// �ӵ� ���͸� �ٱ������� ���� (Y�� ������ 0)
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

			p.SizeW = gStartSize;	// ũ�� ����
			p.Age = 0.0f;					// ���� �ʱ�ȭ
			p.Type = PT_FLARE;				// ��ƼŬ ���� ����
			ptStream.Append(p);
		}

		// ����� ���� �ϳ��� �׻� �����Ѵ�.
		ptStream.Append(gin[0]);
	}
	else
	{
		// emitter �� �ƴ� ���, ��ƼŬ�� ���̰� Lifetime ���� �۰ų� ���� ��쿡�� ���� ��Ʈ���� �߰��Ѵ�.
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

