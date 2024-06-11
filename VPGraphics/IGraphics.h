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
		virtual void BeginRender() abstract;
		virtual void Render() abstract;
		virtual void EndRender() abstract;
		virtual void OnResize() abstract;


		//엔티티가 사라지면 그래픽스 안에 있는 해당 오브젝트도 지워주는 함수
		virtual void EraseObject(std::wstring name) abstract;

		///추가해야할거
		//엔티티의 데이터가 업데이트 되면 그값을 renderlist의 값에 갱신 시켜줘야하는 함수 근데 이게 맞아? 매번 엔티티마다 이함수를 부를거야? 그건 좀..
		virtual void UpdateModelTransform(std::wstring name,DirectX::SimpleMath::Matrix world) abstract;

		//애니메이션 변경
		//virtual void ChangeAnimation(std::wstring name, animationfilter filter) abstract;
		
		//카메라의 상태 업데이트
		virtual void SetCamera(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj) abstract;

		//렌더링
		virtual bool AddRenderModel(MeshFilter mesh, std::wstring name, std::wstring fbx = L"") abstract;

		//빛 추가
		virtual void AddLight(std::wstring name ,Kind_of_Light kind, LightData data) abstract;

		//빛 제거
		virtual void EraseLight(std::wstring name ,Kind_of_Light kind) abstract;


		//빛 정보 업데이트
		virtual void UpdateLightData(std::wstring name, Kind_of_Light kind, LightData data) abstract;

	private:

	};

}