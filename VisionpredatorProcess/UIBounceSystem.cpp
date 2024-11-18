#include "pch.h"
#include "UIBounceSystem.h"
#include "ImageBounceComponent.h"
#include "TextBounceComponent.h"
UIBounceSystem::UIBounceSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
}

void UIBounceSystem::Update(float deltaTime)
{
	COMPLOOP(ImageBounceComponent, bouncecomp)
	{
		if (!bouncecomp.HasImage)
			continue;
		if (bouncecomp.AddedBounce)
			SettingBounce(bouncecomp);
		if (!bouncecomp.Isbouncing)
			continue;

		auto image = bouncecomp.GetComponent<ImageComponent>();
		// BounceProgress 업데이트 (델타 시간을 더해 진행률을 조절)
		bouncecomp.BounceProgress += deltaTime;

		// BounceReturn 함수로 특정 시점 이후 CommonScale로 복귀
		float currentScale= bouncecomp.CommonScele;
		double BounceProgress = static_cast<double>(bouncecomp.BounceProgress);
		double BouncingTime = static_cast<double>(bouncecomp.BouncingTime);
		double BounceTimePercent = static_cast<double>(bouncecomp.BounceTimePercent);
		double percent = BouncePercent(BounceProgress, BouncingTime, BounceTimePercent);
		if (!BounceReturn(BounceProgress, BouncingTime, BounceTimePercent))
			// BounceTimePercent 이하일 때는 Bounce 진행
			currentScale = Lerp(bouncecomp.StartScale, bouncecomp.EndScale, percent);
		else
			// BounceTimePercent 초과 시 CommonScale로 돌아가는 보간
			currentScale = Lerp(bouncecomp.CommonScele,bouncecomp.EndScale,  percent);
		// 곡선 기반의 percent로 스케일 보간
		image->Scale = currentScale;
		// bounce 진행이 완료되면 초기화
		if (bouncecomp.BounceProgress > bouncecomp.BouncingTime)
		{
			bouncecomp.Isbouncing = false;
			bouncecomp.BounceProgress = 0.0f;
			bouncecomp.StartScale = {};
			bouncecomp.EndScale = {};
			image->Scale = bouncecomp.CommonScele;  // 원래 스케일로 복원
		}
	}

	// Text Bounce Update (similar to Image Bounce)
	COMPLOOP(TextBounceComponent, bouncecomp)
	{
		if (!bouncecomp.HasText)
			continue;
		if (bouncecomp.AddedBounce)
			SettingBounce(bouncecomp);
		if (!bouncecomp.Isbouncing)
			continue;

		auto text = bouncecomp.GetComponent<TextComponent>();
		// BounceProgress 업데이트 (델타 시간을 더해 진행률을 조절)
		bouncecomp.BounceProgress += deltaTime;

		// BounceReturn 함수로 특정 시점 이후 CommonScale로 복귀
		float currentScale = bouncecomp.CommonScele;
		double BounceProgress = static_cast<double>(bouncecomp.BounceProgress);
		double BouncingTime = static_cast<double>(bouncecomp.BouncingTime);
		double BounceTimePercent = static_cast<double>(bouncecomp.BounceTimePercent);
		double percent = BouncePercent(BounceProgress, BouncingTime, BounceTimePercent);
		if (!BounceReturn(BounceProgress, BouncingTime, BounceTimePercent))
			// BounceTimePercent 이하일 때는 Bounce 진행
			currentScale = Lerp(bouncecomp.StartScale, bouncecomp.EndScale, percent);
		else
			// BounceTimePercent 초과 시 CommonScale로 돌아가는 보간
			currentScale = Lerp(bouncecomp.CommonScele, bouncecomp.EndScale, percent);
		// 곡선 기반의 percent로 스케일 보간
		text->Scale = currentScale;
		// bounce 진행이 완료되면 초기화
		if (bouncecomp.BounceProgress > bouncecomp.BouncingTime)
		{
			bouncecomp.Isbouncing = false;
			bouncecomp.BounceProgress = 0.0f;
			bouncecomp.StartScale = {};
			bouncecomp.EndScale = {};
			text->Scale = bouncecomp.CommonScele;  // 원래 스케일로 복원
		}
	}
}


void UIBounceSystem::SettingBounce(ImageBounceComponent& bouncecomp)
{
	// Bouncing 시작 설정
	bouncecomp.Isbouncing = true;
	auto text = bouncecomp.GetComponent<ImageComponent>();
	bouncecomp.BounceProgress = 0;
	// StartScale과 EndScale 설정
	bouncecomp.StartScale = text->Scale;
	bouncecomp.EndScale = bouncecomp.StartScale + bouncecomp.AddScale;

	// MaxScale 제한 적용
	if (bouncecomp.EndScale > bouncecomp.MaxScale)
		bouncecomp.EndScale = bouncecomp.MaxScale;

	bouncecomp.AddedBounce = false;
}
void UIBounceSystem::SettingBounce(TextBounceComponent& bouncecomp)
{
	// Bouncing 시작 설정
	bouncecomp.Isbouncing = true;
	auto text = bouncecomp.GetComponent<TextComponent>();
	bouncecomp.BounceProgress = 0;

	// StartScale과 EndScale 설정
	bouncecomp.StartScale = text->Scale;
	bouncecomp.EndScale = bouncecomp.StartScale + bouncecomp.AddScale;

	// MaxScale 제한 적용
	if (bouncecomp.EndScale > bouncecomp.MaxScale)
		bouncecomp.EndScale = bouncecomp.MaxScale;

	bouncecomp.AddedBounce = false;
}

void UIBounceSystem::Initialize()
{
	COMPLOOP(ImageBounceComponent, bouncecomp)
	{
		Start(bouncecomp.GetEntityID());
	}
	COMPLOOP(TextBounceComponent, bouncecomp)
	{
		Start(bouncecomp.GetEntityID());
	}
}

void UIBounceSystem::Start(uint32_t gameObjectId)
{
	if (GetSceneManager()->HasComponent< ImageBounceComponent>(gameObjectId))
	{
		auto bouncecomp = GetSceneManager()->GetComponent< ImageBounceComponent>(gameObjectId);
		if (bouncecomp->HasComponent<ImageComponent>())
		{
			bouncecomp->HasImage = true;
			auto image = bouncecomp->GetComponent<ImageComponent >();
			bouncecomp->CommonScele = image->Scale;
			bouncecomp->AddScale = image->Scale * bouncecomp->AddScalePercent / 100.f;
			bouncecomp->MaxScale = bouncecomp->CommonScele + (bouncecomp->CommonScele * bouncecomp->MaxScalePercent / 100.f);
		}
	}
	if (GetSceneManager()->HasComponent< TextBounceComponent>(gameObjectId))
	{
		auto bouncecomp = GetSceneManager()->GetComponent< TextBounceComponent>(gameObjectId);
		if (bouncecomp->HasComponent<TextComponent>())
		{
			bouncecomp->HasText = true;
			auto text = bouncecomp->GetComponent<TextComponent >();
			bouncecomp->CommonScele = text->Scale;
			bouncecomp->AddScale = text->Scale * bouncecomp->AddScalePercent / 100.f;
			bouncecomp->MaxScale = bouncecomp->CommonScele + (bouncecomp->CommonScele * bouncecomp->MaxScalePercent / 100.f);
		}
	}
}
float UIBounceSystem::Lerp(float start, float end, float percent)
{
	return start + percent * (end - start);
}
double UIBounceSystem::BouncePercent(double progress, double bounceTime, double bounceTimePercent)
{
	// bounceTimePercent 값을 0과 1 사이의 값으로 변환
	double b = bounceTimePercent / 100.0;

	// progress 값이 0 이하일 경우 0 반환
	if (progress <= 0)
		return 0.0f;

	if (progress < bounceTime * b)
	{
		// 첫 번째 절반: 스케일 증가 (아래로 볼록한 포물선)
		double delta = progress - bounceTime * b;
		return -(delta * delta) / (b * b * bounceTime * bounceTime) + 1;
	}
	else if (progress >= bounceTime)
	{
		// bounceTime을 초과한 경우, 원래 스케일로 복원
		return 0.0;
	}
	else
	{
		// 두 번째 절반: 스케일 감소 (위로 볼록한 포물선)
		double delta = progress - bounceTime;
		return (delta * delta) / ((1 - b) * (1 - b) * bounceTime * bounceTime);
	}
}

bool UIBounceSystem::BounceReturn(double progress, double totalBounceTime, double bounceTimePercent)
{
	// BounceTimePercent 값을 0과 1 사이의 값으로 변환
	double threshold = bounceTimePercent / 100.0f;

	// progress가 기준 시간을 초과했는지 확인
	return (progress >= totalBounceTime * threshold);
}


void UIBounceSystem::Finish(uint32_t gameObjectId)
{
}

void UIBounceSystem::Finalize()
{
}
