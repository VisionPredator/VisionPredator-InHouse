#pragma once
#include "SceneManager.h"
#include "SystemInterface.h"
#include "Components.h"
#include "../VPGraphics/IGraphics.h"

#define COMPITER(ClassName) CompIter<ClassName>(m_SceneManager)



	class System
	{
	public:
		System(SceneManager* entityManager) :m_SceneManager(entityManager) {}
		virtual ~System() = default;


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

	protected:
		SceneManager* m_SceneManager;
	};

