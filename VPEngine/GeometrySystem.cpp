#include "pch.h"
#include "GeometrySystem.h"

GeometrySystem::GeometrySystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void GeometrySystem::ComponentAdded(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();

		auto IDComp = meshComponent->GetComponent<IDComponent>();
		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->EntityID = meshComponent->GetEntityID();
		meshComponent->Renderdata->Filter = meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->useTexture = meshComponent->UseTexture;
		meshComponent->Renderdata->color = meshComponent->color;
		meshComponent->Renderdata->textureName = meshComponent->TextureName;
		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}
}

void GeometrySystem::ComponentReleased(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();

		return;
	}
}

void GeometrySystem::BeginRenderUpdate(float deltaTime)
{
	COMPLOOP(GeometryComponent, geometryComp)
	{
		const TransformComponent& transform = *geometryComp.GetComponent<TransformComponent>();

		auto renderdata = geometryComp.Renderdata;
		renderdata->color = geometryComp.color;
		renderdata->color.w = geometryComp.UseTexture;	//shader에서 color의 w값으로 텍스처있는지 판단함
		renderdata->useTexture = geometryComp.UseTexture;
		renderdata->Filter = geometryComp.FBXFilter;
		renderdata->world = transform.WorldTransform;
		renderdata->useTexture = geometryComp.UseTexture;
		renderdata->textureName = geometryComp.TextureName;
		renderdata->Pass = geometryComp.pass;


		switch (geometryComp.FBXFilter)
		{
		case GeoMetryFilter::Grid:
		{
			debug::GridInfo gridInfo;
			gridInfo.Origin = VPMath::Vector3{ 0, 0, 0 };
			gridInfo.XAsix = VPMath::Vector3{ 1, 0, 0 };
			gridInfo.YAsix = VPMath::Vector3{ 0, 0, 1 };
			gridInfo.XDivs = 200;
			gridInfo.YDivs = 200;
			gridInfo.GridSize = 200.f;
			gridInfo.Color = VPMath::Color{ 1,1,1, 1 };
			m_Graphics->DrawGrid(gridInfo);
		}
		break;

		case GeoMetryFilter::Axis:
		{
			float distance = 10;

			//x
			debug::RayInfo x;
			x.Origin = VPMath::Vector3{ 0, 0, 0 };
			x.Direction = VPMath::Vector3{ distance, 0, 0 };
			x.Normalize = false;
			x.Color = VPMath::Color{ 1, 0, 0, 1 };
			m_Graphics->DrawRay(x);

			//y
			debug::RayInfo y;
			y.Origin = VPMath::Vector3{ 0, 0, 0 };
			y.Direction = VPMath::Vector3{ 0, distance, 0 };
			y.Normalize = false;
			y.Color = VPMath::Color{ 0, 1, 0, 1 };
			m_Graphics->DrawRay(y);

			//z
			debug::RayInfo z;
			z.Origin = VPMath::Vector3{ 0, 0, 0 };
			z.Direction = VPMath::Vector3{ 0, 0, distance };
			z.Normalize = false;
			z.Color = VPMath::Color{ 0, 0, 1, 1 };
			m_Graphics->DrawRay(z);

		}
		break;

		case GeoMetryFilter::Frustum:
		{

			debug::FrustumInfo frustumInfo;

			frustumInfo.Frustum.Origin = { renderdata->world._41,renderdata->world._42,renderdata->world._43 };
			frustumInfo.Frustum.Orientation = { 0, 0, 0, 1 };
			frustumInfo.Frustum.Near = 1.f;
			frustumInfo.Frustum.Far = 1000.f;
			frustumInfo.Frustum.LeftSlope = -0.736355f;
			frustumInfo.Frustum.RightSlope = 0.736355f;
			frustumInfo.Frustum.TopSlope = 0.4142f;
			frustumInfo.Frustum.BottomSlope = -0.4142;
			frustumInfo.Color = VPMath::Color{ 1, 1, 0, 1 };

			m_Graphics->DrawFrustum(frustumInfo);

		}
		break;

		default:
			break;
		}
	}
}

void GeometrySystem::RenderUpdate(float deltaTime)
{

}

void GeometrySystem::LateRenderUpdate(float deltaTime)
{
}

void GeometrySystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}
