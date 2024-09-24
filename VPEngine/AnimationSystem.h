#pragma once
#include "System.h"
class AnimationSystem :
	public System, public EventSubscriber, public IRenderable,public IUpdatable
{
public:
	AnimationSystem(std::shared_ptr<SceneManager> sceneManager);
	~AnimationSystem() = default;

	void OnAddedComponent(std::any data);
	void OnReleasedComponent(std::any data);




	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override;

	void OnChangeAnimation(std::any pairdata_entityid_AniIndex);

	// IRenderable을(를) 통해 상속됨
	void RenderUpdate(float deltaTime) override;
	// IRenderable을(를) 통해 상속됨
	void EditorRenderUpdate(float deltaTime) override;




};

