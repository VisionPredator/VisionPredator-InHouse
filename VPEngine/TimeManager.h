#pragma once

///누수 없음.


	class TimeManager
	{
	private:
		LARGE_INTEGER m_CurCount{};
		LARGE_INTEGER m_PrevCount{};
		LARGE_INTEGER m_Frequency{};
		double m_DeltaTime = 0.f;				// 프레임 간의 시간 값
		double m_ElapsedTime = 0.f;
		uint32_t m_LastFPS = 0;
		uint32_t m_FPS = 0;

	public:
		TimeManager();
		void Update();
		~TimeManager() {}

		const float GetDeltaTime() { return static_cast<float>(m_DeltaTime); }
		uint32_t GetFPS() { return m_LastFPS; }
	};

