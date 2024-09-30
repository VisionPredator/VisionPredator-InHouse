#pragma once
#include "System.h"
class AnimationSystem :
	public System, public EventSubscriber, public IRenderable,public IFixedUpdatable,public IUpdatable
{
public:
	AnimationSystem(std::shared_ptr<SceneManager> sceneManager);
	~AnimationSystem() = default;



	void OnChangeAnimation(std::any aniBlendData);

	// IRenderable��(��) ���� ��ӵ�
	void BeginRenderUpdate(float deltaTime) override;
	// IRenderable��(��) ���� ��ӵ�
	void EditorRenderUpdate(float deltaTime) override;

	// IRenderable��(��) ���� ��ӵ�
	void RenderUpdate(float deltaTime) override;

	void LateRenderUpdate(float deltaTime) override;




	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;


	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

};

