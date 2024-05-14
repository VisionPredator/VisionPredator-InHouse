#pragma once
#include "System.h"
#include"EventSubscriber.h"
	class TestSystem :
		public System,public IUpdatable,public IFixedUpdatable, public EventSubscriber
	{
	public:
		TestSystem(EntityManager* entityManager);
		~TestSystem() override = default;



		// IUpdatable��(��) ���� ��ӵ�
		void Update(float deltaTime) override;


		// IFixedUpdatable��(��) ���� ��ӵ�
		void FixedUpdate(float deltaTime) override;

		void OnTest(std::any test);
	};

