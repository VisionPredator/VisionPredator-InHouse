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

	/// æ¿¿ª serialize «’¥œ¥Ÿ.
	void OnSerializeScene(std::any data);

	/// æ¿¿ª deserialize «’¥œ¥Ÿ.
	void OnDeSerializeScene(std::any data);
	friend class SceneManager;
};
