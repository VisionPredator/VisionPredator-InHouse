#pragma once

#include"EventSubscriber.h" 
///Input
#include <dinput.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#pragma comment(lib, "dinput8.lib")

#define INPUTKEYDOWN(KEYENUM) InputManager::GetInstance().GetKeyDown(KEYENUM)
#define INPUTKEY(KEYENUM) InputManager::GetInstance().GetKey(KEYENUM)
#define INPUTKEYUP(KEYENUM) InputManager::GetInstance().GetKeyUp(KEYENUM)

enum class KEY_STATE
{
	NONE,
	DOWN,
	HOLD,
	UP,
};



//enum class BUTTON
//{
//	RIGHT_BUTTON,
//	LEFT_BUTTON,
//	UP_BUTTON,
//	DOWN_BUTTON,
//	FIRE_BUTTON,
//	RELOAD_BUTTON, 
//	OPTION_BUTTON,
//};




enum class KEY
{
	RIGHT,
	LEFT,
	UP,
	DOWN,
	A, B, C, D, E,
	F, G, H, Q, R,
	S, V, W, X, Z,
	NUM1, NUM2, NUM3, NUM4, NUM5,
	SPACE, ENTER, ESC, LSHFIT, LCTRL,
	LBUTTON, RBUTTON,

	END
};
struct KeyInputInfo
{
	KEY_STATE KeyState;
	bool IsPushed;
};

struct ButtonInfo
{
	int key[(int)KEY::END];
};

class InputManager : public EventSubscriber
{

	///버튼을 아직 어떻게 만들지 모르겠음.
	//std::vector<ButtonInfo> m_ButtonInfos;

	std::vector<KeyInputInfo> m_KeyInputInfos;
	const int arrayVK[(int)KEY::END]
	{
		VK_LEFT,
		VK_RIGHT,
		VK_UP,
		VK_DOWN,
		'A', 'B', 'C', 'D', 'E',
		'F', 'G', 'H', 'Q', 'R',
		'S', 'V', 'W', 'X', 'Z',
		'1', '2', '3', '4', '5',
		VK_SPACE, VK_RETURN, VK_ESCAPE, VK_SHIFT,VK_CONTROL,
		VK_LBUTTON, VK_RBUTTON,
	};
	VPMath::Vector2 m_CurPos;

public:
	InputManager();
	static InputManager* instance;


	void Initialize();
	~InputManager() = default;
	bool Initialize(HINSTANCE, HWND, int, int);
	bool Frame();
	static InputManager& GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new InputManager;
		}
		return *instance;
	}
	void Release()
	{
		m_KeyInputInfos.clear();
		delete instance;
	}
	void ProcessInput();


	void OnSetKeyState(std::any data);
	void Update();
	bool GetKey(KEY key);
	bool GetKeyUp(KEY key);
	bool GetKeyDown(KEY key);
	VPMath::Vector2 GetCurPos() const { return m_CurPos; }
	VPMath::Vector2 GetDeltaCurPos() const { return m_DeltaCurPos; }
	VPMath::Vector2 m_DeltaCurPos{};


	void Shutdown();

private:
	IDirectInput8* m_directInput = nullptr;
	IDirectInputDevice8* m_keyboard = nullptr;
	IDirectInputDevice8* m_mouse = nullptr;
	unsigned char m_keyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState;

	bool ReadKeyboard();
	bool ReadMouse();
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;
};

