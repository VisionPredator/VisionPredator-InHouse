#include "pch.h"
#include "EventManager.h"


EventManager* EventManager::instance = nullptr;
void EventManager::Update(float deltatime)
{
	for (auto& ScheduledEvent : m_ScheduledEvents)
	{
		ScheduledEvent.m_scheduledTime -= deltatime;
		if (ScheduledEvent.m_scheduledTime <= 0)
			ScheduledEvent.m_Event();
	}

	///시간이 지나면 삭제되기위한 조건문.
	auto whenTimeIsOver = [&](const ScheduledEvent& event)
		{
			return event.m_scheduledTime < 0;
		};

	std::erase_if(m_ScheduledEvents, whenTimeIsOver);
}

void EventManager::ImmediateEvent(const EventType& type, std::any data)
{
	// Spawn 관련 이벤트를 처리
	if (m_SpawnEventSubscribers.find(type) != m_SpawnEventSubscribers.end())
		for (auto& subscrber : m_SpawnEventSubscribers[type])
			subscrber.CallbackFunction(data);
	//그 후 일반 이벤트를 처리
	if (m_BasicEventSubscribers.find(type) != m_BasicEventSubscribers.end())
		for (auto& subscrber : m_BasicEventSubscribers[type])
			subscrber.CallbackFunction(data);
}

void EventManager::ScheduleEvent(const EventType& type, std::any data, float delay)
{
	// ImmediateEvent 함수 호출
	auto immediateEvent = [=]() { ImmediateEvent(type, data); };
	m_ScheduledEvents.emplace_back(delay, [=]() {immediateEvent(); });
}

void EventManager::Subscribe(const EventType& type, const  Subscriber& listenerInfo, bool IsSpawnEvent)
{
	if (IsSpawnEvent)
		m_SpawnEventSubscribers[type].push_back(listenerInfo);
	else
		m_BasicEventSubscribers[type].push_back(listenerInfo);
}
void EventManager::Unsubscribe(EventSubscriber* listenerInfo)
{
	// BasicEventSubscribers 확인 및 삭제
	for (auto& listenerInfos : m_BasicEventSubscribers | std::views::values)
	{
		auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
			[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

		listenerInfos.erase(it, listenerInfos.end());
	}

	// SpawnEventSubscribers 확인 및 삭제
	for (auto& listenerInfos : m_SpawnEventSubscribers | std::views::values)
	{
		auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
			[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

		listenerInfos.erase(it, listenerInfos.end());
	}
}



