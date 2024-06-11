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
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void EndRender() abstract;
		virtual void OnResize() abstract;


		//��ƼƼ�� ������� �׷��Ƚ� �ȿ� �ִ� �ش� ������Ʈ�� �����ִ� �Լ�
		virtual void EraseObject(std::wstring name) abstract;

		///�߰��ؾ��Ұ�
		//��ƼƼ�� �����Ͱ� ������Ʈ �Ǹ� �װ��� renderlist�� ���� ���� ��������ϴ� �Լ� �ٵ� �̰� �¾�? �Ź� ��ƼƼ���� ���Լ��� �θ��ž�? �װ� ��..
		virtual void UpdateModelTransform(std::wstring name,DirectX::SimpleMath::Matrix world) abstract;

		//�ִϸ��̼� ����
		//virtual void ChangeAnimation(std::wstring name, animationfilter filter) abstract;
		
		//ī�޶��� ���� ������Ʈ
		virtual void SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) abstract;

		//������
		virtual bool AddRenderModel(MeshFilter mesh, std::wstring name, std::wstring fbx = L"") abstract;

		//�� �߰�
		virtual void AddLight(std::wstring name ,Kind_of_Light kind, LightData data) abstract;

		//�� ����
		virtual void EraseLight(std::wstring name ,Kind_of_Light kind) abstract;


		//�� ���� ������Ʈ
		virtual void UpdateLightData(std::wstring name, Kind_of_Light kind, LightData data) abstract;

	private:

	};

}