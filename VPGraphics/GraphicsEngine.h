#pragma once
#include "IGraphics.h"

#pragma region STL
#include <memory> //smart pointer

#pragma endregion STL

#include <vector>
#include <map>
#include "MeshFilter.h"

/// <summary>
/// 2024.03.14
/// �׸��� �׷��� Ŭ����
/// �ۼ��� : ���¿�
/// </summary>

struct D3D11_VIEWPORT;

class Device;
class ResourceManager;
class ModelLoader;
class Animator;
class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;

class Object;

class GraphicsEngine : public Graphics::Interface
{
public:
	GraphicsEngine(HWND hWnd);
	~GraphicsEngine();

	virtual bool Initialize() override;
	virtual void Update(double dt) abstract;
	virtual bool Finalize() override;
	virtual void Render() override;
	virtual void OnResize() override;



	void DeferredRender();
	void Update(double dt, DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	virtual bool LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx, std::wstring VSname, std::wstring PSname) override;
	virtual void UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct) override;
	virtual void UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)override;

protected:
	//objects���� cb�� ������Ʈ�ϰ� ���ҽ��Ŵ������� ��� ���ҽ� �� ������Ʈ? -> ���� ���� ������۷� ���� �κ��� �� �и� �Ǿ���ҰŰ�����?
	// MatrixPallete���� ��� ������ ���� ��������
	//�ν��Ͻ̱��� ����ϸ�?
	//���� �� ������۸� �ٸ��� �׸���

	//forward
	//RTV + DSV + objects

	//deferred
	//RTVs + DSVs + objects

	std::vector<RenderTargetView*> m_RTVs;
	std::vector<DepthStencilView*> m_DSVs;

	std::map<std::wstring, Object*> m_DeferredRenderObjects;
	std::map<std::wstring, Object*> m_FowardRenderObjects;


	void DrawQuad(ShaderResourceView* srv);


private:
	Device* m_device;
	D3D11_VIEWPORT* m_VP;

private:
	ResourceManager* m_ResourceManager;
	ModelLoader* m_Loader;
	Animator* m_Animator;

private:
	HWND m_hWnd;
	RECT m_wndSize;
};