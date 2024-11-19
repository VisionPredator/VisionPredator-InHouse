#pragma once
#include <System.h>
class LDHButtonSystem :
	public System, public IUpdatable,public IRenderable
{
public:
	LDHButtonSystem(std::shared_ptr<SceneManager> scenemanager);
	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;
	bool isDebugRender = false;


	// IRenderable��(��) ���� ��ӵ�
	void BeginRenderUpdate(float deltaTime) override;

	void RenderUpdate(float deltaTime) override;

	void LateRenderUpdate(float deltaTime) override;

	void EditorRenderUpdate(float deltaTime) override;

};

