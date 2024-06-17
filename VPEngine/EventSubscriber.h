#pragma once
#include "EventManager.h"

class EventSubscriber
{
public:
	virtual ~EventSubscriber()
	{
		EventManager::GetInstance().Unsubscribe(this);
	}

protected:
	template <typename T> requires std::derived_from<T, EventSubscriber>
	Subscriber CreateSubscriber(void (T::* func)(std::any))
	{
		return Subscriber(this, [this, func](std::any data) { (static_cast<T*>(this)->*func)(data); });
	}


};
