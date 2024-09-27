#pragma once
#include "System.h"
class SkinnedMeshSystem 
	: public System, public IRenderable, public ICompAddable
{
public:
	SkinnedMeshSystem(std::shared_ptr<SceneManager> sceneManager);


	// ICompAddable을(를) 통해 상속됨
	void ComponentAdded(Component* comp) override;

	void ComponentReleased(Component* comp) override;


	// IRenderable을(를) 통해 상속됨
	void BeginRenderUpdate(float deltaTime) override;

	void RenderUpdate(float deltaTime) override;

	void LateRenderUpdate(float deltaTime) override;

	void EditorRenderUpdate(float deltaTime) override;

};

