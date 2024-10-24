#include "pch.h"
#include "../VPCommon/EventManager.h"

EventManager* EventManager::instance = nullptr;
void EventManager::Update(float deltatime)
{
	// m_ScheduledEvents에 있는 이벤트를 m_SchedulingEvents로 옮기기
	for (const auto& event : m_ScheduledSpawnEvents)
		m_ScheduledEvents.push_back(event);
	for (const auto& event : m_ScheduledBasicEvents)
		m_ScheduledEvents.push_back(event);
	for (const auto& event : m_ScheduledSceneEvents)
		m_ScheduledEvents.push_back(event);
	// m_ScheduledEvents 비우기
	m_ScheduledBasicEvents.clear();
	m_ScheduledSpawnEvents.clear();
	m_ScheduledSceneEvents.clear();

	// m_SchedulingEvents 업데이트 및 이벤트 실행
	for (auto& ScheduledEvent : m_ScheduledEvents)
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

	std::erase_if(m_ScheduledEvents, whenTimeIsOver);
}

void EventManager::ImmediateEvent(const EventName& type, std::any data)
{
	// 일반 이벤트를 처리
	if (m_BasicEventSubscribers.find(type) != m_BasicEventSubscribers.end())
		for (auto& subscrber : m_BasicEventSubscribers[type])
			subscrber.CallbackFunction(data);
	//생성 삭제에 관련해서
	if (m_SpawnEventSubscribers.find(type) != m_SpawnEventSubscribers.end())
		for (auto& subscrber : m_SpawnEventSubscribers[type])
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


	if (m_BasicEventSubscribers.find(type) != m_BasicEventSubscribers.end())
		m_ScheduledBasicEvents.emplace_back(delay, [=]() {immediateEvent(); });

	if (m_SpawnEventSubscribers.find(type) != m_SpawnEventSubscribers.end())
		m_ScheduledSpawnEvents.emplace_back(delay, [=]() {immediateEvent(); });

	if (m_SceneEventSubscribers.find(type) != m_SceneEventSubscribers.end())
		m_ScheduledSceneEvents.emplace_back(delay, [=]() {immediateEvent(); });
}

void EventManager::Subscribe(const EventName& type, const  Subscriber& listenerInfo, EventType EventType)
{
	if (EventType == EventType::BASIC)
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



