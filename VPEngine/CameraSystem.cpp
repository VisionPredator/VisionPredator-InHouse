#include "pch.h"
#include "Components.h"
#include "CameraSystem.h"


CameraSystem::CameraSystem(EntityManager* entityManager) :System(entityManager)
{
	EventManager::GetInstance().Subscribe("OnSetMainCamera",CreateSubscriber(&CameraSystem::OnSetMainCamera));

}


void CameraSystem::OnSetMainCamera(std::any data)
{

	try
	{
		int mainCameraCount = 0;						//방어코드! 해당 이름에 해당하는 카메라수가 1개가 아닐 때 assert발생!
		if (data.type() != typeid(std::string))
		{
			// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
			VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
			return;
		}
		std::string entityName = std::any_cast<std::string> (data);	///오브젝트 이름으로 찾을까??

		for (CameraComponent& cameracomp : COMPITER(CameraComponent))
		{
			cameracomp.IsMain = false;

			if (cameracomp.GetComponent<IDComponent>()->Name != entityName)
				continue;
			mainCameraCount++;
			cameracomp.IsMain = true;
		}
		if (mainCameraCount!=1)
			VP_ASSERT(false, "해당 이름에 해당하는 카메라가 1개가 아닙니다!")
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast에 실패하였습니다.");  // Wrong data type assertion
	}
}

void CameraSystem::FixedUpdate(float deltaTime)
{
	bool IsMainCameraExist = false;
	///CameraComponent를 순회하면서 Main카메라에 해당하는 컴포넌틑를 찾아 찾을시 순회를 그만둔다.
	for (CameraComponent& cameracomp : COMPITER(CameraComponent))
	{
		if (cameracomp.IsMain != true)
			continue;




		/*if (InputManager::GetInstance().GetKey(KEY::RIGHT))
		{

		}*/
		

		///TODO:유승운 여기서 그래픽스한테 메인카메라 설정 보내주기!
		{
			TransformComponent cameratrans= *cameracomp.GetComponent<TransformComponent>();
			cameratrans.World_Location;
			cameratrans.World_Quaternion;

			cameracomp.GetComponent<TransformComponent>()->World_Quaternion;
			//	Graphics::Interface->SetCamera(cameracomp의정보);
			//	Graphics::Interface->SetMeshrender(uint32_t id,cameracomp의정보);
		}


		///그런다음 for문 종료하기!
		break;
	}

	if (!IsMainCameraExist)
	{
		///존재하지 않는다면, 0,0,0에 해당하는 카메라 정보값을 건네주기!
	}
}
