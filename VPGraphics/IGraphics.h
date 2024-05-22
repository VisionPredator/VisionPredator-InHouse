#pragma once
#include <windows.h>

#pragma region STL
#include <string>
#include <memory> //smart pointer
#include <vector>
#include <map>
#pragma endregion STL

#include "../include/directxtk/SimpleMath.h"

#include "MeshFilter.h"
#include "CBuffer.h"



/// <summary>
/// ���� �������� ����� �������̽� Ŭ����
/// </summary>
namespace Graphics
{
	class Interface
	{
	public:
		Interface() {};
		virtual ~Interface() {};

		//���� ������
		Interface(Interface& other) = delete;
		//���� ���� ������
		Interface& operator=(Interface& other) = delete;
		//�̵�������
		Interface(Interface&& other) = delete;
		//�̵� ���� ������
		Interface&& operator=(Interface&& other) = delete;


		virtual bool Initialize() abstract;
		virtual void Update(double dt) abstract;
		virtual bool Finalize() abstract;
		virtual void Render() abstract;
		virtual void OnResize() abstract;


		///�߰��ؾ��Ұ�
		//��ƼƼ�� ������� �׷��Ƚ� �ȿ� �ִ� �ش� ������Ʈ�� �����ִ� �Լ�
		//void DeleteObject();

		//ī�޶��� ���� ������Ʈ
		virtual void SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) abstract;
		

		//���߿� �����
		virtual bool AddRenderModel(MeshFilter mesh, std::wstring name, std::wstring fbx = L"") abstract;

	private:

	};
}