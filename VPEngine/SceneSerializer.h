#pragma once
#include "System.h"
#include "EventSubscriber.h"

class SceneSerializer : public System, public EventSubscriber
{
public:
	using System::System;
	SceneSerializer(SceneManager* entityManager);
	~SceneSerializer();
	void Initialize(SceneManager* entityManager);

	/// ���� serialize �մϴ�.
	void OnSerializeScene(std::any data);

	/// ���� deserialize �մϴ�.
	void OnDeSerializeScene(std::any data);
};
