#pragma once

#include <map>

#include <Xinput.h>

class DRTimer;

/// XBox 360 �����е� �Է��� ���� ������ Ŭ����
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

	/// Ʈ���� �߰�
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

	// �Ƴ��α� ��ƽ�� ��
	// 
	// state.Gamepad.sThumbLX	// ���� �Ƴ��α� ��ƽ X��
	// state.Gamepad.sThumbLY	// ���� �Ƴ��α� ��ƽ Y��
	// state.Gamepad.sThumbRX	// ������ �Ƴ��α� ��ƽ X��
	// state.Gamepad.sThumbRY	// ������ �Ƴ��α� ��ƽ Y��
	// 
	// Ʈ���ſ� �ش��ϴ� ��
	// state.Gamepad.bLeftTrigger	// ���� Ʈ����
	// state.Gamepad.bRightTrigger	// ������ Ʈ����

	float m_PeroidForLeftVib[4];
	float m_PeroidForRightVib[4];

	float m_TimerForLeft[4];
	float m_TimerForRight[4];

	float m_TimeForVibration;	// ���� �Ⱓ�� üũ�ϱ� ���� �ð�
	
	DRTimer* m_TimerForVibration;

public:
	

};

