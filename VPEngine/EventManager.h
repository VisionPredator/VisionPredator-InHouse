#pragma once
	class EventSubscriber;
	using Callback_Event = std::function<void(std::any)>;
	using EventName = std::string;

	struct Subscriber
	{
		EventSubscriber* SubscribedClass;		///Ŭ���� Ÿ���� �־��ش�. : �̺�Ʈ�� ��Ͻ�ų Ŭ���� ����� �־�����Ѵ�.
		Callback_Event CallbackFunction;
		Subscriber(EventSubscriber* subscribedclass, Callback_Event callback)
			:SubscribedClass(subscribedclass), CallbackFunction(callback) {}
	};
	struct ScheduledEvent
	{
		float m_scheduledTime = 0;
		std::function<void()> m_Event;
		ScheduledEvent(float delay, std::function<void()> event)
			: m_scheduledTime(delay), m_Event(event) {}
	};

	enum class EventType
	{
		BASIC,
		ADD_DELETE,
		SCENE
	};


	class EventManager
	{
	public:

		static EventManager* instance;
		static EventManager& GetInstance()
		{
			if (instance == nullptr)
				instance = new EventManager;
			return *instance;
		}
		void Release()
		{
			delete instance;
		}


		void Update(float deltatime);

		///�̺�Ʈ ����
		void Subscribe(const EventName& type, const Subscriber& listenerInfo, EventType  EventType = EventType::BASIC);
		///��� ����
		void ImmediateEvent(const EventName& type, std::any data = std::any{});
		///delay �Ǵ� ���ο� ������ ���� ���� ����
		void ScheduleEvent(const EventName& type, std::any data = std::any{}, float delay = 0);
		///�̺�Ʈ ���� ���
		void Unsubscribe(EventSubscriber* listenerInfo);

	private:
		//�̺�Ʈ ����
		std::vector<ScheduledEvent> m_ScheduledEvents;
		//����� �̺�Ʈ ����
		std::vector<ScheduledEvent> m_SchedulingEvents;
		//�⺻���� �̺�Ʈ
		std::unordered_map<EventName, std::vector<Subscriber>> m_BasicEventSubscribers;
		//���ӵ��� ������Ʈ �������� ���� �̺�Ʈ
		std::unordered_map<EventName, std::vector<Subscriber>> m_SpawnEventSubscribers;
		//������������ ���� �̺�Ʈ
		std::unordered_map<EventName, std::vector<Subscriber>> m_SceneEventSubscribers;
	};

