#include "pch.h"
#include "EventManager.h"


namespace VisPred
{
	EventManager* EventManager::instance=nullptr;
	void EventManager::Update(float deltatime)
	{
		// ��ü���� �ذ��� ���ؼ� �����̺�Ʈ���� ó���Ѵ�. : ���� �ٲ�鼭 ������Ʈ�� ��� ���� �ǰ�, �����Ǿ���� ������Ʈ���� ������ ���������!
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
		/// ���⼭ �ϴ� ������Ʈ ������ ���� �̺�Ʈ�� ��� �����ϴ� ���� ���ƺ��δ�.


		///�ð��� ������ �����Ǳ����� ���ǹ�.
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
			ImmediateEvent(type, data); };// PublishImmediate �Լ� ȣ��

		//Spawn�̶�� �ܾ �� Event�� ����ó���Ѵ�.
		if ((type.find("spawn") != std::string::npos) || (type.find("Spawn") != std::string::npos))
			m_SpawnEntityEvents.emplace_back(delay, [=]() {immediateEvent(); });
		//�׿� �ٸ� �Լ��� �־��� ������� ó���Ѵ�.
		else
			m_ScheduledEvents.emplace_back(delay, [=]() {immediateEvent(); });

	}

	void EventManager::Subscribe(const EventType& type, const  Subscriber& listenerInfo)
	{
		m_EventSubscribers[type].push_back(listenerInfo);
	}

	/// <summary>
	/// TODO:: ����� ��� ����غ���. 
	/// </summary>
	/// <param name="listenerInfo"></param>
	void EventManager::Unsubscribe(EventSubscriber* listenerInfo)
	{
		///����� ��� ����غ���.
		for (auto& listenerInfos : m_EventSubscribers | std::views::values)
		{
			auto it = std::remove_if(listenerInfos.begin(), listenerInfos.end(),
				[listenerInfo](const Subscriber& info) { return info.SubscribedClass == listenerInfo; });

			listenerInfos.erase(it, listenerInfos.end());
		}
		//m_EventSubscribers.clear();
	} 












}