#pragma once
#include "System.h"
struct TransformComponent;

class TransformSystem :
	public System, public IUpdatable, public IFixedUpdatable
{
public:
	TransformSystem(SceneManager* entityManager);
	~TransformSystem() override = default;


	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override;

	void CalulateTransform(TransformComponent* transform);
	// IFixedUpdatable을(를) 통해 상속됨
	void FixedUpdate(float deltaTime) override;

};

