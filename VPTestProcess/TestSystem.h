#pragma once
#include "System.h"
#include"EventSubscriber.h"
	class TestSystem :
		public System,public IUpdatable,public IFixedUpdatable, public EventSubscriber
	{
	public:
		TestSystem(EntityManager* entityManager);
		~TestSystem() override = default;



		// IUpdatable을(를) 통해 상속됨
		void Update(float deltaTime) override;


		// IFixedUpdatable을(를) 통해 상속됨
		void FixedUpdate(float deltaTime) override;

		void OnTest(std::any test);
	};

