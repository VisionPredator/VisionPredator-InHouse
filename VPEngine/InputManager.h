#pragma once

#include"EventSubscriber.h" 
///Input
#define DIRECTINPUT_VERSION 0x0800 // DirectInput 8.0을 사용
#include <dinput.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#pragma comment(lib, "dinput8.lib")

#define INPUTKEYDOWN(KEYENUM) InputManager::GetInstance().GetKeyDown(KEYENUM)

#define INPUTKEYDOWNS(...) \
    CheckMultipleKeys(__VA_ARGS__)

#define INPUTKEY(KEYENUM) InputManager::GetInstance().GetKey(KEYENUM)
#define INPUTKEYS(...) \
    (CheckMultipleKeysState(__VA_ARGS__))


#define INPUTKEYUP(KEYENUM) InputManager::GetInstance().GetKeyUp(KEYENUM)

enum class MOUSEKEY
{
	LBUTTON = 0,
	RBUTTON = 1,
	MBUTTON = 2,
	BUTTON4 = 3,
	BUTTON5 = 4
};

enum class KEYBOARDKEY
{
	ESCAPE = DIK_ESCAPE,
	NUM1 = DIK_1,
	NUM2 = DIK_2,
	NUM3 = DIK_3,
	NUM4 = DIK_4,
	NUM5 = DIK_5,
	NUM6 = DIK_6,
	NUM7 = DIK_7,
	NUM8 = DIK_8,
	NUM9 = DIK_9,
	NUM0 = DIK_0,
	MINUS = DIK_MINUS,
	EQUALS = DIK_EQUALS,
	BACK = DIK_BACK,
	TAB = DIK_TAB,
	Q = DIK_Q,
	W = DIK_W,
	E = DIK_E,
	R = DIK_R,
	T = DIK_T,
	Y = DIK_Y,
	U = DIK_U,
	I = DIK_I,
	O = DIK_O,
	P = DIK_P,
	LBRACKET = DIK_LBRACKET,
	RBRACKET= DIK_RBRACKET,
	ENTER = DIK_RETURN,
	LCONTROL = DIK_LCONTROL,
	A = DIK_A,
	S = DIK_S,
	D = DIK_D,
	F = DIK_F,
	G = DIK_G,
	H = DIK_H,
	J = DIK_J,
	K = DIK_K,
	L = DIK_L,
	SEMICOLON = DIK_SEMICOLON,
	APOSTROPHE = DIK_APOSTROPHE,
	GRAVE = DIK_GRAVE,
	LSHIFT = DIK_LSHIFT,
	BACKSLASH = DIK_BACKSLASH,
	Z = DIK_Z,
	X = DIK_X,
	C = DIK_C,
	V = DIK_V,
	B = DIK_B,
	N = DIK_N,
	M = DIK_M,
	COMMA = DIK_COMMA,
	PERIOD = DIK_PERIOD,
	SLASH = DIK_SLASH,
	RSHIFT = DIK_RSHIFT,
	MULTIPLY = DIK_MULTIPLY,
	LMENU = DIK_LMENU,
	SPACE = DIK_SPACE,
	CAPITAL = DIK_CAPITAL,
	F1 = DIK_F1,
	F2 = DIK_F2,
	F3 = DIK_F3,
	F4 = DIK_F4,
	F5 = DIK_F5,
	F6 = DIK_F6,
	F7 = DIK_F7,
	F8 = DIK_F8,
	F9 = DIK_F9,
	F10 = DIK_F10,
	NUMLOCK = DIK_NUMLOCK,
	SCROLL = DIK_SCROLL,
	NUMPAD7 = DIK_NUMPAD7,
	NUMPAD8 = DIK_NUMPAD8,
	NUMPAD9 = DIK_NUMPAD9,
	SUBTRACT = DIK_SUBTRACT,
	NUMPAD4 = DIK_NUMPAD4,
	NUMPAD5 = DIK_NUMPAD5,
	NUMPAD6 = DIK_NUMPAD6,
	ADD = DIK_ADD,
	NUMPAD1 = DIK_NUMPAD1,
	NUMPAD2 = DIK_NUMPAD2,
	NUMPAD3 = DIK_NUMPAD3,
	NUMPAD0 = DIK_NUMPAD0,
	DECIMAL = DIK_DECIMAL,
	OEM_102 = DIK_OEM_102,
	F11 = DIK_F11,
	F12 = DIK_F12,
	F13 = DIK_F13,
	F14 = DIK_F14,
	F15 = DIK_F15,
	KANA = DIK_KANA,
	ABNT_C1 = DIK_ABNT_C1,
	CONVERT = DIK_CONVERT,
	NOCONVERT = DIK_NOCONVERT,
	YEN = DIK_YEN,
	ABNT_C2 = DIK_ABNT_C2,
	NUMPADEQUALS = DIK_NUMPADEQUALS,
	PREVTRACK = DIK_PREVTRACK,
	AT = DIK_AT,
	COLON = DIK_COLON,
	UNDERLINE = DIK_UNDERLINE,
	KANJI = DIK_KANJI,
	STOP = DIK_STOP,
	AX = DIK_AX,
	UNLABELED = DIK_UNLABELED,
	NEXTTRACK = DIK_NEXTTRACK,
	NUMPADENTER = DIK_NUMPADENTER,
	RCONTROL = DIK_RCONTROL,
	MUTE = DIK_MUTE,
	CALCULATOR = DIK_CALCULATOR,
	PLAYPAUSE = DIK_PLAYPAUSE,
	MEDIASTOP = DIK_MEDIASTOP,
	VOLUMEDOWN = DIK_VOLUMEDOWN,
	VOLUMEUP = DIK_VOLUMEUP,
	WEBHOME = DIK_WEBHOME,
	NUMPADCOMMA = DIK_NUMPADCOMMA,
	DIVIDE = DIK_DIVIDE,
	SYSRQ = DIK_SYSRQ,
	RMENU = DIK_RMENU,
	PAUSE = DIK_PAUSE,
	HOME = DIK_HOME,
	UP = DIK_UP,
	PRIOR = DIK_PRIOR,
	LEFT = DIK_LEFT,
	RIGHT = DIK_RIGHT,
	END = DIK_END,
	DOWN = DIK_DOWN,
	NEXT = DIK_NEXT,
	INSERT = DIK_INSERT,
	DEL = DIK_DELETE,
	LWIN = DIK_LWIN,
	RWIN = DIK_RWIN,
	APPS = DIK_APPS,
	POWER = DIK_POWER,
	SLEEP = DIK_SLEEP,
	WAKE = DIK_WAKE,
	WEBSEARCH = DIK_WEBSEARCH,
	WEBFAVORITES = DIK_WEBFAVORITES,
	WEBREFRESH = DIK_WEBREFRESH,
	WEBSTOP = DIK_WEBSTOP,
	WEBFORWARD = DIK_WEBFORWARD,
	WEBBACK = DIK_WEBBACK,
	MYCOMPUTER = DIK_MYCOMPUTER,
	MAIL = DIK_MAIL,
	MEDIASELECT = DIK_MEDIASELECT,
};

class InputManager : public EventSubscriber
{

public:
	InputManager();
	static InputManager* instance;
	~InputManager() = default;
	bool Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight);
	bool Update();
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
		Shutdown();
		delete instance;
	}
	int GetMouseX() const { return m_mouseX; }
	int GetMouseY() const { return m_mouseY; }
	int GetMouseDeltaX() const { return m_mouseDeltaX; }
	int GetMouseDeltaY() const { return m_mouseDeltaY; }
	void Shutdown();
	bool GetKeyDown(KEYBOARDKEY inputkey);
	bool GetKeyUp(KEYBOARDKEY inputkey);
	bool GetKey(KEYBOARDKEY inputkey);
	bool GetKeyDown(MOUSEKEY inputkey);
	bool GetKeyUp(MOUSEKEY inputkey);
	bool GetKey(MOUSEKEY);
	bool IsEscapePressed();
private:
	void CopyKeyStateToPrevious();
	void CopyMouseStateToPrevious();
	void CalculateMouseDelta(); // 마우스의 델타 이동을 계산하는 함수
	void ProcessMouseInput();


	IDirectInput8* m_directInput = nullptr;
	IDirectInputDevice8* m_keyboard = nullptr;
	IDirectInputDevice8* m_mouse = nullptr;
	unsigned char m_keyboardState[256] = { 0, };
	unsigned char m_previousKeyboardState[256] = { 0, };
	DIMOUSESTATE m_mouseState; // 현재 마우스 상태
	DIMOUSESTATE m_previousMouseState; // 이전 마우스 상태
	bool ReadKeyboard();
	bool ReadMouse();
	int m_screenWidth = 0;
	int m_screenHeight = 0;
	int m_mouseX = 0;
	int m_mouseY = 0;
	int m_mouseDeltaX=0;
	int m_mouseDeltaY=0;
};

template <typename T>
bool CheckMultipleKeys(T key) {
	return InputManager::GetInstance().GetKeyDown(key);
}

template <typename T, typename... Args>
bool CheckMultipleKeys(T key, Args... keys) {
	return InputManager::GetInstance().GetKeyDown(key) || CheckMultipleKeys(keys...);
}
template <typename T>
bool CheckMultipleKeysState(T key) {
	return InputManager::GetInstance().GetKey(key);
}

template <typename T, typename... Args>
bool CheckMultipleKeysState(T key, Args... keys) {
	return InputManager::GetInstance().GetKey(key) || CheckMultipleKeysState(keys...);
}
