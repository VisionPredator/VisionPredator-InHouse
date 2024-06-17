#pragma once
#include <d3d11.h>
#include "CBuffer.h"

namespace BufferDESC
{
	namespace Constant
	{
		static D3D11_BUFFER_DESC DefaultTransform = {
		sizeof(TransformData),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_CONSTANT_BUFFER,
		0,
		0
		};

		static D3D11_BUFFER_DESC DynamicTransform = {
			sizeof(TransformData),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};

		static D3D11_BUFFER_DESC DefaultMatrixPallete = {
			sizeof(MatrixPallete),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0
		};

		static D3D11_BUFFER_DESC DynamicMatrixPallete = {
			sizeof(MatrixPallete),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};

		static D3D11_BUFFER_DESC DefaultLight = {
			sizeof(LightData),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0
		};

		static D3D11_BUFFER_DESC DynamicLight = {
			sizeof(LightData),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};

		static D3D11_BUFFER_DESC DefaultCamera = {
			sizeof(CameraData),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0
		};

		static D3D11_BUFFER_DESC DynamicCamera = {
			sizeof(CameraData),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};


		static D3D11_BUFFER_DESC DefaultLightArray = {
			sizeof(LightArray),
			D3D11_USAGE_DEFAULT,
			D3D11_BIND_CONSTANT_BUFFER,
			0,
			0
		};

		static D3D11_BUFFER_DESC DynamicLightArray = {
			sizeof(LightArray),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};

		static D3D11_BUFFER_DESC DefaultMaterial = {
		sizeof(MaterialData),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_CONSTANT_BUFFER,
		0,
		0
		};

		static D3D11_BUFFER_DESC DynamicMaterial = {
			sizeof(MaterialData),
			D3D11_USAGE_DYNAMIC,
			D3D11_BIND_CONSTANT_BUFFER,
			D3D11_CPU_ACCESS_WRITE,
			0
		};



	}

	namespace Index
	{
		static D3D11_BUFFER_DESC Default =
		{
			0,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};
	}

	namespace Vertex
	{
		static D3D11_BUFFER_DESC Default =
		{
			0,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};
	}
}

namespace RenderStateDESC
{
	//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc

	namespace Solid
	{
		static D3D11_RASTERIZER_DESC Desc = {
			D3D11_FILL_SOLID,
			D3D11_CULL_FRONT,
			true,//시계방향(카메라를 기준으로)으로 감긴 심각형을 전면 으로 간주하고 반시계방향(키메라를 기준으로)으로 감긴 삼각형을 후면으로 간주하	려면 false 를 지정한다
			0,
			0.0f,
			0.0f,
			true,
			false,
			false,
			false,
		};
	}

	namespace Wire
	{
		static D3D11_RASTERIZER_DESC Desc = {
			D3D11_FILL_WIREFRAME,
			D3D11_CULL_BACK,
			true,
			0,
			0.0f,
			0.0f,
			true,
			false,
			false,
			false
		};
	}
}

namespace SamplerDESC
{
	//https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_sampler_desc
	/*
	 D3D11_FILTER               Filter;
	D3D11_TEXTURE_ADDRESS_MODE AddressU;
	D3D11_TEXTURE_ADDRESS_MODE AddressV;
	D3D11_TEXTURE_ADDRESS_MODE AddressW;
	FLOAT                      MipLODBias;
	UINT                       MaxAnisotropy;
	D3D11_COMPARISON_FUNC      ComparisonFunc;
	FLOAT                      BorderColor[4];
	FLOAT                      MinLOD;
	FLOAT                      MaxLOD;
	*/
	static D3D11_SAMPLER_DESC Linear =
	{
		D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		D3D11_TEXTURE_ADDRESS_WRAP,
		0.0f,
		1,
		D3D11_COMPARISON_NEVER,
		(1.0f, 1.0f, 1.0f, 1.0f),
		0,
		D3D11_FLOAT32_MAX,
	};

	static D3D11_SAMPLER_DESC Point =
	{
		D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		D3D11_TEXTURE_ADDRESS_CLAMP,
		0.0f,
		1,
		D3D11_COMPARISON_NEVER,
		{1.0f, 1.0f, 1.0f, 1.0f},
		0,
		D3D11_FLOAT32_MAX
	};

}


namespace TextureDESC
{
	/*UINT Width;
	UINT Height;
	UINT MipLevels;
	UINT ArraySize;
	DXGI_FORMAT Format;
	DXGI_SAMPLE_DESC SampleDesc; {COUNT, QUALITY}
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;*/

	static D3D11_TEXTURE2D_DESC OffScreen = {
		0,
		0,
		1,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,//이게 dsv에서 사용 불가
		{1,0},
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};


	static D3D11_TEXTURE2D_DESC DSVDesc = {
		0,
		0,
		1,
		1,
		DXGI_FORMAT_D24_UNORM_S8_UINT,
	};
}
