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

	// 바퀴 마다 그려지는 Trail을 다르게 하기 위해 offset을 도입했다.
	void CheckPosition(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F offset, float angle);
	void CheckPosition(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F offset, float angle, BOOL isMark);

	void DrawTrail(D2DEngine* pEngine);
	void DrawTrailByBool(D2DEngine* pEngine);

	// 리스트에 이전 값들을 넣어두고, 현재 위치로 바로 그려낸다.
	// 이 경우, for문을 돌때 1번 위치와 현재 위치를 직선 형태로 그려내기 때문에 문제가 발생했다.
	void DrawTrail(D2DEngine* pEngine, D2D1_VECTOR_2F _nowPos);
	void ResetTrail();
};

