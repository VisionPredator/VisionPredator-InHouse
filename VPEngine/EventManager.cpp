#include "pch.h"
#include "EventManager.h"


EventManager* EventManager::instance = nullptr;
void EventManager::Update(float deltatime)
{
	// m_ScheduledEvents에 있는 이벤트를 m_SchedulingEvents로 옮기기
	for (const auto& event : m_ScheduledEvents)
		m_SchedulingEvents.push_back(event);

	// m_ScheduledEvents 비우기
	m_ScheduledEvents.clear();

	// m_SchedulingEvents 업데이트 및 이벤트 실행
	for (auto& ScheduledEvent : m_SchedulingEvents)
	{
		ScheduledEvent.m_scheduledTime -= deltatime;
		if (ScheduledEvent.m_scheduledTime <= 0)
			ScheduledEvent.m_Event();
	}
	// 시간이 지난 이벤트를 m_SchedulingEvents에서 제거
	auto whenTimeIsOver = [&](const ScheduledEvent& event)
		{
			return event.m_scheduledTime <= 0;
		};

	std::erase_if(m_SchedulingEvents, whenTimeIsOver);
}

void EventManager::ImmediateEvent(const EventName& type, std::any data)
{
	// 일반 이벤트를 처리
	if (m_BasicEventSubscribers.find(type) != m_BasicEventSubscribers.end())
		for (auto& subscrber : m_BasicEventSubscribers[type])
			subscrber.CallbackFunction(data);
	//생성 삭제에 관련해서
	if (m_SpawnEventSubscribers.find(type) != m_SpawnEventSubscribers.end())
		for (auto& subscrber : m_BasicEventSubscribers[type])
			subscrber.CallbackFunction(data);
	// Scene 관련 이벤트를 처리
	if (m_SceneEventSubscribers.find(type) != m_SceneEventSubscribers.end())
		for (auto& subscrber : m_SceneEventSubscribers[type])
			subscrber.CallbackFunction(data);
}

void EventManager::ScheduleEvent(const EventName& type, std::any data, float delay)
{
	// ImmediateEvent 함수 호출
	auto immediateEvent = [=]() { ImmediateEvent(type, data); };
	m_ScheduledEvents.emplace_back(delay, [=]() {immediateEvent(); });
}

void EventManager::Subscribe(const EventName& type, const  Subscriber& listenerInfo,EventType EventType )
{
	if (EventType== EventType::BASIC)
		m_BasicEventSubscribers[type].push_back(listenerInfo);
	else if (EventType == EventType::ADD_DELETE)
		m_SpawnEventSubscribers[type].push_back(listenerInfo);
	else if (EventType == EventType::SCENE)
		m_SceneEventSubscribers[type].push_back(listenerInfo);
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

	// SceneEventSubscribers 확인 및 삭제
	for (auto& listenerInfos : m_SceneEventSubscribers | std::views::values)
	{
		auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
			[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

		listenerInfos.erase(it, listenerInfos.end());
	}
}



