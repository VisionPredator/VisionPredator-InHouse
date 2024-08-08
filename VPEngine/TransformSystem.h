#pragma once
#include "System.h"
#include "EventSubscriber.h"
struct TransformComponent;

class TransformSystem :
	public System, public IUpdatable, public EventSubscriber
{
public:
	TransformSystem(std::shared_ptr<SceneManager> entityManager);
	~TransformSystem() override = default;


	// IUpdatable을(를) 통해 상속됨
	void Update(float deltaTime) override;

	void OnSetParentAndChild(std::any data);
	void OnUpdateTransfomData(std::any data);
	void OnRelaseParentAndChild(std::any data);
	void CalulateTransform(TransformComponent* transform);
	void CalulateTransform_Dynamic(TransformComponent* transform);
	void CalulateTransform_World(TransformComponent* transform);
	void CalulateTransform_Local(TransformComponent* transform);
};

