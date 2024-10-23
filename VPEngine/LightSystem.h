#pragma once
#include "System.h"
#include "EventSubscriber.h"

class LightSystem :
	public System, public EventSubscriber,public IRenderable,public ICompAddable
{
public:
	LightSystem(std::shared_ptr<SceneManager> entityManager);
	~LightSystem() = default;



	// IRenderable��(��) ���� ��ӵ�
	void BeginRenderUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void EditorRenderUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;
	void LateRenderUpdate(float deltaTime) override;

	// ICompAddable��(��) ���� ��ӵ�
	void ComponentAdded(Component* comp) override;
	void ComponentReleased(Component* comp) override;
};

