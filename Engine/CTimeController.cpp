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
	// 1. �ػ� ���� �Լ�
	//LARGE_INTEGER _time = QueryPerformanceCounter();

	//_time.LowPart;
	//_time.HighPart;

	LARGE_INTEGER _Frequency;
	QueryPerformanceFrequency(&_Frequency);
	m_Frequency = _Frequency.QuadPart;

	// 2. ģ���� �Լ�
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
/// ���ϴ� FPS�� �ش��ϴ� �ð��� �����°�?
/// 60fps�� �����ϱ� ���� ���� �Լ�
/// </summary>
/// <returns></returns>
bool CTimeController::FPSCheck()
{

	// ���ݱ��� �ɸ� �ð��� ���.
	EndTimeStamp();
	CheckTime();


	//m_TargetElapsedTimeSec = 0.01667f
	if (m_TargetElapsedTimeSec * 1000.0f < GetElaspedTimeMS())
	{
		// .���� ���������� �� �� �ִ�.
		StartTimeStamp();

		return true;
	}

	return false;
}
