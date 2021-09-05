#include <windows.h>
#include "XboxPadInput.h"
#include "DRTimer.h"

#pragma comment(lib, "xinput.lib")

XboxPadInput::XboxPadInput()
{
	m_TimerForVibration = new DRTimer();

	for (int i = 0; i < 4; i++)
	{
		ZeroMemory(&m_GamePadState[i], sizeof(XINPUT_STATE));	// ��Ʈ�ѷ� ���� �ʱ�ȭ
	
		m_PadGetResult[i] = 0;

		m_TimerForLeft[i] = m_TimerForVibration->GetCurrentTimeSec();
		m_TimerForRight[i] = m_TimerForVibration->GetCurrentTimeSec();

		m_PeroidForLeftVib[i] = -1.0f;
		m_PeroidForRightVib[i] = -1.0f;
	}

}

XboxPadInput::~XboxPadInput()
{

}

void XboxPadInput::Initialize()
{
	for (int i = 0; i < 4; i++)
	{
		ZeroMemory(&m_GamePadState[i], sizeof(XINPUT_STATE));	// ��Ʈ�ѷ� ���� �ʱ�ȭ

		m_PadGetResult[i] = XInputGetState(i, &m_GamePadState[i]);			// ��Ʈ�ѷ� ���� ������
	}


}

void XboxPadInput::Update()
{
	m_TimerForVibration->Update();	// �ð� ����� �ؾ� ��������
	this->CheckEndVibration();		// �� ���� �ð��� �������� �����.

	for (int i = 0; i < 4; i++)
	{
		XInputGetState(i, &m_GamePadState[i]);	// ���� ���� ������
	}
}

bool XboxPadInput::GetKeyDown(int padIndex, int key)
{
	if (m_GamePadState[padIndex].Gamepad.wButtons & key)
	{
		// X ��ư�� �������� �� ��
		//DoVibration();

		return true;
	}

	return false;
}



SHORT XboxPadInput::GetAxisX(int padIndex)
{
	return m_GamePadState[padIndex].Gamepad.sThumbLX;
}

SHORT XboxPadInput::GetAxisY(int padIndex)
{
	return m_GamePadState[padIndex].Gamepad.sThumbLY;
}

/// Ʈ���� �߰�����
SHORT XboxPadInput::GetLTrigger(int padIndex)
{
	return m_GamePadState[padIndex].Gamepad.bLeftTrigger;
}

SHORT XboxPadInput::GetRTrigger(int padIndex)
{
	return m_GamePadState[padIndex].Gamepad.bRightTrigger;
}
/// Ʈ���� �߰����� ��

void XboxPadInput::Vibrate(int padIndex, short leftMotorSpeed, short rightMotorSpeed)
{
	/// Ÿ�̸Ӹ� �־, ������ �����ϰ� ������ ���� ������ �Ѵ�.
	XINPUT_VIBRATION vibration;
	ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
	vibration.wLeftMotorSpeed = leftMotorSpeed;	// 0 ~ 65535 ������ ��
	vibration.wRightMotorSpeed = rightMotorSpeed;		// 0 ~ 65535 ������ ��

	XInputSetState(padIndex, &vibration);
}



void XboxPadInput::MakeLeftVibration(int padIndex, float time)
{
	m_PeroidForLeftVib[padIndex] = time;

	Vibrate(padIndex, 32767, 0);

	m_TimerForLeft[padIndex] = m_TimerForVibration->GetCurrentTimeSec();
}

void XboxPadInput::MakeRightVibration(int padIndex, float time)
{
	m_PeroidForRightVib[padIndex] = time;

	Vibrate(padIndex, 0, 32767);

	m_TimerForRight[padIndex] = m_TimerForVibration->GetCurrentTimeSec();
}

void XboxPadInput::MakeBothVibration(int padIndex, float time)
{
	m_PeroidForLeftVib[padIndex] = time;
	m_PeroidForRightVib[padIndex] = time;

	Vibrate(padIndex, 32767, 32767);

	m_TimerForLeft[padIndex] = m_TimerForVibration->GetCurrentTimeSec();
	m_TimerForRight[padIndex] = m_TimerForVibration->GetCurrentTimeSec();
}

/// Ÿ�̸Ӹ� �־, ������ �����ϰ� ������ ���� ������ �Ѵ�.
void XboxPadInput::CheckEndVibration()
{
 	for (int i = 0; i < 4; i++)
 	{
 		if (0 < m_PeroidForLeftVib[i] &&
			m_PeroidForLeftVib[i] <= m_TimerForVibration->GetCurrentTimeSec() - m_TimerForLeft[i])
 		{
 			Vibrate(i, 0, 0);

			m_PeroidForLeftVib[i] = -1.0f;
 		}
 
 		if (0 < m_PeroidForRightVib[i] &&
			m_PeroidForRightVib[i] <= m_TimerForVibration->GetCurrentTimeSec() - m_TimerForRight[i])
 		{
 			Vibrate(i, 0, 0);

			m_PeroidForRightVib[i] = -1.0f;
 		}
 	}
}

