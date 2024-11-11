#pragma once
#include "System.h"
#include "EventSubscriber.h"

class SceneSerializer : public System, public EventSubscriber
{
public:
	using System::System;
	SceneSerializer(std::shared_ptr<SceneManager> entityManager);
	~SceneSerializer();
	void Initialize(std::shared_ptr<SceneManager> entityManager);

	/// ���� serialize �մϴ�.
	void OnSerializeScene(std::any data);

	/// ���� deserialize �մϴ�.
	void OnDeSerializeScene(std::any data);
	friend class SceneManager;
};
