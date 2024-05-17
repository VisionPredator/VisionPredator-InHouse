#pragma once
#include <windows.h>
#include <string>

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
		//virtual void SetCamera(DirectX::XMFLOAT3 pos, float Xrotate, float Yrotate) abstract;
		



		//���߿� �����
		virtual bool LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx, std::wstring VSname, std::wstring PSname) abstract;
		virtual void UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct) abstract;
		virtual void UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)abstract;


	private:

	};
}