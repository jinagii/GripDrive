#include "windows.h"
#include "KeyInput.h"

KeyState KeyInput::mKeyStateArr[256];
POINT KeyInput::MousePosScreen;
POINT KeyInput::MousePosClient;

void KeyInput::KeyUpdate()
{
	for (int index = 0; index < 255; index++)
	{
		mKeyStateArr[index].prev = mKeyStateArr[index].current;
		mKeyStateArr[index].current = HIWORD(GetAsyncKeyState(index));
	}
}

bool KeyInput::InputKey(int vk)
{
	return mKeyStateArr[vk].current;
}

bool KeyInput::InputKeyDown(int vk)
{
	return !mKeyStateArr[vk].prev && mKeyStateArr[vk].current;
}

bool KeyInput::InputKeyUp(int vk)
{
	return mKeyStateArr[vk].prev && !mKeyStateArr[vk].current;
}

//마우스 입력은 고쳐야한다.
POINT KeyInput::GetMousePosCT(int wndIdx)
{

	MousePosClient = MousePosScreen;
	//ScreenToClient(BTCore::GetHWnd(wndIdx), &MousePosClient);
	return MousePosClient;
}

POINT KeyInput::GetMousePosSC()
{
	return MousePosScreen;
}

void KeyInput::MouseUpdate()
{
	GetCursorPos(&MousePosScreen);
}