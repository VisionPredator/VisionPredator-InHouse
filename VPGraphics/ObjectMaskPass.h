#pragma once

/*
 *	특정 오브젝트에 대한 마스크를 생성
 *	즉 특정 오브젝트를 단일 컬러로 렌더한다.
 *	Outline을 그리는 등, 포스트 프로세싱 단계에 활용할 텍스처를 생성하는 패스
 */

class ObjectMaskPass
{
public:
	ObjectMaskPass();

	void Initialize();
	void Render();

	void OnResize();

private:

};

