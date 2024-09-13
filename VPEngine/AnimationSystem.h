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


	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;

	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

	void OnChangeAnimation(std::any pairdata_entityid_AniIndex);
};

