#pragma once

#include "GlobalDefine.h"
#include <vector>
#include <list>

using namespace std;

class D2DEngine;


struct TrailInfo
{
	D2D1_POINT_2F m_Point;
	BOOL m_IsMark;
	float m_Opacity;
	
};


class Trail
{
public:
	Trail();
	~Trail();

private:
	list<D2D1_POINT_2F> l_Point;
	list<TrailInfo*> l_TrailInfo;

	D2D1_VECTOR_2F m_NowPos;
	D2D1_VECTOR_2F m_PrevPos;

public:

	// ���� ���� �׷����� Trail�� �ٸ��� �ϱ� ���� offset�� �����ߴ�.
	void CheckPosition(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F offset, float angle);
	void CheckPosition(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F offset, float angle, BOOL isMark);

	void DrawTrail(D2DEngine* pEngine);
	void DrawTrailByBool(D2DEngine* pEngine);

	// ����Ʈ�� ���� ������ �־�ΰ�, ���� ��ġ�� �ٷ� �׷�����.
	// �� ���, for���� ���� 1�� ��ġ�� ���� ��ġ�� ���� ���·� �׷����� ������ ������ �߻��ߴ�.
	void DrawTrail(D2DEngine* pEngine, D2D1_VECTOR_2F _nowPos);
	void ResetTrail();
};

