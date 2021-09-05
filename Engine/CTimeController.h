#pragma once



/// <summary>
/// 시간 관련 클래스
/// 2020.11.13
/// </summary>

class CTimeController
{
public:
	CTimeController();
	~CTimeController();



public:
	void Initialize();

	void StartTimeStamp();
	void EndTimeStamp();


	void CheckTime();
	float GetElaspedTimeMS() const;

	float GetElapsedTimeSec() const;
	bool FPSCheck();


private:
	__int64 m_Frequency;

	LARGE_INTEGER m_StartTimeStamp;
	LARGE_INTEGER m_EndTimeStamp;

	__int64 m_ElaspedTime;

	float m_FPS;						// 원하는 FPS(ex.60)
	float m_TargetElapsedTimeSec;			// 도달해야하는 시간 (ex. 16.7ms)

	// FPS 제어
	// 특정 구간의 걸린 시간을 잰다


};

