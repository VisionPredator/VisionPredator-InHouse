#pragma once
#include "System.h"
class AnimationSystem :
	public System, public EventSubscriber, public IRenderable,public IFixedUpdatable
{
public:
	AnimationSystem(std::shared_ptr<SceneManager> sceneManager);
	~AnimationSystem() = default;



	void OnChangeAnimation(std::any pairdata_entityid_AniIndex);

	// IRenderable을(를) 통해 상속됨
	void BeginRenderUpdate(float deltaTime) override;
	// IRenderable을(를) 통해 상속됨
	void EditorRenderUpdate(float deltaTime) override;

	// IRenderable을(를) 통해 상속됨
	void RenderUpdate(float deltaTime) override;

	void LateRenderUpdate(float deltaTime) override;




	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

};

