#include "pch.h"
#include "SceneManager.h"
#include "EntityManager.h"

SceneManager::SceneManager()
{
	EventManager::GetInstance().Subscribe("OnChangeScene", CreateSubscriber(&SceneManager::OnChangeScene));
	EventManager::GetInstance().Subscribe("OnEndScene", CreateSubscriber(&SceneManager::OnEndScene));
	EventManager::GetInstance().Subscribe("OnResetScene", CreateSubscriber(&SceneManager::OnResetScene));
	EventManager::GetInstance().Subscribe("OnOpenScene", CreateSubscriber(&SceneManager::OnOpenScene));
	EventManager::GetInstance().Subscribe("OnStartScene", CreateSubscriber(&SceneManager::OnStartScene));
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



