#include "windows.h"
#include "CTimeController.h"


CTimeController::CTimeController()
{

}

CTimeController::~CTimeController()
{

}

void CTimeController::Initialize()
{
	// 1. 해상도 높은 함수
	//LARGE_INTEGER _time = QueryPerformanceCounter();

	//_time.LowPart;
	//_time.HighPart;

	LARGE_INTEGER _Frequency;
	QueryPerformanceFrequency(&_Frequency);
	m_Frequency = _Frequency.QuadPart;

	// 2. 친숙한 함수
	//GetTickCount();


	m_FPS = 60.0f;
	m_TargetElapsedTimeSec = 1.0f / m_FPS;
}

void CTimeController::StartTimeStamp()
{
	QueryPerformanceCounter(&m_StartTimeStamp);
}

void CTimeController::EndTimeStamp()
{
	QueryPerformanceCounter(&m_EndTimeStamp);
}

void CTimeController::CheckTime()
{
	m_ElaspedTime = m_EndTimeStamp.QuadPart - m_StartTimeStamp.QuadPart;
}

float CTimeController::GetElaspedTimeMS() const
{
	return (float)((float)m_ElaspedTime / (float)m_Frequency) * 1000.f;
}

float CTimeController::GetElapsedTimeSec() const
{
	return (float)((float)m_ElaspedTime / (float)m_Frequency);
}


/// <summary>
/// 원하는 FPS에 해당하는 시간이 지났는가?
/// 60fps을 유지하기 위한 삽질 함수
/// </summary>
/// <returns></returns>
bool CTimeController::FPSCheck()
{

	// 지금까지 걸린 시간을 잰다.
	EndTimeStamp();
	CheckTime();


	//m_TargetElapsedTimeSec = 0.01667f
	if (m_TargetElapsedTimeSec * 1000.0f < GetElaspedTimeMS())
	{
		// .다음 프레임으로 갈 수 있다.
		StartTimeStamp();

		return true;
	}

	return false;
}
