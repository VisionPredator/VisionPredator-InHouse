#pragma once
#include "IGraphics.h"

#pragma region STL
#include <memory> //smart pointer

#include <vector>
#include <map>
#include "MeshFilter.h"
#pragma endregion STL


#include "Camera.h"

/// <summary>
/// 2024.03.14
/// 그림을 그려줄 클래스
/// 작성자 : 유승운
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
class ModelData;


class ForwardPass;
class SkinnigPass;
class TexturePass;



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
	std::vector<RenderTargetView*> m_RTVs;
	std::vector<DepthStencilView*> m_DSVs;

	std::map<std::wstring, std::pair<PassState,ModelData*>> m_RenderList;

	void DrawQuad(ShaderResourceView* srv);

private:
	Device* m_Device;
	D3D11_VIEWPORT* m_VP;

private:
	ResourceManager* m_ResourceManager;
	ModelLoader* m_Loader;
	Animator* m_Animator;

private:
	HWND m_hWnd;
	RECT m_wndSize;

	Camera* m_Camera;



	ForwardPass* m_BasePass;
	TexturePass* m_TexturePass;
	SkinnigPass* m_SkinningPass;

};