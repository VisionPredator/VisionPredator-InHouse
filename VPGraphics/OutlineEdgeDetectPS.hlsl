SamplerState LinearClamp : register(s0);
Texture2D ObjectMask : register(t0);

cbuffer ScreenSize : register(b0)
{
	unsigned int Width;
	unsigned int Height;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

static const float gMask[9] =
{
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1
};	// Laplacian Filter

float4 main(VS_OUTPUT input) : SV_TARGET
{
	float4 color = float4(0, 0, 0, 1);  // �ʱ� ���� (���� ���)
	const float coord[3] = { -1, 0, 1 };

	for (int i = 0; i < 9; i++)
	{
		const float2 offset = float2(coord[i % 3] / Width, coord[i / 3] / Height);
		const float4 sampleColor = ObjectMask.Sample(LinearClamp, input.TexCoord + offset);

		// ���� ��� ����
		color += gMask[i] * sampleColor;
	}

	return color;
}

// ���⼭ �߻��ߴ� ����
/* mask ������ ���� ������ �����ϰ� ����� ���� ���� ������ �����ϰ� ����Ҷ��� ������� �ٸ� ������ �־���.
 *
 * ���� 1
 * "HLSL ���� ������ �ʱ�ȭ ����"
 * HLSL���� ���� ������ �迭�� ������ ��, Ư�� ��� �迭�� �����ϸ� �����Ϸ��� ����ȭ ��������
 * ��� �����͸� �޸𸮿� �����ϴ� ����� ����� �� �ִ�. �̷� ���� ��Ÿ�ӽ� ���� ����ġ ���� ������� ����ǰų�
 * �Ҿ����ϰ� �ʱ�ȭ�� �� �ִ�.
 *
 * ���� 2
 * "���� ������ �޸� ��ġ �� �б� ��� ����"
 * HLSL���� ���� ������ ��� ����(Constant Buffer)�� ����ǰų� �޸��� Ư�� ��ġ�� ������ �� �ִ�.
 * �ݸ鿡, ���� ������ �������Ϳ� �ӽ÷� ����ȴ�. �̷��� ���̷� ���� ���͸� ���꿡���� ���� �ٸ��� ��Ÿ�� �� �ִ�.
 *
 * -> ������۳� ���� ��� ������ ���
 */
