#include "DRTimer.h"

DRTimer::DRTimer()
	: m_SecondsPerCount(0.0), m_DeltaCount(0), m_PreviousCount(0), m_CurrentCount(0)
	, m_DeltaTime(0)
{
	// ī���� üũ. 1�ʴ� �� ���� ���� ���ΰ�?
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_CountsPerSec);

	// 1ī��Ʈ�� �� �ʳ� �ɸ���?
	m_SecondsPerCount = 1.0 / (double)m_CountsPerSec;
}

DRTimer::~DRTimer()
{

}

float DRTimer::GetDeltaTimeMS()
{
	m_DeltaTime = m_DeltaCount * m_SecondsPerCount;

	// ���� double���� float�� �߸����� �װ��� �߿��� �Լ��� �ƴϴ�.
	// �и�������� ��ȯ �� �ش�.
	return (float)m_DeltaTime * 1000.0f;
}

float DRTimer::GetFPS()
{
	if (m_DeltaTime != 0)
	{
		return 1.0f / m_DeltaTime;
	}
	return 0;
}

float DRTimer::GetCurrentTimeSec()
{
	return (float)m_CurrentCount * m_SecondsPerCount;
}

void DRTimer::Update()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentCount);

	// Time difference between this frame and the previous.
	m_DeltaCount = (m_CurrentCount - m_PreviousCount);

	// ī��Ʈ�� �ð��� �и��Ѵ�.
	m_DeltaTime = m_DeltaCount * m_SecondsPerCount;

	// Prepare for next frame.
	m_PreviousCount = m_CurrentCount;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (m_DeltaCount < 0.0)
	{
		m_DeltaCount = 0;
	}
}
