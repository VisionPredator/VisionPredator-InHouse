#pragma once
#include "SceneManager.h"
#include "SystemInterface.h"
#include "TimeManager.h"
#include "EventManager.h"
#include "InputManager.h"
#include "Components.h"
#include "../VPGraphics/IGraphics.h"
#include <thread>

#define COMPITER(ClassName) CompIter<ClassName>(m_SceneManager)

#define COMPLOOP(ClassName)\
for(ClassName& comp : COMPITER(ClassName))

// Define a macro to handle component splitting and threading
#define THREAD_COMPONENTS(System_Name, ComponentType, Function)                                                                   \
{                                                                                                                                 \
    auto task = [](std::vector<std::reference_wrapper<ComponentType>>& components, size_t start, size_t end, System_Name* sys)    \
    {                                                                                                                             \
        for (size_t i = start; i < end; ++i)                                                                                      \
            (sys->*Function)(components[i]);                                                                                      \
    };                                                                                                                            \
    auto ComponentType##Pool = COMPITER(ComponentType);                                                                           \
    std::vector<std::reference_wrapper<ComponentType>> components(ComponentType##Pool.begin(), ComponentType##Pool.end());        \
    if (!components.empty())                                                                                                      \
    {                                                                                                                             \
        size_t midPoint = components.size() / 2;                                                                                  \
        if (this->m_Jthread1 && this->m_Jthread1->joinable())                                                                     \
            this->m_Jthread1->join();                                                                                             \
        if (this->m_Jthread2 && this->m_Jthread2->joinable())                                                                     \
            this->m_Jthread2->join();                                                                                             \
        *(this->m_Jthread1) = std::jthread(task, std::ref(components), 0, midPoint, this);                                        \
        *(this->m_Jthread2) = std::jthread(task, std::ref(components), midPoint, components.size(), this);                        \
        if (this->m_Jthread1 && this->m_Jthread1->joinable())                                                                     \
            this->m_Jthread1->join();                                                                                             \
        if (this->m_Jthread2 && this->m_Jthread2->joinable())                                                                     \
            this->m_Jthread2->join();                                                                                             \
    }                                                                                                                             \
}

class System
{
public:
	System(SceneManager* sceneManager) :m_SceneManager(sceneManager) {}
	virtual ~System() { m_Jthread1 = nullptr; m_Jthread2 = nullptr; }

		template<typename T>
		class CompIter 
		{
			SceneManager* m_SceneManager; // sceneManager의 참조 추가
		public:
			std::vector<std::reference_wrapper<T>> innerVector;

			CompIter(SceneManager* sceneManager) : m_SceneManager(sceneManager) {
				if (!m_SceneManager) {
					throw std::runtime_error("sceneManager is not initialized");
				}
				auto components = m_SceneManager->GetComponentPool<T>(); // 반환된 벡터를 복사합니다.
				for (T* comp : components) {
					innerVector.emplace_back(*comp);
				}
			}

			typename std::vector<std::reference_wrapper<T>>::iterator begin() {
				return innerVector.begin();
			}

			typename std::vector<std::reference_wrapper<T>>::iterator end() {
				return innerVector.end();
			}
		};

		inline void SetThread1(std::jthread* thread) { m_Jthread1 = thread; }
		inline void SetThread2(std::jthread* thread) { m_Jthread2 = thread; }
		std::jthread& GetThread1() { return *m_Jthread1; }
		std::jthread& GetThread2() { return *m_Jthread2; }
	protected:
		SceneManager* m_SceneManager;
		std::jthread* m_Jthread1=nullptr;
		std::jthread* m_Jthread2 = nullptr;
	};

