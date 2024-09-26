#include "pch.h"
#include "UISystem.h"

UISystem::UISystem(std::shared_ptr<SceneManager> sceneManager):System{ sceneManager }
{
}
void UISystem::ComponentAdded(Component* comp)
{
	//// UI Object
	//if (comp->GetHandle()->type().id() == Reflection::GetTypeID<Sprite2DComponent>())
	//{
	//	Sprite2DComponent* component = static_cast<Sprite2DComponent*>(comp);
	//	ui::ImageInfo info;
	//	info.ImagePath = component->TexturePath;
	//	info.PosXPercent = component->PosXPercent;
	//	info.PosYPercent = component->PosYPercent;
	//	info.Layer = component->Layer;
	//	info.Color = component->Color;
	//	info.Scale = component->Scale;

	//	m_Graphics->CreateImageObject(component->GetEntityID(), info);
	//}
}

void UISystem::ComponentReleased(Component* comp)
{
	//if (comp->GetHandle()->type().id() == Reflection::GetTypeID<Sprite2DComponent>())
	//{
	//	Sprite2DComponent* component = static_cast<Sprite2DComponent*>(comp);
	//	m_Graphics->DeleteImageObject(component->GetEntityID());
	//}
}

void UISystem::BeginRenderUpdate(float deltaTime)
{
	//COMPLOOP(Sprite2DComponent, spriteComp)
	//{
	//	ui::ImageInfo info;
	//	info.ImagePath = spriteComp.TexturePath;
	//	info.PosXPercent = spriteComp.PosXPercent;
	//	info.PosYPercent = spriteComp.PosYPercent;
	//	info.Layer = spriteComp.Layer;
	//	info.Color = spriteComp.Color;
	//	info.Scale = spriteComp.Scale;

	//	m_Graphics->UpdateImageObject(spriteComp.GetEntityID(), info);
	//}
}

void UISystem::RenderUpdate(float deltaTime)
{

}

void UISystem::LateRenderUpdate(float deltaTime)
{
}

void UISystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}

