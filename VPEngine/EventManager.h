#pragma once
namespace VisPred
{
	class EventSubscriber;
	using Callback_Event = std::function<void(std::any)>;
	using EventType = std::string;


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


		 void Subscribe(const EventType& type, const Subscriber& listenerInfo);					///�̺�Ʈ ����
		 void ImmediateEvent(const EventType& type, std::any data = std::any{});					///��� ����
		 void ScheduleEvent(const EventType& type, std::any data = std::any{}, float delay = 0);	///delay �Ǵ� ���ο� ������ ���� ���� ����
		 void Unsubscribe(EventSubscriber* listenerInfo);											///���� ���� ���� �������� �ʾҽ��ϴ�. ������� ���� �ڵ尰��.

	private:
		std::vector<ScheduledEvent> m_SpawnEntityEvents;		///Entity �������� �̺�Ʈ�� ���� ó���Ǵ� ���� ���ƺ��δ�.
		std::vector<ScheduledEvent> m_ScheduledEvents;		///����ó�� �̺�Ʈ : �߰������� �����ؾ���. ������ for���� ���⿡�� vector�� ���� ������嵵 ���Ͼ��.
		std::unordered_map<EventType, std::vector<Subscriber>> m_EventSubscribers;

	};

}