#include "pch.h"
#include "EventManager.h"


namespace VisPred
{
	EventManager* EventManager::instance=nullptr;
	void EventManager::Update(float deltatime)
	{
		// 씬체인지 해결을 위해서 생성이벤트부터 처리한다. : 씬이 바뀌면서 오브젝트가 모두 삭제 되고, 삭제되어야할 오브젝트들이 문제가 생길수있음!
		for (auto& SpawnEntityEvent : m_SpawnEntityEvents)
		{
			SpawnEntityEvent.m_scheduledTime -= deltatime;
			if (SpawnEntityEvent.m_scheduledTime <= 0)
				SpawnEntityEvent.m_Event();
		}
		//
		for (auto& ScheduledEvent : m_ScheduledEvents)
		{
			ScheduledEvent.m_scheduledTime -= deltatime;
			if (ScheduledEvent.m_scheduledTime <= 0)
				ScheduledEvent.m_Event();
		}
		/// 여기서 일단 오브젝트 생성에 대한 이벤트를 모두 실행하는 것이 좋아보인다.


		///시간이 지나면 삭제되기위한 조건문.
		auto whenTimeIsOver = [&](const ScheduledEvent& event)
			{
				return event.m_scheduledTime < 0;
			};

		std::erase_if(m_SpawnEntityEvents, whenTimeIsOver);
		std::erase_if(m_ScheduledEvents, whenTimeIsOver);
	}

	void EventManager::ImmediateEvent(const EventType& type, std::any data)
	{
		if (m_EventSubscribers.find(type) != m_EventSubscribers.end())
		{
			for (auto& subscrber : m_EventSubscribers[type])
			{
				subscrber.CallbackFunction(data);
			}
		}
	}

	void EventManager::ScheduleEvent(const EventType& type, std::any data, float delay)
	{
		auto immediateEvent = [=]() {
			ImmediateEvent(type, data); };// PublishImmediate 함수 호출

		//Spawn이라는 단어가 들어간 Event는 먼저처리한다.
		if ((type.find("spawn") != std::string::npos) || (type.find("Spawn") != std::string::npos))
			m_SpawnEntityEvents.emplace_back(delay, [=]() {immediateEvent(); });
		//그외 다른 함수는 넣어진 순서대로 처리한다.
		else
			m_ScheduledEvents.emplace_back(delay, [=]() {immediateEvent(); });

	}

	void EventManager::Subscribe(const EventType& type, const  Subscriber& listenerInfo)
	{
		m_EventSubscribers[type].push_back(listenerInfo);
	}

	/// <summary>
	/// TODO:: 지우는 방식 고민해보기. 
	/// </summary>
	/// <param name="listenerInfo"></param>
	void EventManager::Unsubscribe(EventSubscriber* listenerInfo)
	{
		///지우는 방식 고민해보기.
		for (auto& listenerInfos : m_EventSubscribers | std::views::values)
		{
			auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
				[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

			listenerInfos.erase(it, listenerInfos.end());
		}
		//m_EventSubscribers.clear();
	} 












}