#pragma once
#include "System.h"
#include "EventSubscriber.h"
struct TransformComponent;

class TransformSystem :
	public System, public IUpdatable, public EventSubscriber
{
public:
	TransformSystem(SceneManager* entityManager);
	~TransformSystem() override = default;


	// IUpdatable��(��) ���� ��ӵ�
	void Update(float deltaTime) override;

	void OnSetParentAndChild(std::any data);
	void OnUpdateTransfomData(std::any data);
	void OnRelaseParentAndChild(std::any data);
	void CalulateTransform(TransformComponent* transform);

};

