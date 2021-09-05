#pragma once

#include <map>

#include <Xinput.h>

class DRTimer;

/// XBox 360 게임패드 입력을 위한 급조한 클래스
///
/// 2021.01.27 LeHideOffice
///
class XboxPadInput
{
public:
	XboxPadInput();
	~XboxPadInput();


	void Initialize();

	void Update();
	bool GetKeyDown(int padIndex, int key);


	SHORT GetAxisX(int padIndex);
	SHORT GetAxisY(int padIndex);

	/// 트리거 추가
	SHORT GetLTrigger(int padIndex);
	SHORT GetRTrigger(int padIndex);

	void Vibrate(int padIndex, short leftMotorSpeed, short rightMotorSpeed);
	void MakeLeftVibration(int padIndex, float time);
	void MakeRightVibration(int padIndex, float time);
	void MakeBothVibration(int padIndex, float time);
	void CheckEndVibration();

private:
	XINPUT_STATE m_GamePadState[4];
	DWORD m_PadGetResult[4];

	// #define XINPUT_GAMEPAD_DPAD_UP          0x0001
	// #define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
	// #define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
	// #define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
	// #define XINPUT_GAMEPAD_START            0x0010
	// #define XINPUT_GAMEPAD_BACK             0x0020
	// #define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
	// #define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
	// #define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
	// #define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
	// #define XINPUT_GAMEPAD_A                0x1000
	// #define XINPUT_GAMEPAD_B                0x2000
	// #define XINPUT_GAMEPAD_X                0x4000
	// #define XINPUT_GAMEPAD_Y                0x8000

	// 아날로그 스틱의 값
	// 
	// state.Gamepad.sThumbLX	// 왼쪽 아날로그 스틱 X축
	// state.Gamepad.sThumbLY	// 왼쪽 아날로그 스틱 Y축
	// state.Gamepad.sThumbRX	// 오른쪽 아날로그 스틱 X축
	// state.Gamepad.sThumbRY	// 오른쪽 아날로그 스틱 Y축
	// 
	// 트리거에 해당하는 값
	// state.Gamepad.bLeftTrigger	// 왼쪽 트리거
	// state.Gamepad.bRightTrigger	// 오른쪽 트리거

	float m_PeroidForLeftVib[4];
	float m_PeroidForRightVib[4];

	float m_TimerForLeft[4];
	float m_TimerForRight[4];

	float m_TimeForVibration;	// 진동 기간을 체크하기 위한 시각
	
	DRTimer* m_TimerForVibration;

public:
	

};

