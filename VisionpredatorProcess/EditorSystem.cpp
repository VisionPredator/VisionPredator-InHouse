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
        ShowCursor(TRUE);  // 커서를 표시합니다.
        //CURSORINFO cursorInfo;
        //cursorInfo.cbSize = sizeof(CURSORINFO);

        //if (GetCursorInfo(&cursorInfo))
        //{
        //    // 커서의 현재 가시성 상태와 반대로 설정
        //    if (cursorInfo.flags & CURSOR_SHOWING)
        //    {
        //        ShowCursor(FALSE); // 커서를 숨깁니다.
        //    }
        //    else
        //    {

        //    }
        //}
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
