#include "pch.h"
#include "D3DUtill.h"

#include "Defines.h"

// [0, 1] ������ �ε��Ҽ��� ������ �����ش�.
float D3DUtill::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// [a, b] ������ �ε��Ҽ��� ������ �����ش�.
float D3DUtill::RandF(float a, float b)
{
	return a + RandF() * (b - a);
}

Vector4 D3DUtill::RandUnitVec3(Vector4 n)
{
	Vector4 One = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 Zero = Vector4::Zero;

	// �ݱ� ����/���� ���� ���� ������ �õ��Ѵ�.
	while (true)
	{
		// �Թ�ü [-1, 1]^3 ���� ������ �� �ϳ��� �����Ѵ�.
		Vector4 v = { RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f };

		// ���� ���� ���� �� ������ ��� ����, ���� �� �ٱ���
		// ���� �����Ѵ�. �̷��� ���� ������ ������ �����ٴ�
		// �Թ�ü �����̵鿡 �� ���� ���� ���̴�.
		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		// ���� �ݱ��� ���κп� ��ġ�ϵ��� �����Ѵ�.
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;

		return XMVector3Normalize(v);
	}
}

void D3DUtill::CreateRandomTexture1DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV)
{
	//
	// ������ �ڷḦ �����Ѵ�.
	//
	const size_t ELEMENT_COUNT = 1024u;
	std::vector<Vector4> randomValues(ELEMENT_COUNT);

	for (size_t i = 0; i < ELEMENT_COUNT; ++i)
	{
		randomValues[i].x = RandF(-1.0f, 1.0f);
		randomValues[i].y = RandF(-1.0f, 1.0f);
		randomValues[i].z = RandF(-1.0f, 1.0f);
		randomValues[i].w = RandF(-1.0f, 1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &randomValues[0];
	initData.SysMemPitch = ELEMENT_COUNT * sizeof(Vector4);
	initData.SysMemSlicePitch = 0;

	//
	// �ؽ�ó�� �����Ѵ�.
	//
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	Microsoft::WRL::ComPtr<ID3D11Texture1D> randomTex;
	HR_CHECK(device->CreateTexture1D(&texDesc, &initData, randomTex.GetAddressOf()));

	//
	// ���̴� ���ҽ� �並 �����Ѵ�.
	//
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	HR_CHECK(device->CreateShaderResourceView(randomTex.Get(), &viewDesc, ppSRV));
}

void D3DUtill::CreateRandomTexture2DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV)
{

}

