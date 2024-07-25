#pragma once
#include "VPPhysicsStructs.h"
#include "../VPEngine/EventSubscriber.h"


/// <summary>
/// 2024.07.23
/// �ۼ��� : ���¿�
/// �浹 ������ ��ƼƼ���� �����͸� ������ Ŭ����
/// </summary>
class CollisionManager:public EventSubscriber
{
public: 
	CollisionManager();
	~CollisionManager();
	void Update();

private:
	void OnAddEnterData(std::any entitypair);
	void OnAddContactData(std::any entitypair);
	void OnAddExitData(std::any entitypair);
	void OnEraseData(std::any entitypair);
	void OnFindData(std::any entitypair);
	std::vector<VPPhysics::CollisionData> m_CollisionData;
	//� �����̳ʿ� �������ΰ� � �����͸� �������ΰ� - ���� id�� ������ ȣ���Ұ��̴�
	
	std::multimap<uint32_t, uint32_t> Enter;
	std::multimap<uint32_t, uint32_t> Contact;
	std::multimap<uint32_t, uint32_t> Exit;
};
 

