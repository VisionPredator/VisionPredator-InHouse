#include "pch.h"
#include "D3DUtill.h"

#include <DDSTextureLoader.h>

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

void D3DUtill::CreateTexture2DArraySRV(ID3D11Device* device, ID3D11DeviceContext* context,
	ID3D11ShaderResourceView** ppSRV, const std::vector<std::wstring>& filename)
{
	assert(device != nullptr);
	assert(context != nullptr);
	assert(filename.size() != 0);

	const UINT SIZE = filename.size();
	std::vector<ID3D11Texture2D*> srcTex(SIZE);

	for (UINT i = 0; i < SIZE; ++i)
	{
		DirectX::CreateDDSTextureFromFileEx(device,
			filename[i].c_str(),
			1024 * 1024,
			D3D11_USAGE_STAGING,
			0,
			D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ,
			0,
			DirectX::DDS_LOADER_FORCE_SRGB,
			(ID3D11Resource**)&srcTex[i],
			nullptr);
	}

	D3D11_TEXTURE2D_DESC texElementDesc;
	srcTex[0]->GetDesc(&texElementDesc);

	D3D11_TEXTURE2D_DESC texArrayDesc;
	texArrayDesc.Width = texElementDesc.Width;
	texArrayDesc.Height = texElementDesc.Height;
	texArrayDesc.MipLevels = texElementDesc.MipLevels;
	texArrayDesc.ArraySize = SIZE;
	texArrayDesc.Format = texElementDesc.Format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;

	ID3D11Texture2D* texArray = 0;
	HR_CHECK(device->CreateTexture2D(&texArrayDesc, 0, &texArray));

	// 각 텍스처의 모든 밉맵 레벨을 순회한다. 밉맵은 텍스처의 다양한 해상도를 의미한다.
	for (UINT texElement = 0; texElement < SIZE; ++texElement)
	{
		for (UINT mipLevel = 0; mipLevel < texElementDesc.MipLevels; ++mipLevel)
		{
			D3D11_MAPPED_SUBRESOURCE mappedTex2D;
			// 텍스처 데이터를 CPU가 접근할 수 있도록 매핑한다. 매핑된 데이터는 mappedTex2D 구조체를 통해 접근한다.
			HR_CHECK(context->Map(srcTex[texElement], mipLevel, D3D11_MAP_READ, 0, &mappedTex2D));

			// 매핑된 텍스처 이미지를 텍스처 배열의 적절한 위치에 복사한다.
			// D3D11CalcSubresource: 밉맵 레벨과 텍스처 요소 인덱스를 조합하여 올바른 서브리소스를 계산한다.
			context->UpdateSubresource(texArray,
				D3D11CalcSubresource(mipLevel, texElement, texElementDesc.MipLevels),
				0,
				mappedTex2D.pData,
				mappedTex2D.RowPitch,
				mappedTex2D.DepthPitch);

			// 매핑된 텍스처 데이터를 GPU가 다시 접근할 수 있도록 텍스처 데이터를 언매핑한다.
			context->Unmap(srcTex[texElement], mipLevel);
		}
	}

	// 앞서 생성한 텍스처 배열을 쉐이더에서 접근할수 있게 하는 리소스 뷰를 생성한다.
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texArrayDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	viewDesc.Texture2DArray.MostDetailedMip = 0;
	viewDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	viewDesc.Texture2DArray.FirstArraySlice = 0;
	viewDesc.Texture2DArray.ArraySize = SIZE;

	HR_CHECK(device->CreateShaderResourceView(texArray, &viewDesc, ppSRV));

	SAFE_RELEASE_COM(texArray);

	for (UINT i = 0; i < SIZE; ++i)
	{
		SAFE_RELEASE_COM(srcTex[i]);
	}

}
