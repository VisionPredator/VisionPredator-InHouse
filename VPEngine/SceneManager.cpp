#include "pch.h"
#include "SceneManager.h"
#include "EntityManager.h"
#include "Components.h"

SceneManager::SceneManager()
{
	EventManager::GetInstance().Subscribe("OnChangeScene", CreateSubscriber(&SceneManager::OnChangeScene),EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnEndScene", CreateSubscriber(&SceneManager::OnEndScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnResetScene", CreateSubscriber(&SceneManager::OnResetScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnOpenScene", CreateSubscriber(&SceneManager::OnOpenScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnStartScene", CreateSubscriber(&SceneManager::OnStartScene), EventType::SCENE);
	EventManager::GetInstance().Subscribe("OnDestroyEntity", CreateSubscriber(&SceneManager::OnDestroyEntity), EventType::ADD_DELETE);
	EventManager::GetInstance().Subscribe("OnClearEntity", CreateSubscriber(&SceneManager::OnClearEntity), EventType::ADD_DELETE);

}
SceneManager::~SceneManager()
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
}
void SceneManager::Initialize()
{
	///�� ����
	m_CurrentScene = new Scene;
	m_CurrentScene->SceneName = "TempScene";
	EventManager::GetInstance().ScheduleEvent("OnStartScene");
}
void SceneManager::OnDestroyEntity(std::any data)
{
	// EntityMap���� �ش� Entity�� ã���ϴ�.
	uint32_t entityID = std::any_cast <int>(data);

	auto& entityMap = GetEntityMap();

	auto entityIter = entityMap.find(entityID);
	if (entityIter != entityMap.end())
	{
		Entity* entityToRemove = entityIter->second;
		// m_ComponentPool���� �ش� Entity�� �ش��ϴ� ������Ʈ���� �����մϴ�.
		for (auto& compPair :GetScene().m_ComponentPool)
		{
			auto& components = compPair.second;
			// �ش� Entity�� ������Ʈ�� ã�� �����մϴ�.
			auto it = std::remove_if(components.begin(), components.end(),
				[entityID](Component* comp) { return comp->GetEntityID() == entityID; });
			components.erase(it, components.end());
			// ���ŵ� ������Ʈ���� �����մϴ�.
			for (auto eraseIt = it; eraseIt != components.end(); ++eraseIt)
			{
				delete* eraseIt;
			}
		}
		// EntityMap���� �ش� Entity�� �����մϴ�.
		delete entityToRemove;
		entityMap.erase(entityIter);
	}
}

void SceneManager::OnClearEntity(std::any data)
{
	// Clear EntityMap and manage memory for Entity*
	for (auto& pair : m_CurrentScene->EntityMap) {
		delete pair.second;  // Assuming that you are responsible for deleting the Entity*
	}
	m_CurrentScene->EntityMap.clear();

	// Clear m_ComponentPool and manage memory for Component* objects
	for (auto& pair : m_CurrentScene->m_ComponentPool) {
		for (Component* comp : pair.second) {
			delete comp;  // Assuming that you are responsible for deleting the Component*
		}
		pair.second.clear();
	}
	m_CurrentScene->m_ComponentPool.clear();
}
//�� ü���� �̺�Ʈ :: SchduleEvent�� ȣ�� ����!
void SceneManager::OnChangeScene(std::any data)
{

	try
	{
		if (data.type() != typeid(std::string))
		{
			// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
			VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
			return;
		}

		//�� ������ �̺�Ʈ ��� ����
		EventManager::GetInstance().GetInstance().ImmediateEvent("OnEndScene");
		//�� ���� �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnOpenScene", data);

	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}
}
void SceneManager::OnStartScene(std::any data)
{
	///TODO: �� ���۽� ������ init??
	EventManager::GetInstance().ImmediateEvent("OnInitialize");
	EventManager::GetInstance().ImmediateEvent("OnInitializeSystem");


}

void SceneManager::OnResetScene(std::any data)
{
	delete m_CurrentScene;
	m_CurrentScene = nullptr;
	m_CurrentScene = new Scene;
}
void SceneManager::OnEndScene(std::any data)
{
	///TODO:�� ������ �� ó���� Finalize?
	EventManager::GetInstance().ImmediateEvent("OnFinalize");
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystem");

}

void SceneManager::OnOpenScene(std::any data)
{
	try
	{
		if (data.type() != typeid(std::string))
		{
			// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
			VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
			return;
		}
		//�� �ʱ�ȭ �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnResetScene");
		//�� �ø�������� �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
		//�� ���� �̺�Ʈ ��� ����
		EventManager::GetInstance().ImmediateEvent("OnStartScene");
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}

}

Entity* SceneManager::GetEntity(uint32_t entityID)
{
	VP_ASSERT(HasEntity(entityID), "Entity�� �������� �ʽ��ϴ�!!");
	return m_CurrentScene->EntityMap[entityID];
}

bool SceneManager::HasEntity(uint32_t entityID)
{
	return m_CurrentScene->EntityMap.count(entityID) > 0;
}



