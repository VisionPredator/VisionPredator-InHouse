#include "pch.h"
#include "CursorSystem.h"
#include "CursorComponent.h"
CursorSystem::CursorSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
}

void CursorSystem::Initialize()
{

	COMPLOOP(CursorComponent, comp)
	{
		Start(comp.GetEntityID());
	}
}

void CursorSystem::Start(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity->HasComponent<CursorComponent>())
		return;

	auto cursor = entity->GetComponent<CursorComponent>();
	cursor->preIsCursor = cursor->ShowCursor;
	auto image = entity->GetComponent<ImageComponent>();
	if (cursor->ShowCursor)
	{
		image->TexturePath = cursor->CursorImage;
		image->Scale = cursor->CursorScale;
		SetImagePos(image);
		image->Color.w = 1;


	}	
	else
	{
		image->TexturePath = cursor->CursorImage;
		image->Scale = cursor->CursorScale;
		SetImagePos(image);
		image->Color.w = 0;

	}

}

void CursorSystem::Finish(uint32_t gameObjectId)
{

}

void CursorSystem::Finalize()
{

}

void CursorSystem::SetImagePos(ImageComponent* image)
{
	VPMath::Vector2 clientsize = InputManager::GetInstance().GetClientSize();
	int x =InputManager::GetInstance().GetMouseX();
	int y = InputManager::GetInstance().GetMouseY();

	image->PosXPercent = (x / clientsize.x)*100;
	image->PosYPercent = (y / clientsize.y) * 100;
}

void CursorSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(CursorComponent, cursor)
	{
		if (!cursor.HasComponent<ImageComponent>())
			continue;
		auto cursorimage = cursor.GetComponent<ImageComponent>();

		if (cursor.ShowCursor != cursor.preIsCursor)
		{

			if (cursor.ShowCursor)
				cursorimage->Color.w = 1;
			else
				cursorimage->Color.w = 0;
			cursor.preIsCursor = cursor.ShowCursor;
		}

		if (cursor.ShowCursor)
			SetImagePos(cursorimage);

	}
}
