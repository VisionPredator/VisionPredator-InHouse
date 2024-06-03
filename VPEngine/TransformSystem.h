#pragma once
#include "System.h"
struct TransformComponent;

class TransformSystem :
	public System, public IUpdatable, public IFixedUpdatable
{
public:
	TransformSystem(SceneManager* entityManager);
	~TransformSystem() override = default;


	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

	void CalulateTransform(TransformComponent* transform);
	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;

};

