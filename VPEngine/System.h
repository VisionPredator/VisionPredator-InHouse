#pragma once
#include "SceneManager.h"
#include "SystemInterface.h"
#include "TimeManager.h"
#include "EventManager.h"
#include "InputManager.h"
#include "Components.h"
#include "../VPGraphics/IGraphics.h"
#include <thread>

#define COMPITER(ClassName) CompIter<ClassName>(m_SceneManager.lock())

#define COMPLOOP(ClassName)\
for(auto& comp : COMPITER(ClassName))

///쓰레드를 위한 매크로
#define THREAD_COMPONENTS(System_Name, ComponentType, Function)                                                                   \
{                                                                                                                                 \
    auto task = [](std::vector<std::shared_ptr<ComponentType>>& components, size_t start, size_t end, System_Name* sys)    \
    {                                                                                                                             \
        for (size_t i = start; i < end; ++i)                                                                                      \
            (sys->*Function)(*components[i]);                                                                                     \
    };                                                                                                                            \
    auto ComponentType##Pool = COMPITER(ComponentType);                                                                           \
    std::vector<std::shared_ptr<ComponentType>> components(ComponentType##Pool.begin(), ComponentType##Pool.end());        \
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
    System(std::shared_ptr<SceneManager> sceneManager) : m_SceneManager(sceneManager) {}
    virtual ~System() { m_Jthread1 = nullptr; m_Jthread2 = nullptr; }

    template<typename T>
    class CompIter
    {
        std::weak_ptr<SceneManager> m_SceneManager; // SceneManager의 weak_ptr 추가
    public:
        std::vector<std::reference_wrapper<T>> innerVector;

        CompIter(std::shared_ptr<SceneManager> sceneManager) : m_SceneManager(sceneManager)
        {
            auto sharedSceneManager = m_SceneManager.lock();
            if (!sharedSceneManager)
                throw std::runtime_error("SceneManager is not initialized");

            // 컴포넌트 풀에서 참조를 추출하여 벡터에 추가합니다.
            auto components = sharedSceneManager->GetComponentPool<T>();
            for (const auto& comp : components)
            {
                innerVector.push_back(*comp);
            }
        }

        typename std::vector<std::reference_wrapper<T>>::iterator begin()
        {
            return innerVector.begin();
        }

        typename std::vector<std::reference_wrapper<T>>::iterator end()
        {
            return innerVector.end();
        }
    };
    SceneManager* GetSceneManager() { return m_SceneManager.lock().get(); }
    inline void SetThread1(std::jthread* thread) { m_Jthread1 = thread; }
    inline void SetThread2(std::jthread* thread) { m_Jthread2 = thread; }
    std::jthread& GetThread1() { return *m_Jthread1; }
    std::jthread& GetThread2() { return *m_Jthread2; }

protected:
    std::weak_ptr<SceneManager> m_SceneManager;
    std::jthread* m_Jthread1 = nullptr;
    std::jthread* m_Jthread2 = nullptr;
};