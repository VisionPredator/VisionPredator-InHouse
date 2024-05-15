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

	///�ð��� ������ �����Ǳ����� ���ǹ�.
	auto whenTimeIsOver = [&](const ScheduledEvent& event)
		{
			return event.m_scheduledTime < 0;
		};

	std::erase_if(m_ScheduledEvents, whenTimeIsOver);
}

void EventManager::ImmediateEvent(const EventType& type, std::any data)
{
	// Spawn ���� �̺�Ʈ�� ó��
	if (m_SpawnEventSubscribers.find(type) != m_SpawnEventSubscribers.end())
		for (auto& subscrber : m_SpawnEventSubscribers[type])
			subscrber.CallbackFunction(data);
	//�� �� �Ϲ� �̺�Ʈ�� ó��
	if (m_BasicEventSubscribers.find(type) != m_BasicEventSubscribers.end())
		for (auto& subscrber : m_BasicEventSubscribers[type])
			subscrber.CallbackFunction(data);
}

void EventManager::ScheduleEvent(const EventType& type, std::any data, float delay)
{
	// ImmediateEvent �Լ� ȣ��
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
}



