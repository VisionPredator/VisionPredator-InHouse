#pragma once
	class EventSubscriber;
	using Callback_Event = std::function<void(std::any)>;
	using EventName = std::string;

	struct Subscriber
	{
		EventSubscriber* SubscribedClass;		///클래스 타입을 넣어준다. : 이벤트를 등록시킬 클래스 상속을 넣어줘야한다.
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

		///이벤트 구독
		void Subscribe(const EventName& type, const Subscriber& listenerInfo, EventType  EventType = EventType::BASIC);
		///즉시 실행
		void ImmediateEvent(const EventName& type, std::any data = std::any{});
		///delay 또는 새로운 프레임 시작 이후 실행
		void ScheduleEvent(const EventName& type, std::any data = std::any{}, float delay = 0);
		///이벤트 구독 취소
		void Unsubscribe(EventSubscriber* listenerInfo);

	private:
		//이벤트 예약
		std::vector<ScheduledEvent> m_ScheduledEvents;
		//예약된 이벤트 수락
		std::vector<ScheduledEvent> m_SchedulingEvents;
		//기본적인 이벤트
		std::unordered_map<EventName, std::vector<Subscriber>> m_BasicEventSubscribers;
		//게임도중 오브젝트 생성삭제 관련 이벤트
		std::unordered_map<EventName, std::vector<Subscriber>> m_SpawnEventSubscribers;
		//씬생성삭제에 관련 이벤트
		std::unordered_map<EventName, std::vector<Subscriber>> m_SceneEventSubscribers;
	};

