#include "pch.h"
#include "FPSSystem.h"
#include "FPSComponent.h"
FPSSystem::FPSSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
}

void FPSSystem::BeginRenderUpdate(float deltaTime)
{
}

void FPSSystem::RenderUpdate(float deltaTime)
{
}

void FPSSystem::LateRenderUpdate(float deltaTime)
{
	COMPLOOP(FPSComponent, comp)
	{

		auto ui = comp.GetComponent<TextComponent>();

		float text = (1.f / deltaTime);
		ui::TextInfo info;

		// 소수 첫 번째 자리까지 표시
		std::wstringstream stream;
		stream << std::fixed << std::setprecision(1) << text;
		info.Text = stream.str();
		info.FontPath = ui->FontPath;
		info.PosXPercent = ui->PosXPercent;
		info.PosYPercent = ui->PosYPercent;
		info.Color = ui->Color;
		info.Scale = ui->Scale;
		info.Angle = ui->Angle;
		info.Layer = ui->Layer;
		if (!comp.IsShow)
			info.Color.w = 0;
		m_Graphics->UpdateTextObject(ui->GetEntityID(), info);
	}

}

void FPSSystem::EditorRenderUpdate(float deltaTime)
{
}

void FPSSystem::Update(float deltaTime)
{
	COMPLOOP(FPSComponent, comp)
		if (INPUTKEYDOWN(KEYBOARDKEY::F2))
			comp.IsShow = !comp.IsShow;


}
