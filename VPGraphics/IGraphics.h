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
/// 게임 엔진에서 사용할 인터페이스 클래스
/// </summary>
namespace Graphics
{
	class Interface
	{
	public:
		Interface() {};
		virtual ~Interface() {};

		//복사 생성자
		Interface(Interface& other) = delete;
		//복사 대입 연산자
		Interface& operator=(Interface& other) = delete;
		//이동생성자
		Interface(Interface&& other) = delete;
		//이동 대입 연산자
		Interface&& operator=(Interface&& other) = delete;


		virtual bool Initialize() abstract;
		virtual void Update(double dt) abstract;
		virtual bool Finalize() abstract;
		virtual void Render() abstract;
		virtual void OnResize() abstract;


		///추가해야할거
		//엔티티가 사라지면 그래픽스 안에 있는 해당 오브젝트도 지워주는 함수
		//void DeleteObject();

		//카메라의 상태 업데이트
		virtual void SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) abstract;
		

		//나중에 지울거
		virtual bool AddRenderModel(MeshFilter mesh, std::wstring name, std::wstring fbx = L"") abstract;

	private:

	};
}