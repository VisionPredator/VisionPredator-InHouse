#pragma once
#include "System.h"
class AnimationSystem :
	public System, public EventSubscriber, public IRenderable
{
public:
	AnimationSystem(std::shared_ptr<SceneManager> sceneManager);
	~AnimationSystem() = default;



	void OnChangeAnimation(std::any pairdata_entityid_AniIndex);

	// IRenderable��(��) ���� ��ӵ�
	void BeginRenderUpdate(float deltaTime) override;
	// IRenderable��(��) ���� ��ӵ�
	void EditorRenderUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;

	void LateRenderUpdate(float deltaTime) override;



};

