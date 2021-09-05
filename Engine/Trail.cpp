#include "Trail.h"

#include "D2DEngine.h"
#include "CTransform.h"

Trail::Trail()
	:m_PrevPos(D2D1::Vector2F()),
	m_NowPos(D2D1::Vector2F())
{

}

Trail::~Trail()
{

}

void Trail::CheckPosition(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F offset, float angle)
{
	// ������ ��ġ�� ���� ��ġ�� ���� �ʴٸ� list�� �־��ٰŴ�.
	if (m_PrevPos.x == _nowPos.x && m_PrevPos.y == _nowPos.y)
	{
		return;
	}
	m_PrevPos = _nowPos;

	// ���� ���� ���� nowPos, ���� �������� offset�� ������ ��
	D2D1_POINT_2F _newPos = CTransform::VectorRotation(_nowPos, offset, angle);

}

void Trail::CheckPosition(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F offset, float angle, BOOL isMark)
{
	if (m_PrevPos.x == _nowPos.x && m_PrevPos.y == _nowPos.y)
	{
		return;
	}
	m_PrevPos = _nowPos;

	D2D1_POINT_2F _newPos = CTransform::VectorRotation(_nowPos, offset, angle);

	TrailInfo* _info = new TrailInfo();
	_info->m_Point = _newPos;
	_info->m_IsMark = isMark;
	_info->m_Opacity = 0.8f;

	l_TrailInfo.push_back(_info);
}

void Trail::DrawTrail(D2DEngine* pEngine, const D2D1_VECTOR_2F _nowPos)
{
	if (l_Point.size() <= 2) return;

	//Auto�� ���� ����
	for (auto _iter : l_Point)
	{
		D2D1_POINT_2F _prevPos;

		_prevPos = _iter;

		pEngine->DrawLineForTrail(_prevPos, D2D1::Point2F(_nowPos.x, _nowPos.y));
	}

	// ���� ����Ʈ�� ����ִ� ��尡 �ʹ� Ŀ���� ������ ���ؿ��� �߶��ش�.
	if (l_Point.size() > 20)
	{
		l_Point.pop_front();
	}
}

void Trail::DrawTrail(D2DEngine* pEngine)
{
	if (l_Point.size() <= 2) return;

	//iter�� ���� ����
	list<D2D1_POINT_2F>::iterator iter;
	for (iter = l_Point.begin(); iter != l_Point.end();)
	{

		D2D1_POINT_2F _pos1 = *iter;
		++iter;
		if (iter == l_Point.end()) break;
		D2D1_POINT_2F _pos2 = *iter;

		if (abs(_pos1.x) - abs(_pos2.x) > 5.f || abs(_pos1.y) - abs(_pos2.y) > 5.f)
		{
			continue;
		}

		pEngine->DrawLineForTrail(_pos1, _pos2);

	}

	// ���� ����Ʈ�� ����ִ� ��尡 �ʹ� Ŀ���� ������ ���ؿ��� �߶��ش�.
	if (l_Point.size() > 100)
	{
		l_Point.pop_front();
	}
}

void Trail::ResetTrail()
{
	if (!l_TrailInfo.empty())
	{
		for (TrailInfo* _trail : l_TrailInfo)
		{
			delete _trail;
		}
	}

	l_TrailInfo.clear();

}

void Trail::DrawTrailByBool(D2DEngine* pEngine)
{
	if (l_TrailInfo.size() <= 2) return;

	//iter�� ���� ����
	list<TrailInfo*>::iterator iter;
	for (iter = l_TrailInfo.begin(); iter != l_TrailInfo.end();)
	{
		TrailInfo* _info1 = *iter;
		++iter;
		if (iter == l_TrailInfo.end()) break;
		TrailInfo* _info2 = *iter;

		if (_info2->m_IsMark)
		{
			pEngine->DrawLineForTrail(_info1->m_Point, _info2->m_Point, _info1->m_Opacity);
		}

		_info1->m_Opacity -= 0.005f;

	}

	// ���� ����Ʈ�� ����ִ� ��尡 �ʹ� Ŀ���� ������ ���ؿ��� �߶��ش�.
	if (l_TrailInfo.size() > 300)
	{
		TrailInfo* _temp = l_TrailInfo.front();

		l_TrailInfo.pop_front();

		delete _temp;
	}
}
