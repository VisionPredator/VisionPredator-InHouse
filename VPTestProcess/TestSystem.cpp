#include "pch.h"
#include "TestSystem.h"
#include "Components.h"
#include "EventManager.h"

TestSystem::TestSystem(EntityManager* entityManager)
	:System(entityManager)

{
	EventManager::GetInstance().Subscribe("OnTest", CreateSubscriber(&TestSystem::OnTest));

}

void TestSystem::Update(float deltaTime)
{
	static float a = 0;
	a += deltaTime;
	if (a > 1)
		EventManager::GetInstance().ScheduleEvent("OnTest");

}

void TestSystem::FixedUpdate(float deltaTime)
{
}

void TestSystem::OnTest(std::any test)
{
	///ID 
	///entity 알고있어야한다.
	///id 를 알면 그걸로 entity찾을수있고 그리고 그걸로 comp에 있는 변수를 실행할수있다.
	static int s = 0;
	while (s < 200)
	{
		m_EntityManager->CreateEntity();
		s++;  
	}

}
