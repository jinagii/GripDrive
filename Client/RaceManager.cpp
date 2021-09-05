#include "RaceManager.h"
#include "D2DEngine.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CarMK3.h"
#include "CAABB.h"
#include <algorithm>
#include <list>

CheckPoint::CheckPoint()
{

}

CheckPoint::CheckPoint(D2D1_VECTOR_2F pos, int gridSize, int carCount)
{
	D2D1_RECT_F _rect;

	_rect.left = pos.x;
	_rect.top = pos.y;
	_rect.right = _rect.left + gridSize;
	_rect.bottom = _rect.top + gridSize;

	m_AABB = new CAABB(_rect);


	// ������ üũ ����Ʈ�� �ڵ��� ������ŭ�� �ڷḦ ������ �־�� �Ѵ�.
	for (int i = 0; i < carCount; i++)
	{
		CheckInfo* _info = new CheckInfo();

		// ó������ ��� ������� ���� ���·� �Ѵ�.
		_info->m_ReachedPlayer = -1;				// �ε��� ���� �Ѿ�� �����ѹ�
		_info->m_PassedTime = 0;
		_info->m_IsPassed = false;
		_info->m_PrevState = false;
		_info->m_NowState = false;

		v_CheckInfo.push_back(_info);
	}

}

CheckPoint::CheckPoint(D2D1_VECTOR_2F pos, int gridSize, int carCount, int index, float angle)
{
	D2D1_RECT_F _rect;

	_rect.left = pos.x;
	_rect.top = pos.y;
	_rect.right = _rect.left + gridSize;
	_rect.bottom = _rect.top + gridSize;

	m_AABB = new CAABB(_rect);
	m_Index = index;
	m_Angle = angle;
	m_ReverseDriver = -1;

	// ������ üũ ����Ʈ�� �ڵ��� ������ŭ�� �ڷḦ ������ �־�� �Ѵ�.
	for (int i = 0; i < carCount; i++)
	{
		CheckInfo* _info = new CheckInfo();

		// ó������ ��� ������� ���� ���·� �Ѵ�.
		_info->m_ReachedPlayer = -1;				// �ε��� ���� �Ѿ�� �����ѹ�
		_info->m_IsPassed = false;
		_info->m_PassedTime = 0;

		v_CheckInfo.push_back(_info);
	}
}

CheckPoint::~CheckPoint()
{
	/*
	for (int i = 0; i < v_CheckInfo.size(); i++)
	{
		delete v_CheckInfo[i];
	}

	v_CheckInfo.clear();
	*/
}

void CheckPoint::SetCheckInfo(int passedPlayer, float checkTime)
{
	CheckInfo* _info = v_CheckInfo[passedPlayer];

	// �ش� üũ ����Ʈ�� ������ ���� �� �����ߴٸ� ������.

	// �ش� üũ ����Ʈ�� ������ �̹� �ٸ� ���� �ǵ�ȴٸ� �ٸ� üũ ����Ʈ ������ ����
	if (_info->m_IsPassed) return;

	_info->m_ReachedPlayer = passedPlayer;
	_info->m_IsPassed = true;
	_info->m_PassedTime = checkTime;
	_info->m_IsCalculated = false;

	// �� ������ �����ٸ� �����ش�.

	// üũ����Ʈ�� ������ ���� �÷��̾ �ε��̵� int������ ������ �־��ش�.
	/*
	for (int carIndex = 0; carIndex < v_CheckInfo.size(); carIndex++)
	{

	}
	*/
}

void CheckPoint::CheckingUpdate(int carIndex, bool isReached, float runTime)
{
	CheckInfo* _info = v_CheckInfo[carIndex];

	_info->m_NowState = isReached;

	if (_info->m_NowState != _info->m_PrevState)
	{
		// ��Ҵٰ� �������µ� �ٽ� ��Ҵ�?
		// ���⿡ ���� ���� �ð��� ���� ���� ���� �ð��� ���� ���Ѵ�.
		if (_info->m_IsPassed && 10.0f < (runTime - _info->m_PassedTime))
		{
			// �������̴�!
			m_ReverseDriver = carIndex;
			_info->m_PassedTime = runTime;
		}
	}

	_info->m_PrevState = _info->m_NowState;
}

void CheckPoint::ReverseRun(int carIndex)
{
	m_ReverseDriver = carIndex;
}

RaceManager::RaceManager()
	:m_RunningTime(0),
	m_Winner(-1)

{
	D2DEngine* pEngine = D2DEngine::Instance();

	//Initialize(objs, totalLap);
}

RaceManager::~RaceManager()
{

}

void RaceManager::Initialize(vector<CarMK3*> objs, int totalLap, eGameTrack track)
{

	ResetRaceManager();
	m_RunningTime = 0;
	m_Winner = -1;

	// int�� ���� �����ϱ� ���� MaxPlayer�� �����ߴ�.
	m_MaxPlayer = objs.size();

	// ��� �� ������ ���ƾ��ϴ°�?
	m_TotalLap = totalLap;

	// ���� ������ ��ŭ ����  m_PlayerIndex�� �־� �ε������ش�.

	for (unsigned int i = 0; i < objs.size(); i++)
	{
		// 1. �÷��̾�� index�� �������ش�.
		int _index = i;
		m_PlayerIndex.insert({ _index, objs[i] });

		// 2. �÷��̾�� ������ �ű� ������ ScoreInfo�� �����Ѵ�.
		ScoreInfo* _score = new ScoreInfo();
		_score->m_CheckCount = 0;
		_score->m_TotalLapTime = 0.0f;
		_score->m_CarIndex = _index;

		// ó�� ������ �����ѹ��� �����Ѵ�.
		_score->m_ScoreByTime = 999999.0f;
		m_ScoreInfo.insert({ _index, _score });

		// �÷��̾���� ���Ƿ� ��ũ ������ ����ִ´�.
		// ���� �� 1�� �÷��̾�� 1���� �����Ѵ�.
		v_Rank.push_back(i);
	}


	switch (track)
	{

	case eGameTrack::eTrack1:
	{
		// üũ����Ʈ�� ��� ��������� �̸� �˰� �־�� �Ѵ�.
		D2D1_VECTOR_2F arr_Vector[14] =
		{
			{700,  400 },
			{1100, 950 },
			{1380, 1450},
			{1860, 1730},
			{1610, 1180},
			{1400, 600 },
			{2600, 640 },
			{2500, 1000},
			{1800, 888 },
			{2250, 1600},
			{2800, 1500},
			{2900, 380 },
			{2400, 230 },
			{1120, 230 },
		};


		float arr_Angle[14] =
		{
			180,
			135,
			180,
			0  ,
			315,
			0  ,
			180,
			225,
			180,
			135,
			0  ,
			0  ,
			270,
			270,
		};

		for (int i = 0; i < 14; i++)
		{
			CheckPoint* _checkInfo = new CheckPoint(arr_Vector[i], 150, 4, i, arr_Angle[i]);
			v_CheckPoint.push_back(_checkInfo);
		}

		break;

	}
	case eGameTrack::eTrack2:
	{
		D2D1_VECTOR_2F arr_Vector[14] =
		{
			{1300, 500 },
			{2400, 1500},
			{1300, 500 },
			{2400, 1500},
			{1300, 500 },
			{2400, 1500},
			{1300, 500 },
			{2400, 1500},
			{1300, 500 },
			{2400, 1500},
			{1300, 500 },
			{2400, 1500},
			{1300, 500 },
			{2400, 1500},
		};

		float arr_Angle[14] =
		{
			0,
			180,
			0  ,
			180,
			0  ,
			180,
			0,
			180,
			0  ,
			180,
			0  ,
			180,
			0,
			180,
		};

		for (int i = 0; i < 14; i++)
		{
			CheckPoint* _checkInfo = new CheckPoint(arr_Vector[i], 150, 4, i, arr_Angle[i]);
			v_CheckPoint.push_back(_checkInfo);
		}

		break;
	}

	}


}

void RaceManager::CheckReached(float deltaTime)
{
	m_RunningTime += deltaTime;

	// �����ߴ��� ��� üũ���ش�.
	for (int i = 0; i < v_CheckPoint.size(); i++)
	{
		CheckPoint* _checkPoint = v_CheckPoint[i];

		CAABB* _aabb = _checkPoint->GetAABB();

		for (int carIndex = 0; carIndex < m_MaxPlayer; carIndex++)
		{
			// ���� key�� ���� ���� �ѱ��.
			CarMK3* _target = m_PlayerIndex.at(carIndex);

			// �����ߴٸ� � ����, ���� �����ߴ����� Map�� �־��ش�.
			bool _isReached = _aabb->CollisionDetection(_target->GetCollider());

			_checkPoint->CheckingUpdate(carIndex, _isReached, m_RunningTime);
			if (_isReached)
			{

				// ���� �˻��Ϸ��� ������ ���� üũ ����Ʈ�� �����ϸ�
				// �Ʒ��� ���� üũ����Ʈ�� �ǳʶ� ��� üũ�� �ȵǰ� �ϱ� �����̴�.
				if (i - 1 >= 0)
				{
					CheckInfo* _checkInfo = v_CheckPoint[i - 1]->GetCheckPointInfo(carIndex);

					if (_checkInfo->m_IsPassed)
					{
						_checkPoint->SetCheckInfo(carIndex, m_RunningTime);


						// ���� ������ ������ ������ �����̶�� �ʱ�ȭ ���ش�.
						if (i == v_CheckPoint.size() - 1)
						{
							ResetCheckPoint(carIndex);
							IsFinish(carIndex);
						}
					}
				}
				else
				{
					_checkPoint->SetCheckInfo(carIndex, m_RunningTime);
				}
			}
		}

		if (_checkPoint->m_ReverseDriver != -1)
		{
			GetReverseDriver(_checkPoint->m_ReverseDriver, i);

			_checkPoint->m_ReverseDriver = -1;
		}
	}
}

void RaceManager::CalculateLapTime()
{

	// ���� ���� ���
	// ���� ���� l_CheckPoint�� ��带 �����ϰų�
	// �ش� ������ m_LapTime�� �ջ��� ���� ���� ���� �˻��Ѵ�.
	// ����, ��尡 ���� �����鼭�� Laptime�� ª�ٸ� �� ���� 1����.
	// CheckPoint�� ��� ���鼭 �� ������ üũ���ش�.

	// 1. ���� ��� �߿� ������ �÷��̾ �ִ°�?
	// 2. �����ߴٸ�, üũ PointCount�� Laptime�� �� ���ش�.
	for (int i = 0; i < v_CheckPoint.size(); i++)
	{
		CheckPoint* _checkPoint = v_CheckPoint[i];

		for (int carIndex = 0; carIndex < m_MaxPlayer; carIndex++)
		{
			CheckInfo* _info = _checkPoint->GetCheckPointInfo(carIndex);

			if (_info->m_IsCalculated) continue;

			// 1���� ó���Ѵ�.
			if (_info->m_ReachedPlayer == carIndex)
			{
				ScoreInfo* _score = m_ScoreInfo.at(carIndex);

				float _laptime = _info->m_PassedTime;

				// 2���� ó���Ѵ�.
				_score->m_CheckCount++;
				_score->m_TotalLapTime += _laptime;
			}
			_info->m_IsCalculated = true;
		}
	}

	for (int i = 0; i < m_MaxPlayer; i++)
	{
		ScoreInfo* _score = m_ScoreInfo.at(i);

		if (_score->m_TotalLapTime <= 0) continue;

		float _combination = 1 / _score->m_TotalLapTime;
		int _devide = _score->m_CheckCount + 1;

		_score->m_ScoreByTime = (100 / _devide) / _combination;
	}

}

void Swap(int& a, int& b)
{
	int _temp = a;
	a = b;
	b = _temp;
}

bool IsGreaterThan_Score(ScoreInfo* a, ScoreInfo* b)
{
	return a->m_CheckCount > b->m_CheckCount;
}

void RaceManager::CalculateRank()
{
	// m_ScoreInfod�� �����͸� �����Ѵ�. ���� ��, ���ĵ� ������ �ε����� �����̴�.
	//std::sort(m_ScoreInfo.begin(), m_ScoreInfo.end(), IsGreaterThan_Score);

	std::vector<ScoreInfo*> _rankList;
	_rankList.push_back(m_ScoreInfo[0]);
	_rankList.push_back(m_ScoreInfo[1]);
	_rankList.push_back(m_ScoreInfo[2]);
	_rankList.push_back(m_ScoreInfo[3]);

	std::sort(_rankList.begin(), _rankList.end(), IsGreaterThan_Score);


	// �÷��̾� ���� ���鼭, �´� ���ھ ������ ���� �ο�
	int _rankIndex = 0;
	for (ScoreInfo* score : _rankList)
	{
 		m_PlayerIndex[score->m_CarIndex]->SetRankIndex(_rankIndex);
		_rankIndex++;
	}
}

void RaceManager::DrawCheckPoint(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
	for (CheckPoint* _checkPoint : v_CheckPoint)
	{
		D2D1_RECT_F _rect = _checkPoint->GetAABB()->GetRectPosition();

		pEngine->SetTransform(CTransform::MatrixTranslation(D2D1::Vector2F(_rect.left, _rect.top)) * projTM * viewTM);

		pEngine->DrawRectangle_PreSetCol(
			0,
			0,
			150,
			150,
			RED_COLOR);

		pEngine->DrawTextW(10, 10, L"%d", _checkPoint->GetIndex());
	}
}

void RaceManager::ShowScoreInfo(D2DEngine* pEngine)
{
	D2D1_POINT_2F _point = D2D1::Point2F(1400, 10);

	for (int i = 0; i < m_MaxPlayer; i++)
	{
		ScoreInfo* _score = m_ScoreInfo.at(i);

		int _reachedCount = _score->m_CheckCount;
		float _totallaptime = _score->m_TotalLapTime;

		pEngine->SetTransform(D2D1::Matrix3x2F::Identity());

		pEngine->DrawText(_point.x, _point.y + i * 16, L"Player %d ||", i);
		pEngine->DrawText(_point.x + 150, _point.y + i * 16, L"Reached Point = %d", _reachedCount);
		pEngine->DrawText(_point.x + 400, _point.y + i * 16, L"Total Lap Time = %f", _totallaptime);
	}
}

void RaceManager::ShowRank(D2DEngine* pEngine)
{

	for (int i = 0; i < m_MaxPlayer; i++)
	{
		CarMK3* _car = m_PlayerIndex.at(v_Rank[i]);




	}


}


void RaceManager::ResetCheckPoint(int carIndex)
{
	for (CheckPoint* _checkPoint : v_CheckPoint)
	{
		_checkPoint->GetCheckPointInfo(carIndex)->m_IsPassed = false;
	}
}

void RaceManager::ResetRaceManager()
{
	// ���� ��� ����� ����� ������ ��

	m_PlayerIndex.clear();
	m_ScoreInfo.clear();
	v_CheckPoint.clear();
	v_Rank.clear();
}

void RaceManager::IsFinish(int carIndex)
{
	ScoreInfo* _score = m_ScoreInfo.at(carIndex);
	_score->m_NowLap++;

	if (_score->m_NowLap >= m_TotalLap)
	{
		m_Winner = carIndex;
	}
	// �� �κп� ������ ���̹Ƿ� ���⼭ ���� Lap���� �÷��ش�
}

void RaceManager::GetReverseDriver(int carIndex, int checkPointIndext)
{
	/*
	CarMK3* _car = m_PlayerIndex.at(carIndex);

	D2D1_VECTOR_2F _CheckPos = v_CheckPoint[checkPointIndext]->GetAABB()->GetPosition();

	_CheckPos.x += 70;
	_CheckPos.y += 70;

	_car->ResetPositionByCheckPoint(_CheckPos, v_CheckPoint[checkPointIndext]->m_Angle);
	*/
}
