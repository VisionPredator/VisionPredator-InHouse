#pragma once
#include "IGraphics.h"

#pragma region DX
struct D3D11_VIEWPORT;

class Device;
class ResourceManager;
class ModelLoader;
class Animator;
class RenderTargetView;
class DepthStencilView;
class ShaderResourceView;
#pragma endregion DX

#pragma region Pass
class ForwardPass;
class SkinnigPass;
class TexturePass;
#pragma endregion Pass

#include "MeshFilter.h"

class Camera;
class ModelData;




/// <summary>
/// 2024.03.14
/// 그림을 그려줄 클래스
/// 작성자 : 유승운
/// </summary>

class GraphicsEngine : public Graphics::Interface
{
public:
	GraphicsEngine(HWND hWnd);
	~GraphicsEngine();

	virtual bool Initialize() override;
	virtual void Update(double dt) override;
	virtual bool Finalize() override;
	virtual void Render() override;
	virtual void OnResize() override;

	void DeferredRender();
	virtual bool LoadResource(MeshFilter mesh, std::wstring name, std::wstring fbx = L"") override;
	virtual void UpdateCB(std::wstring name, std::wstring cbname, WorldTransformCB constantstruct) override;
	virtual void UpdateCB(std::wstring name, std::wstring cbname, DirectionLightCB constantstruct)override;

	

protected:
	std::vector<std::weak_ptr<RenderTargetView>> m_RTVs;
	std::vector<std::weak_ptr<DepthStencilView>> m_DSVs;

	std::map<std::wstring, std::pair<PassState,std::shared_ptr<ModelData>>> m_RenderList;

	void DrawQuad(ShaderResourceView* srv);

private:
	std::shared_ptr<Device> m_Device;
	D3D11_VIEWPORT* m_VP;

private:
	std::shared_ptr<ResourceManager> m_ResourceManager;
	std::shared_ptr<ModelLoader> m_Loader;
	std::shared_ptr<Animator> m_Animator;

private:
	HWND m_hWnd;
	RECT m_wndSize;

	Camera* m_Camera;

	ForwardPass* m_BasePass;
	TexturePass* m_TexturePass;
	SkinnigPass* m_SkinningPass;

};