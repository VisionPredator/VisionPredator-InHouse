#pragma once
#pragma once
#include "VertexData.h"


namespace MyColor
{
	static DirectX::XMFLOAT4 Red = { 1.0f,0.f,0.f,1.0f };
	static DirectX::XMFLOAT4 Green = { 0.0f,1.f,0.f,1.0f };
	static DirectX::XMFLOAT4 Blue = { 0.0f,0.f,1.f,1.0f };
	static DirectX::XMFLOAT4 Gray = { 0.5f,0.5f,0.5f,1.0f };
}




namespace Axis
{
	namespace Vertex
	{
		static BaseVertex Buffer[] = {
				{DirectX::XMFLOAT4(0.f, 0.f, 0.f,1.0f), { 1.f, 0.f, 0.f, 1.f },{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f}},
				{DirectX::XMFLOAT4(10.f, 0.f, 0.f,1.0f), { 1.f, 0.f, 0.f, 1.f },{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f}},

				{DirectX::XMFLOAT4(0.f, 0.f, 0.f,1.0f), { 0.f, 1.f, 0.f, 1.f },{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f}},
				{DirectX::XMFLOAT4(0.f, 10.f, 0.f,1.0f), { 0.f, 1.f, 0.f, 1.f },{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f}},

				{DirectX::XMFLOAT4(0.f, 0.f, 0.f,1.0f), { 0.f, 0.f, 1.f, 1.f },{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f}},
				{DirectX::XMFLOAT4(0.f, 0.f, 10.f,1.0f), { 0.f, 0.f, 1.f, 1.f },{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f, 0.f,0.0f},{0.f, 0.f}},
		};

		static UINT count = 6;

		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(BaseVertex) * Axis::Vertex::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data =
		{
			Axis::Vertex::Buffer
		};
	}

	namespace Index
	{
		static UINT Buffer[] =
		{
			0,1,
			2,3,
			4,5
		};

		static UINT count = 6;

		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(UINT) * Axis::Index::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data = { Axis::Index::Buffer };
	}

	static D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
}

namespace Box
{
	namespace Vertex
	{

		static BaseVertex Buffer[] = {
				{DirectX::XMFLOAT4(-1.f,-1.f, -1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},
				{DirectX::XMFLOAT4(-1.f, 1.f, -1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},

				{DirectX::XMFLOAT4(1.f, 1.f, -1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},
				{DirectX::XMFLOAT4(1.f,-1.f, -1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},

				{DirectX::XMFLOAT4(-1.f,-1.f, 1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},
				{DirectX::XMFLOAT4(-1.f, 1.f, 1.f,1.0f), {0.5f, 0.5f, 0.5f, 1.f }},

				{DirectX::XMFLOAT4(1.f, 1.f, 1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},
				{DirectX::XMFLOAT4(1.f,-1.f, 1.f,1.0f), { 0.5f, 0.5f, 0.5f, 1.f }},
		};

		static UINT count = 8;

		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(BaseVertex) * Box::Vertex::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data =
		{
			Box::Vertex::Buffer
		};
	}

	namespace Index
	{
		static UINT Buffer[] =
		{
		0,1,2,
		0,2,3,

		4,6,5,
		4,7,6,

		1,4,5,
		0,4,1,

		3,2,6,
		3,6,7,

		1,5,6,
		1,6,2,

		3,4,0,
		7,4,3,
		};

		static UINT count = 36;

		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(UINT) * Box::Index::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data =
		{
			Box::Index::Buffer
		};
	}

	static D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}

namespace TextureBox
{
	namespace Vertex
	{

		static float scale = 1;
		static BaseVertex Buffer[] = {
			//face ������ ���ø�

			//���� 4�� + �Ʒ��� 4�� �ؼ� ����ü�ϱ� 24��
			//�ε����� face�� 3���� 12�� * 3�� 
			//8���� ���� 3���� �븻 ������ �л�
			//�ո�
			{ DirectX::XMFLOAT4(-1.0f * scale, -1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f,-1.0f,0) ,{},{},DirectX::XMFLOAT2(0.0f,1.0f) },//1 //0
			{ DirectX::XMFLOAT4(-1.0f * scale, +1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f,-1.0f,0),{},{},DirectX::XMFLOAT2(0.0f,0.0f) },//2 //1
			{ DirectX::XMFLOAT4(+1.0f * scale, +1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f,-1.0f,0),{},{},DirectX::XMFLOAT2(1.0f,0.0f) },//3 //2
			{ DirectX::XMFLOAT4(+1.0f * scale, -1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f,-1.0f,0),{},{},DirectX::XMFLOAT2(1.0f,1.0f) },//4 //3

			// �޸�
			{ DirectX::XMFLOAT4(-1.0f * scale, -1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f,0) ,{},{},DirectX::XMFLOAT2(1.0f,1.0f) }, //5 //4
			{ DirectX::XMFLOAT4(-1.0f * scale, +1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f,0),{},{},DirectX::XMFLOAT2(1.0f,0.0f) }, //6 //5
			{ DirectX::XMFLOAT4(+1.0f * scale, +1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f,0),{},{},DirectX::XMFLOAT2(0.0f,0.0f) }, //7 //6
			{ DirectX::XMFLOAT4(+1.0f * scale, -1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f,0),{},{},DirectX::XMFLOAT2(0.0f,1.0f) }, //8 //7

			//// ����
			{ DirectX::XMFLOAT4(-1.0f * scale, +1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f,0) ,{},{},DirectX::XMFLOAT2(0.0f,1.0f) }, //2 //8
			{ DirectX::XMFLOAT4(+1.0f * scale, +1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,1.0f) }, //3 //9
			{ DirectX::XMFLOAT4(-1.0f * scale, +1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f,0),{},{},DirectX::XMFLOAT2(0.0f,0.0f) }, //6 //10
			{ DirectX::XMFLOAT4(+1.0f * scale, +1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,0.0f) }, //7 //11

			// �Ʒ���
			{ DirectX::XMFLOAT4(-1.0f * scale, -1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, -1.0f,0.0f,0) ,{},{},DirectX::XMFLOAT2(0.0f,0.0f) }, //1 //12
			{ DirectX::XMFLOAT4(+1.0f * scale, -1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, -1.0f,0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,0.0f) }, //4 //13
			{ DirectX::XMFLOAT4(-1.0f * scale, -1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, -1.0f,0.0f,0),{},{},DirectX::XMFLOAT2(0.0f,1.0f) }, //5 //14
			{ DirectX::XMFLOAT4(+1.0f * scale, -1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(0.0f, -1.0f,0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,1.0f) }, //8 //15

			//���ʸ�
			{ DirectX::XMFLOAT4(-1.0f * scale, -1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(-1.0f, 0.0f,0.0f,0) ,{},{},DirectX::XMFLOAT2(0.0f,0.0f) }, //1 //16
			{ DirectX::XMFLOAT4(-1.0f * scale, +1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(-1.0f, 0.0f,0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,0.0f) }, //2 //17
			{ DirectX::XMFLOAT4(-1.0f * scale, -1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(-1.0f, 0.0f,0.0f,0),{},{},DirectX::XMFLOAT2(0.0f,1.0f) }, //5 //18
			{ DirectX::XMFLOAT4(-1.0f * scale, +1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(-1.0f, 0.0f,0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,1.0f) }, //6 //19

			// �����ʸ�
			{ DirectX::XMFLOAT4(+1.0f * scale, +1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f,0), {},{},DirectX::XMFLOAT2(1.0f,0.0f) }, //3 //20
			{ DirectX::XMFLOAT4(+1.0f * scale, -1.0f * scale, -1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f,0),{},{},DirectX::XMFLOAT2(0.0f,0.0f) }, //4 //21
			{ DirectX::XMFLOAT4(+1.0f * scale, +1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f,0),{},{},DirectX::XMFLOAT2(1.0f,1.0f) }, //7 //22
			{ DirectX::XMFLOAT4(+1.0f * scale, -1.0f * scale, +1.0f * scale,1.0f), MyColor::Red, DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f,0),{},{},DirectX::XMFLOAT2(0.0f,1.0f) }  //8 //234		
		};

		static UINT count = 24; // 8 * 3;


		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(BaseVertex) * TextureBox::Vertex::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data = { TextureBox::Vertex::Buffer };
	}

	namespace Index
	{
		static UINT Buffer[] =
		{
			// front face
			1,2,0,
			2,3,0,

			// back face
			6,5,7,
			5,4,7,

			// left face
			19,17,18,
			17,16,18,

			// right face
			20,22,21,
			22,23,21,

			// top face
			10,11,8,
			11,9,8,

			// bottom face
			12,13,14,
			13,15,14
		};

		static UINT count = 36;

		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(UINT) * TextureBox::Index::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data = { TextureBox::Index::Buffer };
	}

	static D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}


namespace Quad
{
	namespace Vertex
	{
		//z���� 1���ϴ� ������ foward�� �ϴ� ��쿡�� quad(texture)�� ���̰��� ���� ���� �׷����������ִ�

		static QuadVertex Buffer[] = {

		{ DirectX::XMFLOAT4(-1.0f , -1.0f , 1.f , 1.0f),DirectX::XMFLOAT2(0.0f,1.0f) },// ���� �Ʒ�
		{ DirectX::XMFLOAT4(-1.0f ,  1.0f , 1.f , 1.0f),DirectX::XMFLOAT2(0.0f,0.0f) },// ���� ��
		{ DirectX::XMFLOAT4(1.0f  ,  1.0f , 1.f , 1.0f),DirectX::XMFLOAT2(1.0f,0.0f) },// ������ ��
		{ DirectX::XMFLOAT4(1.0f  , -1.0f , 1.f , 1.0f),DirectX::XMFLOAT2(1.0f,1.0f) },// ������ �Ʒ�


		};
		static UINT count = 4;


		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(QuadVertex) * Quad::Vertex::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_VERTEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data = { Quad::Vertex::Buffer };
	}

	namespace Index
	{
		static UINT Buffer[] =
		{
			// front face
			0,1,2,
			0,2,3,
		};

		static UINT count = 6;

		static D3D11_BUFFER_DESC Desc =
		{
			sizeof(UINT) * Quad::Index::count,
			D3D11_USAGE_IMMUTABLE,
			D3D11_BIND_INDEX_BUFFER,
			0,
			0,
			0
		};

		static D3D11_SUBRESOURCE_DATA Data = { Quad::Index::Buffer };
	}

	static D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}



//namespace Sphere
//{
//	const float x = 0.525731f;
//	const float z = 0.850651f;
//
//	namespace Vertex
//	{
//		SphereVertex Buffer[] = {
//				{DirectX::XMFLOAT4(-x, 0.f, z, 1.0f),{MyColor::Gray},{1.f, 0.f, 0.f, 1.f}},
//				{DirectX::XMFLOAT4(x, 0.f, z, 1.0f),{MyColor::Gray},{1.f, 0.f, 0.f, 1.f}},
//
//				{DirectX::XMFLOAT4(-x, 0.f,-z,1),{MyColor::Gray}, { 1.f, 0.f, 0.f, 1.f }},
//				{DirectX::XMFLOAT4(x, 0.f,-z,1),{MyColor::Gray}, { 1.f, 0.f, 0.f, 1.f }},
//
//				{DirectX::XMFLOAT4(0.f, z, x,1),{MyColor::Gray}, { 0.f, 1.f, 0.f, 1.f }},
//				{DirectX::XMFLOAT4(0.f, z,-x,1),{MyColor::Gray}, { 0.f, 1.f, 0.f, 1.f }},
//
//				{DirectX::XMFLOAT4(0.f,-z, x,1),{MyColor::Gray}, { 0.f, 1.f, 0.f, 1.f }},
//				{DirectX::XMFLOAT4(0.f,-z,-x,1),{MyColor::Gray}, { 0.f, 1.f, 0.f, 1.f }},
//
//				{DirectX::XMFLOAT4(z, x, 0.f,1),{MyColor::Gray}, { 0.f, 0.f, 1.f, 1.f }},
//				{DirectX::XMFLOAT4(-z, x,0.f,1),{MyColor::Gray}, { 0.f, 0.f, 1.f, 1.f }},
//
//				{DirectX::XMFLOAT4(z,-x, 0.f ,1),{MyColor::Gray}, { 0.f, 0.f, 1.f, 1.f }},
//				{DirectX::XMFLOAT4(-z,-x, 0.f,1),{MyColor::Gray}, { 0.f, 0.f, 1.f, 1.f }}
//		};
//
//		UINT count = 12;
//	}
//
//	namespace Index
//	{
//		UINT Buffer[] =
//		{
//			/*
//			*/
//			1,4,0,
//			4,9,0,
//			4,5,9,
//			8,5,4,
//			1,8,4,
//			1,10,8,
//			10,3,8,
//			8,3,5,
//			3,2,5,
//			3,7,2,
//			3,10,7,
//			10,6,7,
//			6,11,7,
//			6,0,11,
//			6,1,0,
//			10,1,6,
//			11,0,9,
//			2,11,9,
//			5,2,9,
//			11,2,7
//		};
//
//		UINT count = 60;
//	}
//
//	D3D11_PRIMITIVE_TOPOLOGY PRIMITIVE_TOPOLOGY = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//}
