#include "pch.h"
#include "RenderSystem.h"
#include "Components.h"
#include "EventManager.h"


RenderSystem::RenderSystem(std::shared_ptr<SceneManager> sceneManager)
	: System(sceneManager)
{
}
void RenderSystem::ComponentAdded(Component* comp)
{
	// Particle Object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		const TransformComponent& transform = *component->GetComponent<TransformComponent>();

		std::string Path;
		Path.assign(component->TexturePath.begin(), component->TexturePath.end());

		effect::ParticleInfo info;
		info.TexturePath = Path;

		info.IsRender = component->IsRender;
		info.IsLoop = component->IsLoop;

		if (component->RestartPrev == true)
			component->Restart = false;
		info.Restart = component->Restart;
		component->RestartPrev = component->Restart;

		info.Gravity = component->Gravity;

		info.Duration = component->Duration;

		info.StartLifetime = component->StartLifetime;
		info.StartSize = component->StartSize;
		info.StartSpeed = component->StartSpeed;
		info.StartColor = component->StartColor;
		info.EndColor = component->EndColor;

		info.Shape.Shape = component->Shape;
		info.Shape.Angle = component->Angle;
		info.Shape.Radius = component->Radius;

		info.Renderer.RenderMode = component->RenderMode;

		info.BlendMode = component->BlendMode;

		info.PosW = transform.World_Location;
		info.Direction = transform.Local_Rotation;
		m_Graphics->CreateParticleObject(component->GetEntityID(), info);
		return;
	}

	// Image Object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ImageComponent>())
	{
		ImageComponent* component = static_cast<ImageComponent*>(comp);
		const TransformComponent& Transform = *component->GetComponent<TransformComponent>();
		ui::ImageInfo info;
		info.ImagePath = component->TexturePath;
		info.PosXPercent = component->PosXPercent;
		info.PosYPercent = component->PosYPercent;
		info.Layer = component->Layer;
		info.Color = component->Color;
		info.Scale = component->Scale;
		info.World = Transform.WorldTransform;
		info.RenderMode = component->RenderMode;
		info.Billboard = component->Billboard;
		info.LeftPercent = component->LeftPercent;
		info.RightPercent = component->RightPercent;
		info.TopPercent = component->TopPercent;
		info.BottomPercent = component->BottomPercent;
		info.DrawAfterText = component->DrawAfterText;

		m_Graphics->CreateImageObject(component->GetEntityID(), info);
	}

	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<TextComponent>())
	{
		TextComponent* component = static_cast<TextComponent*>(comp);
		ui::TextInfo info;
		info.Text = component->Text;
		info.FontPath = component->FontPath;
		info.Color = component->Color;
		info.PosXPercent = component->PosXPercent;
		info.PosYPercent = component->PosYPercent;
		info.Scale = component->Scale;
		info.Angle = component->Angle;
		info.Layer = component->Layer;

		m_Graphics->CreateTextObject(component->GetEntityID(), info);
	}
}

void RenderSystem::ComponentReleased(Component* comp)
{
	// particle object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		m_Graphics->DeleteParticleObjectByID(component->GetEntityID());
	}

	// image object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ImageComponent>())
	{
		ImageComponent* component = static_cast<ImageComponent*>(comp);
		m_Graphics->DeleteImageObject(component->GetEntityID());
	}

	// text object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<TextComponent>())
	{
		TextComponent* component = static_cast<TextComponent*>(comp);
		m_Graphics->DeleteTextObject(component->GetEntityID());
	}
}



void RenderSystem::BeginRenderUpdate(float deltaTime)
{
	///렌더데이터 업데이트 정보 업데이트 하기.
	for (ParticleComponent& component : COMPITER(ParticleComponent))
	{
		const TransformComponent& transform = *component.GetComponent<TransformComponent>();

		effect::ParticleInfo info;
		info.TexturePath = component.TexturePath;

		info.IsRender = component.IsRender;
		info.IsLoop = component.IsLoop;



		if (component.RestartPrev == true)
		{
			component.Restart = false;
		}
		if (component.Restart)
		{
			component.IsPlaying = true;
			component.Progresstime = 0;
		}

		if (component.IsPlaying)
		{
			component.Progresstime += deltaTime;
		}
		if (component.Progresstime > component.Duration)
		{
			component.IsPlaying = false; 
			component.Progresstime = 0;
		}

		info.Restart = component.Restart;
		component.RestartPrev = component.Restart;

		info.Gravity = component.Gravity;

		info.Duration = component.Duration;

		info.StartLifetime = component.StartLifetime;
		info.StartSize = component.StartSize;
		info.StartSpeed = component.StartSpeed;
		info.StartColor = component.StartColor;
		info.EndColor = component.EndColor;

		info.Shape.Shape = component.Shape;
		info.Shape.Angle = component.Angle;
		info.Shape.Radius = component.Radius;

		info.Renderer.RenderMode = component.RenderMode;

		info.BlendMode = component.BlendMode;

		info.PosW = transform.World_Location;
		info.Direction = transform.Local_Rotation;
		m_Graphics->UpdateParticleObject(component.GetComponent<IDComponent>()->GetEntityID(), info);
	}


	for (ImageComponent& component : COMPITER(ImageComponent))
	{
		const TransformComponent& transform = *component.GetComponent<TransformComponent>();
		ui::ImageInfo info;
		info.ImagePath = component.TexturePath;
		info.PosXPercent = component.PosXPercent;
		info.PosYPercent = component.PosYPercent;
		info.Layer = component.Layer;
		info.Color = component.Color;
		info.Scale = component.Scale;
		info.World = transform.WorldTransform;
		info.RenderMode = component.RenderMode;
		info.Billboard = component.Billboard;
		info.LeftPercent = component.LeftPercent;
		info.RightPercent = component.RightPercent;
		info.TopPercent = component.TopPercent;
		info.BottomPercent = component.BottomPercent;
		info.DrawAfterText = component.DrawAfterText;

		m_Graphics->UpdateImageObject(component.GetEntityID(), info);
	}

	for (TextComponent& component: COMPITER(TextComponent))
	{
		ui::TextInfo info;
		info.Text = component.Text;
		info.FontPath = component.FontPath;
		info.PosXPercent = component.PosXPercent;
		info.PosYPercent = component.PosYPercent;
		info.Color = component.Color;
		info.Scale = component.Scale;
		info.Angle = component.Angle;
		info.Layer = component.Layer;

		m_Graphics->UpdateTextObject(component.GetComponent<IDComponent>()->GetEntityID(), info);
	}
	m_Graphics->CulingUpdate();
	m_Graphics->AnimationUpdate(deltaTime);
	m_Graphics->Update(deltaTime);
	m_Graphics->EndUpdate(deltaTime);
}

void RenderSystem::RenderUpdate(float deltaTime)
{
	///변경된 RenderData 를 통해서 컬링하기.


	///이후 소켓에 의한 렌더 데이터 수정하기.
	///Socek
	/// Mesh
	/// 
	/// 
	/// 
}

void RenderSystem::LateRenderUpdate(float deltaTime)
{
	///소켓이 있는 애들만 RenderData 수정하기.

}

void RenderSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}








