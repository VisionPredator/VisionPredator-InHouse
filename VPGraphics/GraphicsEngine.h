#pragma once
#include "IGraphics.h"

#pragma region STL
#include <memory> //smart pointer

#pragma endregion STL

class Device;
class ResourceManager;
class Animator;
class ModelLoader;

/// <summary>
/// 2024.05.13
/// 렌더링을하기 위해 전반적인 일을 할 클래스
/// 작성자 : 유승운
/// </summary>
class GraphicsEngine :
    public Graphics::Interface
{
public:
    GraphicsEngine(HWND hWnd);
    ~GraphicsEngine();

	//복사 생성자
	GraphicsEngine(GraphicsEngine& other) = delete;
	//복사 대입 연산자
	GraphicsEngine& operator=(GraphicsEngine& other) = delete;
	//이동생성자
	GraphicsEngine(GraphicsEngine&& other) = delete;
	//이동 대입 연산자
	GraphicsEngine&& operator=(GraphicsEngine&& other) = delete;



	virtual void Initialize() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

	virtual void Render() override;

	virtual void OnResize() override;


private:
	HWND m_hWnd;	//윈도우 핸들
	RECT m_WndSize;	//창의 크기

	std::shared_ptr<Device> m_Device;
	std::unique_ptr<ResourceManager> m_Device;
	std::unique_ptr<Animator> m_Device;
};

