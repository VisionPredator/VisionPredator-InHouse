#pragma once
#include <d3d11.h>
#include "CBuffer.h"

namespace 

{
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
		DXGI_FORMAT_R32G32B32A32_FLOAT,//이게 dsv에서 사용 불가
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

	static D3D11_TEXTURE2D_DESC TexArray = {

		0,
		0,
		1,
		1,//size
		DXGI_FORMAT_R8G8B8A8_UNORM,
		{1,0},
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0
	};

}



//inputlayout
namespace Instancing
{
	static const int Count = 18;

	static D3D11_INPUT_ELEMENT_DESC Desc[] =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"BITANGENT",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,64,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,80,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"LIGHTMAPUV",0,DXGI_FORMAT_R32G32_FLOAT,0,88,D3D11_INPUT_PER_VERTEX_DATA,0},

		///Instancing Data
		//Transfrom
		{"WORLD",0,DXGI_FORMAT_R32G32B32A32_FLOAT,1,0,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"WORLD",1,DXGI_FORMAT_R32G32B32A32_FLOAT,1,16,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"WORLD",2,DXGI_FORMAT_R32G32B32A32_FLOAT,1,32,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"WORLD",3,DXGI_FORMAT_R32G32B32A32_FLOAT,1,48,D3D11_INPUT_PER_INSTANCE_DATA,1},
		
		{"WORLDINVERSE",0,DXGI_FORMAT_R32G32B32A32_FLOAT,1,64,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"WORLDINVERSE",1,DXGI_FORMAT_R32G32B32A32_FLOAT,1,80,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"WORLDINVERSE",2,DXGI_FORMAT_R32G32B32A32_FLOAT,1,96,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"WORLDINVERSE",3,DXGI_FORMAT_R32G32B32A32_FLOAT,1,112,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"LIGHTMAPOFFSET",0,DXGI_FORMAT_R32G32_FLOAT,1,128,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"LIGHTMAPTILING",0,DXGI_FORMAT_R32G32_FLOAT,1,136,D3D11_INPUT_PER_INSTANCE_DATA,1},
		{"LIGHTMAPINDEX",0,DXGI_FORMAT_R32G32_FLOAT,1,144,D3D11_INPUT_PER_INSTANCE_DATA,1}
	};

}
