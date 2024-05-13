#pragma once
namespace VisPred
{
	class EventSubscriber;
	using Callback_Event = std::function<void(std::any)>;
	using EventType = std::string;


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

	class EventManager
	{
	public:
		EventManager() {};
		~EventManager() 
		{
			int a = 5;
		};
		static EventManager* instance;
		static EventManager& GetInstance()
		{
			if (instance == nullptr)
			{
				instance = new EventManager;
			}
			return *instance;
		}
		void Release()
		{
			delete instance;
		}
		

		void Update(float deltatime);


		 void Subscribe(const EventType& type, const Subscriber& listenerInfo);					///이벤트 구독
		 void ImmediateEvent(const EventType& type, std::any data = std::any{});					///즉시 실행
		 void ScheduleEvent(const EventType& type, std::any data = std::any{}, float delay = 0);	///delay 또는 새로운 프레임 시작 이후 실행
		 void Unsubscribe(EventSubscriber* listenerInfo);											///구독 삭제 아직 구현하지 않았습니다. 사용하지 않을 코드같음.

	private:
		std::vector<ScheduledEvent> m_SpawnEntityEvents;		///Entity 생성관련 이벤트가 먼저 처리되는 것이 좋아보인다.
		std::vector<ScheduledEvent> m_ScheduledEvents;		///지연처리 이벤트 : 중간삭제가 용의해야함. 하지만 for문을 돌기에는 vector가 좋고 오버헤드도 안일어난다.
		std::unordered_map<EventType, std::vector<Subscriber>> m_EventSubscribers;

	};

}