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
		int mainCameraCount = 0;						//����ڵ�! �ش� �̸��� �ش��ϴ� ī�޶���� 1���� �ƴ� �� assert�߻�!
		if (data.type() != typeid(std::string))
		{
			// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
			VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
			return;
		}
		std::string entityName = std::any_cast<std::string> (data);	///������Ʈ �̸����� ã����??

		for (CameraComponent& cameracomp : COMPITER(CameraComponent))
		{
			cameracomp.IsMain = false;

			if (cameracomp.GetComponent<IDComponent>()->Name != entityName)
				continue;
			mainCameraCount++;
			cameracomp.IsMain = true;
		}
		if (mainCameraCount!=1)
			VP_ASSERT(false, "�ش� �̸��� �ش��ϴ� ī�޶� 1���� �ƴմϴ�!")
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}
}

void CameraSystem::FixedUpdate(float deltaTime)
{
	bool IsMainCameraExist = false;
	///CameraComponent�� ��ȸ�ϸ鼭 Mainī�޶� �ش��ϴ� �����ͺz�� ã�� ã���� ��ȸ�� �׸��д�.
	for (CameraComponent& cameracomp : COMPITER(CameraComponent))
	{
		if (cameracomp.IsMain != true)
			continue;




		/*if (InputManager::GetInstance().GetKey(KEY::RIGHT))
		{

		}*/
		

		///TODO:���¿� ���⼭ �׷��Ƚ����� ����ī�޶� ���� �����ֱ�!
		{
			TransformComponent cameratrans= *cameracomp.GetComponent<TransformComponent>();
			cameratrans.World_Location;
			cameratrans.World_Quaternion;

			cameracomp.GetComponent<TransformComponent>()->World_Quaternion;
			//	Graphics::Interface->SetCamera(cameracomp������);
			//	Graphics::Interface->SetMeshrender(uint32_t id,cameracomp������);
		}


		///�׷����� for�� �����ϱ�!
		break;
	}

	if (!IsMainCameraExist)
	{
		///�������� �ʴ´ٸ�, 0,0,0�� �ش��ϴ� ī�޶� �������� �ǳ��ֱ�!
	}
}
