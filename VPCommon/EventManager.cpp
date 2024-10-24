#include "pch.h"
#include "../VPCommon/EventManager.h"

EventManager* EventManager::instance = nullptr;
void EventManager::Update(float deltatime)
{
	// m_ScheduledEvents�� �ִ� �̺�Ʈ�� m_SchedulingEvents�� �ű��
	for (const auto& event : m_ScheduledSpawnEvents)
		m_ScheduledEvents.push_back(event);
	for (const auto& event : m_ScheduledBasicEvents)
		m_ScheduledEvents.push_back(event);
	for (const auto& event : m_ScheduledSceneEvents)
		m_ScheduledEvents.push_back(event);
	// m_ScheduledEvents ����
	m_ScheduledBasicEvents.clear();
	m_ScheduledSpawnEvents.clear();
	m_ScheduledSceneEvents.clear();

	// m_SchedulingEvents ������Ʈ �� �̺�Ʈ ����
	for (auto& ScheduledEvent : m_ScheduledEvents)
	{
		ScheduledEvent.m_scheduledTime -= deltatime;
		if (ScheduledEvent.m_scheduledTime <= 0)
			ScheduledEvent.m_Event();
	}
	// �ð��� ���� �̺�Ʈ�� m_SchedulingEvents���� ����
	auto whenTimeIsOver = [&](const ScheduledEvent& event)
		{
			return event.m_scheduledTime <= 0;
		};

	std::erase_if(m_ScheduledEvents, whenTimeIsOver);
}

void EventManager::ImmediateEvent(const EventName& type, std::any data)
{
	// �Ϲ� �̺�Ʈ�� ó��
	if (m_BasicEventSubscribers.find(type) != m_BasicEventSubscribers.end())
		for (auto& subscrber : m_BasicEventSubscribers[type])
			subscrber.CallbackFunction(data);
	//���� ������ �����ؼ�
	if (m_SpawnEventSubscribers.find(type) != m_SpawnEventSubscribers.end())
		for (auto& subscrber : m_SpawnEventSubscribers[type])
			subscrber.CallbackFunction(data);
	// Scene ���� �̺�Ʈ�� ó��
	if (m_SceneEventSubscribers.find(type) != m_SceneEventSubscribers.end())
		for (auto& subscrber : m_SceneEventSubscribers[type])
			subscrber.CallbackFunction(data);
}

void EventManager::ScheduleEvent(const EventName& type, std::any data, float delay)
{
	// ImmediateEvent �Լ� ȣ��
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
	// BasicEventSubscribers Ȯ�� �� ����
	for (auto& listenerInfos : m_BasicEventSubscribers | std::views::values)
	{
		auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
			[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

		listenerInfos.erase(it, listenerInfos.end());
	}

	// SpawnEventSubscribers Ȯ�� �� ����
	for (auto& listenerInfos : m_SpawnEventSubscribers | std::views::values)
	{
		auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
			[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

		listenerInfos.erase(it, listenerInfos.end());
	}

	// SceneEventSubscribers Ȯ�� �� ����
	for (auto& listenerInfos : m_SceneEventSubscribers | std::views::values)
	{
		auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
			[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

		listenerInfos.erase(it, listenerInfos.end());
	}
}



