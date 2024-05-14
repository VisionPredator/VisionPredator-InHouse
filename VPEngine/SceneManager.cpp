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
	///씬 생성
	m_CurrentScene = new Scene;
}
//씬 체인지 이벤트 :: SchduleEvent로 호출 권장!
void SceneManager::OnChangeScene(std::any data)
{

	try
	{
		if (data.type() != typeid(std::string))
		{
			// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
			VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
			return;
		}

		//씬 끝나는 이벤트 즉시 실행
		EventManager::GetInstance().GetInstance().ImmediateEvent("OnEndScene");
		//씬 열기 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnOpenScene", data);

	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast에 실패하였습니다.");  // Wrong data type assertion
	}
}
void SceneManager::OnStartScene(std::any data)
{
	///TODO: 씬 시작시 설정할 init??
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
	///TODO:씬 끝났을 때 처리할 Finalize?
	EventManager::GetInstance().ImmediateEvent("OnFinalize");
	EventManager::GetInstance().ImmediateEvent("OnFinalizeSystem");
}

void SceneManager::OnOpenScene(std::any data)
{
	try
	{
		if (data.type() != typeid(std::string))
		{
			// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
			VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
			return;
		}
		//씬 초기화 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnResetScene");
		//씬 시리얼라이즈 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnDeSerializeScene", data);
		//씬 시작 이벤트 즉시 실행
		EventManager::GetInstance().ImmediateEvent("OnStartScene");
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast에 실패하였습니다.");  // Wrong data type assertion
	}

}

Entity* SceneManager::GetEntity(uint32_t entityID)
{
	VP_ASSERT(HasEntity(entityID), "Entity가 존재하지 않습니다!!");
	return m_CurrentScene->EntityMap[entityID];
}

bool SceneManager::HasEntity(uint32_t entityID)
{
	return m_CurrentScene->EntityMap.count(entityID) > 0;
}



