#include "pch.h"
#include "TestSystem.h"
#include "Components.h"
#include "EventManager.h"

TestSystem::TestSystem(SceneManager* sceneManager)
	:System(sceneManager)

{
	EventManager::GetInstance().Subscribe("OnTest", CreateSubscriber(&TestSystem::OnTest));

}

void TestSystem::Update(float deltaTime)
{
	static float a = 0;
	a += deltaTime;
	if (a > 1)
		EventManager::GetInstance().ScheduleEvent("OnTest");
	//if (a>8)
	//{
	//	for (IDComponent& idcomp : CompIter<IDComponent>(m_SceneManager))
	//	{
	//		idcomp.GetEntity()->RemoveComponent<IDComponent>();
	//	}
	//	for (TransformComponent& transformComponent : COMPITER(TransformComponent))
	//	{
	//		transformComponent.GetEntity()->RemoveComponent<TransformComponent>();
	//	}
	//	//EventManager::GetInstance().ScheduleEvent("OnSerializeScene");
	//	std::string sceneName = "LDH_Map";
	//	//EventManager::GetInstance().ScheduleEvent("OnChangeScene", sceneName);
	//}
}

void TestSystem::FixedUpdate(float deltaTime)
{
}

void TestSystem::OnTest(std::any test)
{
	///ID 
	///entity �˰��־���Ѵ�.
	///id �� �˸� �װɷ� entityã�����ְ� �׸��� �װɷ� comp�� �ִ� ������ �����Ҽ��ִ�.
	static int s = 0;
	while (s < 400)
	{
		m_SceneManager->CreateEntity();
		///
		/// ADDComponen
		///
		//Entity* testentity=new Entity;
		//m_SceneManager->AddComponent<TransformComponent>(testentity->GetEntityID());
		s++;  
	}

}
