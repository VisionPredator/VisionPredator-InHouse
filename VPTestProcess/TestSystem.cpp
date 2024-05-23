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
	///entity �˰��־���Ѵ�.
	///id �� �˸� �װɷ� entityã�����ְ� �׸��� �װɷ� comp�� �ִ� ������ �����Ҽ��ִ�.
	static int s = 0;
	while (s < 200)
	{
		m_EntityManager->CreateEntity();
		s++;  
	}

}
