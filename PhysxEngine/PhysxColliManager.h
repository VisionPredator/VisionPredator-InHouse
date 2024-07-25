#pragma once
#include "VPPhysicsStructs.h"
#include "../VPEngine/EventSubscriber.h"


/// <summary>
/// 2024.07.23
/// 작성자 : 유승운
/// 충돌 감지된 엔티티들의 데이터를 관리할 클래스
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
	//어떤 컨테이너에 담을것인가 어떤 데이터를 담을것인가 - 나는 id만 가지고 호출할것이다
	
	std::multimap<uint32_t, uint32_t> Enter;
	std::multimap<uint32_t, uint32_t> Contact;
	std::multimap<uint32_t, uint32_t> Exit;
};
 

