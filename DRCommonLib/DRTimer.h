#pragma once

#include <windows.h>

/// <summary>
/// 시간 관련 클래스
/// 
/// 프레임 제어, 1프레임당 걸린 시간 등
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


	/// 추가 할 것:
	/// FPS제어 기능 온/오프
	/// 포즈/리줌
	/// freeze되었다가 다시 가동하는 경우에 대한 처리
	/// 엄밀한 시간처리
};

