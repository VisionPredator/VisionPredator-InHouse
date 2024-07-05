#include "pch.h"
#include "D3DUtill.h"

#include "Defines.h"

// [0, 1] 구간의 부동소수점 난수를 돌려준다.
float D3DUtill::RandF()
{
	return (float)(rand()) / (float)RAND_MAX;
}

// [a, b] 구간의 부동소수점 난수를 돌려준다.
float D3DUtill::RandF(float a, float b)
{
	return a + RandF() * (b - a);
}

Vector4 D3DUtill::RandUnitVec3(Vector4 n)
{
	Vector4 One = { 1.0f, 1.0f, 1.0f, 1.0f };
	Vector4 Zero = Vector4::Zero;

	// 반구 위의/안의 점이 나올 때까지 시도한다.
	while (true)
	{
		// 입방체 [-1, 1]^3 안의 무작위 점 하나를 생성한다.
		Vector4 v = { RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), RandF(-1.0f, 1.0f), 0.0f };

		// 단위 구에 대한 고른 분포를 얻기 위해, 단위 구 바깥의
		// 점은 무시한다. 이렇게 하지 않으면 점들이 구보다는
		// 입방체 모퉁이들에 더 많이 모일 것이다.
		if (XMVector3Greater(XMVector3LengthSq(v), One))
			continue;

		// 점이 반구의 윗부분에 위치하도록 보장한다.
		if (XMVector3Less(XMVector3Dot(n, v), Zero))
			continue;

		return XMVector3Normalize(v);
	}
}

void D3DUtill::CreateRandomTexture1DSRV(ID3D11Device* device, ID3D11ShaderResourceView** ppSRV)
{
	//
	// 무작위 자료를 생성한다.
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
	// 텍스처를 생성한다.
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
	// 쉐이더 리소스 뷰를 생성한다.
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

