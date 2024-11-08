#include "pch.h"
#include "EditorSystem.h"

EditorSystem::EditorSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{

}

void EditorSystem::Update(float deltaTime)
{
    if (INPUTKEYDOWN(KEYBOARDKEY::F2))
    {
        isDebugRender = !isDebugRender;
        m_Graphics->DebugRenderONOFF(isDebugRender);
    }
    if (INPUTKEYDOWN(KEYBOARDKEY::ESCAPE))
    {
        CURSORINFO cursorInfo;
        cursorInfo.cbSize = sizeof(CURSORINFO);

        if (GetCursorInfo(&cursorInfo))
        {
            // Ŀ���� ���� ���ü� ���¿� �ݴ�� ����
            if (cursorInfo.flags & CURSOR_SHOWING)
            {
                ShowCursor(FALSE); // Ŀ���� ����ϴ�.
            }
            else
            {
                ShowCursor(TRUE);  // Ŀ���� ǥ���մϴ�.
            }
        }
    }
}

void EditorSystem::BeginRenderUpdate(float deltaTime)
{
	

}

void EditorSystem::RenderUpdate(float deltaTime)
{
}

void EditorSystem::LateRenderUpdate(float deltaTime)
{
}

void EditorSystem::EditorRenderUpdate(float deltaTime)
{

}
