#pragma once
#include "System.h"
class DoorSystem :public System, public IFixedUpdatable, public IStartable,public EventSubscriber
{
public :
	DoorSystem(std::shared_ptr<SceneManager> scenemanger);
	~DoorSystem() = default;

	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;


	// IStartable��(��) ���� ��ӵ�
	void Initialize() override;

	void Start(uint32_t gameObjectId) override;

	void Finish(uint32_t gameObjectId) override;

	void Finalize() override;
	void OnChangeDoorUseable(std::any entityid_bool);
	void OnInterected(std::any interective_interector);
};

