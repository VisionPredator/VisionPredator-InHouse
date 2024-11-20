#pragma once
#include "System.h"
class TrunSystem :public System, public IFixedUpdatable,public IStartable
{
public:
	
	TrunSystem(std::shared_ptr<SceneManager> SceneManager);
	~TrunSystem() = default;
	// IFixedUpdatable��(��) ���� ��ӵ�
	void FixedUpdate(float deltaTime) override;


	// IStartable��(��) ���� ��ӵ�
	void Initialize() override;

	void Start(uint32_t gameObjectId) override;

	void Finish(uint32_t gameObjectId) override;

	void Finalize() override;

};

