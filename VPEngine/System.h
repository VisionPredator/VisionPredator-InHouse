#pragma once
#include "EntityManager.h"
#include "SystemInterface.h"

	class System
	{
	public:
		System(EntityManager* entityManager) :m_EntityManager(entityManager) {}
		virtual ~System() = default;


		template<typename T>
		class CompIter 
		{
			EntityManager* m_EntityManager; // EntityManager의 참조 추가
		public:
			std::vector<std::reference_wrapper<T>> innerVector;

			CompIter(EntityManager* entityManager) : m_EntityManager(entityManager) {
				if (!m_EntityManager) {
					throw std::runtime_error("EntityManager is not initialized");
				}
				auto components = m_EntityManager->GetComponentPool<T>(); // 반환된 벡터를 복사합니다.
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
		EntityManager* m_EntityManager;
	};

