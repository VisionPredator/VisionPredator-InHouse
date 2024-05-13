#pragma once
#include "System.h"
#include "EventSubscriber.h"
namespace VisPred
{
	class EntityManager;

	class SceneSerializer : public System, public EventSubscriber
	{
	public:
		using System::System;
		SceneSerializer(EntityManager* entityManager);
		~SceneSerializer();
		void Initialize(EntityManager* entityManager);

		/// æ¿¿ª serialize «’¥œ¥Ÿ.
		void OnSerializeScene(std::any data);
		void OnSerializePrefab(std::any data);

		/// æ¿¿ª deserialize «’¥œ¥Ÿ.
		void OnDeSerializeScene(std::any data);
		void OnDeSerializePrefab(std::any data);
	};
}
