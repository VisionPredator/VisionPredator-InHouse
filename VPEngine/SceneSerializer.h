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

	/// æ¿¿ª serialize «’¥œ¥Ÿ.
	void OnSerializeScene(std::any data);

	/// æ¿¿ª deserialize «’¥œ¥Ÿ.
	void OnDeSerializeScene(std::any data);
};
