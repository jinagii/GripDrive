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


	// 각각의 체크 포인트는 자동차 개수만큼의 자료를 가지고 있어야 한다.
	for (int i = 0; i < carCount; i++)
	{
		CheckInfo* _info = new CheckInfo();

		// 처음에는 모두 통과하지 않은 상태로 한다.
		_info->m_ReachedPlayer = -1;				// 인덱싱 값을 넘어서는 매직넘버
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

	// 각각의 체크 포인트는 자동차 개수만큼의 자료를 가지고 있어야 한다.
	for (int i = 0; i < carCount; i++)
	{
		CheckInfo* _info = new CheckInfo();

		// 처음에는 모두 통과하지 않은 상태로 한다.
		_info->m_ReachedPlayer = -1;				// 인덱싱 값을 넘어서는 매직넘버
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

	// 해당 체크 포인트에 도달한 차가 또 도달했다면 끝내라.

	// 해당 체크 포인트의 정보를 이미 다른 차가 건드렸다면 다른 체크 포인트 정보에 접근
	if (_info->m_IsPassed) return;

	_info->m_ReachedPlayer = passedPlayer;
	_info->m_IsPassed = true;
	_info->m_PassedTime = checkTime;
	_info->m_IsCalculated = false;

	// 값 변경이 끝났다면 끝내준다.

	// 체크포인트에 도달한 순간 플레이어가 인덱싱된 int값으로 정보를 넣어준다.
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
		// 밟았다가 떨어졌는데 다시 밟았다?
		// 여기에 현재 밟은 시간과 지난 번에 밟은 시간의 차를 구한다.
		if (_info->m_IsPassed && 10.0f < (runTime - _info->m_PassedTime))
		{
			// 역주행이다!
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

	// int로 쉽게 접근하기 위해 MaxPlayer를 설정했다.
	m_MaxPlayer = objs.size();

	// 모두 몇 바퀴를 돌아야하는가?
	m_TotalLap = totalLap;

	// 차가 생성된 만큼 차를  m_PlayerIndex에 넣어 인덱싱해준다.

	for (unsigned int i = 0; i < objs.size(); i++)
	{
		// 1. 플레이어와 index를 매핑해준다.
		int _index = i;
		m_PlayerIndex.insert({ _index, objs[i] });

		// 2. 플레이어와 순위를 매길 정보인 ScoreInfo를 매핑한다.
		ScoreInfo* _score = new ScoreInfo();
		_score->m_CheckCount = 0;
		_score->m_TotalLapTime = 0.0f;
		_score->m_CarIndex = _index;

		// 처음 시작은 매직넘버로 시작한다.
		_score->m_ScoreByTime = 999999.0f;
		m_ScoreInfo.insert({ _index, _score });

		// 플레이어들을 임의로 랭크 정보에 집어넣는다.
		// 시작 시 1번 플레이어는 1등을 차지한다.
		v_Rank.push_back(i);
	}


	switch (track)
	{

	case eGameTrack::eTrack1:
	{
		// 체크포인트가 어디에 만들어질지 미리 알고 있어야 한다.
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

	// 도달했는지 계속 체크해준다.
	for (int i = 0; i < v_CheckPoint.size(); i++)
	{
		CheckPoint* _checkPoint = v_CheckPoint[i];

		CAABB* _aabb = _checkPoint->GetAABB();

		for (int carIndex = 0; carIndex < m_MaxPlayer; carIndex++)
		{
			// 맵의 key를 통해 값을 넘긴다.
			CarMK3* _target = m_PlayerIndex.at(carIndex);

			// 도달했다면 어떤 차가, 언제 도착했는지를 Map에 넣어준다.
			bool _isReached = _aabb->CollisionDetection(_target->GetCollider());

			_checkPoint->CheckingUpdate(carIndex, _isReached, m_RunningTime);
			if (_isReached)
			{

				// 현재 검사하려는 지점의 전에 체크 포인트가 존재하면
				// 아래는 이전 체크포인트를 건너뛸 경우 체크가 안되게 하기 위함이다.
				if (i - 1 >= 0)
				{
					CheckInfo* _checkInfo = v_CheckPoint[i - 1]->GetCheckPointInfo(carIndex);

					if (_checkInfo->m_IsPassed)
					{
						_checkPoint->SetCheckInfo(carIndex, m_RunningTime);


						// 만약 도달한 지점이 마지막 지점이라면 초기화 해준다.
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

	// 순위 산정 방식
	// 가장 많은 l_CheckPoint의 노드를 차지하거나
	// 해당 노드들의 m_LapTime의 합산이 가장 적은 수를 검사한다.
	// 만약, 노드가 가장 많으면서도 Laptime이 짧다면 그 차가 1위다.
	// CheckPoint를 모두 돌면서 위 사항을 체크해준다.

	// 1. 현재 노드 중에 도달한 플레이어가 있는가?
	// 2. 도달했다면, 체크 PointCount와 Laptime을 더 해준다.
	for (int i = 0; i < v_CheckPoint.size(); i++)
	{
		CheckPoint* _checkPoint = v_CheckPoint[i];

		for (int carIndex = 0; carIndex < m_MaxPlayer; carIndex++)
		{
			CheckInfo* _info = _checkPoint->GetCheckPointInfo(carIndex);

			if (_info->m_IsCalculated) continue;

			// 1번을 처리한다.
			if (_info->m_ReachedPlayer == carIndex)
			{
				ScoreInfo* _score = m_ScoreInfo.at(carIndex);

				float _laptime = _info->m_PassedTime;

				// 2번을 처리한다.
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
	// m_ScoreInfod의 데이터를 정렬한다. 정렬 후, 정렬된 상태의 인덱스가 순위이다.
	//std::sort(m_ScoreInfo.begin(), m_ScoreInfo.end(), IsGreaterThan_Score);

	std::vector<ScoreInfo*> _rankList;
	_rankList.push_back(m_ScoreInfo[0]);
	_rankList.push_back(m_ScoreInfo[1]);
	_rankList.push_back(m_ScoreInfo[2]);
	_rankList.push_back(m_ScoreInfo[3]);

	std::sort(_rankList.begin(), _rankList.end(), IsGreaterThan_Score);


	// 플레이어 맵을 돌면서, 맞는 스코어가 있으면 순위 부여
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
	// 내부 요소 지우는 방법을 강구할 것

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
	// 끝 부분에 도달한 것이므로 여기서 현재 Lap수를 늘려준다
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
