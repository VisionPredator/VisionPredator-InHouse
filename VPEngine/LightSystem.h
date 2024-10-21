#pragma once
#include "System.h"
#include "EventSubscriber.h"

class LightSystem :
	public System, public EventSubscriber,public IRenderable,public ICompAddable
{
public:
	LightSystem(std::shared_ptr<SceneManager> entityManager);
	~LightSystem() = default;



	// IRenderable을(를) 통해 상속됨
	void BeginRenderUpdate(float deltaTime) override;

	// IRenderable을(를) 통해 상속됨
	void EditorRenderUpdate(float deltaTime) override;

	// IRenderable을(를) 통해 상속됨
	void RenderUpdate(float deltaTime) override;
	void LateRenderUpdate(float deltaTime) override;

	// ICompAddable을(를) 통해 상속됨
	void ComponentAdded(Component* comp) override;
	void ComponentReleased(Component* comp) override;
};

