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
		// BounceProgress ������Ʈ (��Ÿ �ð��� ���� ������� ����)
		bouncecomp.BounceProgress += deltaTime;

		// BounceReturn �Լ��� Ư�� ���� ���� CommonScale�� ����
		float currentScale= bouncecomp.CommonScele;
		double BounceProgress = static_cast<double>(bouncecomp.BounceProgress);
		double BouncingTime = static_cast<double>(bouncecomp.BouncingTime);
		double BounceTimePercent = static_cast<double>(bouncecomp.BounceTimePercent);
		double percent = BouncePercent(BounceProgress, BouncingTime, BounceTimePercent);
		if (!BounceReturn(BounceProgress, BouncingTime, BounceTimePercent))
			// BounceTimePercent ������ ���� Bounce ����
			currentScale = Lerp(bouncecomp.StartScale, bouncecomp.EndScale, percent);
		else
			// BounceTimePercent �ʰ� �� CommonScale�� ���ư��� ����
			currentScale = Lerp(bouncecomp.CommonScele,bouncecomp.EndScale,  percent);
		// � ����� percent�� ������ ����
		image->Scale = currentScale;
		// bounce ������ �Ϸ�Ǹ� �ʱ�ȭ
		if (bouncecomp.BounceProgress > bouncecomp.BouncingTime)
		{
			bouncecomp.Isbouncing = false;
			bouncecomp.BounceProgress = 0.0f;
			bouncecomp.StartScale = {};
			bouncecomp.EndScale = {};
			image->Scale = bouncecomp.CommonScele;  // ���� �����Ϸ� ����
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
		// BounceProgress ������Ʈ (��Ÿ �ð��� ���� ������� ����)
		bouncecomp.BounceProgress += deltaTime;

		// BounceReturn �Լ��� Ư�� ���� ���� CommonScale�� ����
		float currentScale = bouncecomp.CommonScele;
		double BounceProgress = static_cast<double>(bouncecomp.BounceProgress);
		double BouncingTime = static_cast<double>(bouncecomp.BouncingTime);
		double BounceTimePercent = static_cast<double>(bouncecomp.BounceTimePercent);
		double percent = BouncePercent(BounceProgress, BouncingTime, BounceTimePercent);
		if (!BounceReturn(BounceProgress, BouncingTime, BounceTimePercent))
			// BounceTimePercent ������ ���� Bounce ����
			currentScale = Lerp(bouncecomp.StartScale, bouncecomp.EndScale, percent);
		else
			// BounceTimePercent �ʰ� �� CommonScale�� ���ư��� ����
			currentScale = Lerp(bouncecomp.CommonScele, bouncecomp.EndScale, percent);
		// � ����� percent�� ������ ����
		text->Scale = currentScale;
		// bounce ������ �Ϸ�Ǹ� �ʱ�ȭ
		if (bouncecomp.BounceProgress > bouncecomp.BouncingTime)
		{
			bouncecomp.Isbouncing = false;
			bouncecomp.BounceProgress = 0.0f;
			bouncecomp.StartScale = {};
			bouncecomp.EndScale = {};
			text->Scale = bouncecomp.CommonScele;  // ���� �����Ϸ� ����
		}
	}
}


void UIBounceSystem::SettingBounce(ImageBounceComponent& bouncecomp)
{
	// Bouncing ���� ����
	bouncecomp.Isbouncing = true;
	auto text = bouncecomp.GetComponent<ImageComponent>();
	bouncecomp.BounceProgress = 0;
	// StartScale�� EndScale ����
	bouncecomp.StartScale = text->Scale;
	bouncecomp.EndScale = bouncecomp.StartScale + bouncecomp.AddScale;

	// MaxScale ���� ����
	if (bouncecomp.EndScale > bouncecomp.MaxScale)
		bouncecomp.EndScale = bouncecomp.MaxScale;

	bouncecomp.AddedBounce = false;
}
void UIBounceSystem::SettingBounce(TextBounceComponent& bouncecomp)
{
	// Bouncing ���� ����
	bouncecomp.Isbouncing = true;
	auto text = bouncecomp.GetComponent<TextComponent>();
	bouncecomp.BounceProgress = 0;

	// StartScale�� EndScale ����
	bouncecomp.StartScale = text->Scale;
	bouncecomp.EndScale = bouncecomp.StartScale + bouncecomp.AddScale;

	// MaxScale ���� ����
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
	// bounceTimePercent ���� 0�� 1 ������ ������ ��ȯ
	double b = bounceTimePercent / 100.0;

	// progress ���� 0 ������ ��� 0 ��ȯ
	if (progress <= 0)
		return 0.0f;

	if (progress < bounceTime * b)
	{
		// ù ��° ����: ������ ���� (�Ʒ��� ������ ������)
		double delta = progress - bounceTime * b;
		return -(delta * delta) / (b * b * bounceTime * bounceTime) + 1;
	}
	else if (progress >= bounceTime)
	{
		// bounceTime�� �ʰ��� ���, ���� �����Ϸ� ����
		return 0.0;
	}
	else
	{
		// �� ��° ����: ������ ���� (���� ������ ������)
		double delta = progress - bounceTime;
		return (delta * delta) / ((1 - b) * (1 - b) * bounceTime * bounceTime);
	}
}

bool UIBounceSystem::BounceReturn(double progress, double totalBounceTime, double bounceTimePercent)
{
	// BounceTimePercent ���� 0�� 1 ������ ������ ��ȯ
	double threshold = bounceTimePercent / 100.0f;

	// progress�� ���� �ð��� �ʰ��ߴ��� Ȯ��
	return (progress >= totalBounceTime * threshold);
}


void UIBounceSystem::Finish(uint32_t gameObjectId)
{
}

void UIBounceSystem::Finalize()
{
}
