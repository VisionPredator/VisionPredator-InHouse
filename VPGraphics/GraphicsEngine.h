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
/// ���������ϱ� ���� �������� ���� �� Ŭ����
/// �ۼ��� : ���¿�
/// </summary>
class GraphicsEngine :
    public Graphics::Interface
{
public:
    GraphicsEngine(HWND hWnd);
    ~GraphicsEngine();

	//���� ������
	GraphicsEngine(GraphicsEngine& other) = delete;
	//���� ���� ������
	GraphicsEngine& operator=(GraphicsEngine& other) = delete;
	//�̵�������
	GraphicsEngine(GraphicsEngine&& other) = delete;
	//�̵� ���� ������
	GraphicsEngine&& operator=(GraphicsEngine&& other) = delete;



	virtual void Initialize() override;
	virtual void Update(double dt) override;
	virtual void Finalize() override;

	virtual void Render() override;

	virtual void OnResize() override;


private:
	HWND m_hWnd;	//������ �ڵ�
	RECT m_WndSize;	//â�� ũ��

	std::shared_ptr<Device> m_Device;
	std::unique_ptr<ResourceManager> m_Device;
	std::unique_ptr<Animator> m_Device;
};

