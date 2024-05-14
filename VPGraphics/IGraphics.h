#pragma once
#include <windows.h>

#include <string>

#include "SimpleMath.h"
#include "MeshFilter.h"
#include "CBuffer.h"

namespace Graphics
{
	class Interface
	{
	public:
		Interface();
		virtual ~Interface();

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


		//나중에 지울거
		virtual bool LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx, std::wstring VSname, std::wstring PSname) abstract;
		virtual void UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct) abstract;
		virtual void UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)abstract;


	private:

	};
}