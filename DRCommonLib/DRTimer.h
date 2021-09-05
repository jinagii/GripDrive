#pragma once

#include <windows.h>

/// <summary>
/// �ð� ���� Ŭ����
/// 
/// ������ ����, 1�����Ӵ� �ɸ� �ð� ��
/// 
/// [1/19/2021 LeHideOffice]
/// </summary>
class DRTimer
{
public:
	DRTimer();
	~DRTimer();

	// Quert
private:
	__int64 m_CountsPerSec;
	double m_SecondsPerCount;

	__int64 m_PreviousCount;
	__int64 m_CurrentCount;
	__int64 m_DeltaCount;
	double m_DeltaTime;

public:
	float GetDeltaTimeMS();
	float GetFPS();

	float GetCurrentTimeSec();

	void Update();


	/// �߰� �� ��:
	/// FPS���� ��� ��/����
	/// ����/����
	/// freeze�Ǿ��ٰ� �ٽ� �����ϴ� ��쿡 ���� ó��
	/// ������ �ð�ó��
};

