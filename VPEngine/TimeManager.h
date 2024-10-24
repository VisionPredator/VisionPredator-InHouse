#pragma once

///���� ����.
#include "EventSubscriber.h"
class TimeManager :public EventSubscriber
{
private:
	LARGE_INTEGER m_CurCount{};
	LARGE_INTEGER m_PrevCount{};
	LARGE_INTEGER m_Frequency{};
	double m_DeltaTime = 0.f;				// ������ ���� �ð� ��
	double m_ElapsedTime = 0.f;
	uint32_t m_LastFPS = 0;
	uint32_t m_PreviousLastFPS = 0;
	uint32_t m_FPS = 0;

	double m_TotalTime = 0;

public:
	TimeManager();
	void Update();
	~TimeManager() {}
	const float GetDeltaTime() { return static_cast<float>(m_DeltaTime); }
	uint32_t GetFPS() { return m_LastFPS; }
	uint32_t GetPrevFPS() { return m_PreviousLastFPS; }

	const float GetTotalGameTime() { return static_cast<float>(m_TotalTime); }
};

