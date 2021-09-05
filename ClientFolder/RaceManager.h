#pragma once

#include "GlobalDefine.h"

#include "GameManager.h"

class D2DEngine;
class CAABB;
class CGameObject;
class CarMK3;

struct CheckInfo
{
	// map으로 인덱싱된 상태이므로 int만으로 누가 지나갔는지 알 수 있다.
	int m_ReachedPlayer;
	BOOL m_IsPassed;

	float m_PassedTime;
	BOOL m_IsCalculated;	// 이미 합산이 끝났다면 또 하지마라

	BOOL m_PrevState;
	BOOL m_NowState;
};

struct ScoreInfo
{
	int m_CarIndex;			// 이 스코어가 연동되어 있는 차
	int m_CheckCount;		// 몇 개의 체크 포인트를 지나쳤는지
	float m_TotalLapTime;	// 랩 타임의 총 합산이 몇 인지?
	float m_ScoreByTime;	// 걸린 시간으로 낮은 수가 나오도록 유도
	int m_NowLap;			// 현재 플레이어가 도달한 바퀴
};

class CheckPoint
{
public:
	CheckPoint();
	CheckPoint(D2D1_VECTOR_2F pos, int gridSize, int carCount);
	CheckPoint(D2D1_VECTOR_2F pos, int gridSize, int carCount, int index, float angle);
	~CheckPoint();

private:
	int m_Index;
	CAABB* m_AABB;

	// 차가 도착했는지를 각 체크 포인트별로 알고 있어야 한다.
	// 각 체크 포인트는 자동차의 개수만큼의 boolean을 가지고 있어야 한다.
	vector<CheckInfo*> v_CheckInfo;

public:
	int m_ReverseDriver;
	float m_Angle;

	CAABB* GetAABB() { return m_AABB; }
	void SetCheckInfo(int passedPlayer, float checkTime);
	CheckInfo* GetCheckPointInfo(int index) { return v_CheckInfo[index]; }
	
	void CheckingUpdate(int carIndex, bool isReached, float runTime);
	void ReverseRun(int carIndex);
	int GetIndex() { return m_Index; }


};

class RaceManager
{
public:
	RaceManager();
	~RaceManager();

private:
	// 플레이어들을 인덱싱해 mapping 해준다.
	map<int, CarMK3*> m_PlayerIndex;	
	
	// 인덱싱된 정수에 점수판을 mapping 해준다.
	map<int, ScoreInfo*> m_ScoreInfo;
	
	// 체크 포인트에 대한 정보들을 가지고 있는다.
	// 계속해서 도달 체크를 해줘야 하므로
	vector<CheckPoint*> v_CheckPoint;
	
	// 벡터에 들어있는 순서가 바로 순위다.

	int m_MaxPlayer;
	float m_RunningTime;

	int m_TotalLap;

	// 게임을 종료하게 되는 변수다
	int m_Winner;
	
public:
	vector<int> v_Rank;
	// 게임 재시작 시 재활용을 위해 Initializer를 만든다
	void Initialize(vector<CarMK3*> objs, int totalLap, eGameTrack track);

	// 차가 체크 포인트들에 도달했는지를 계속 검사해주는 함수
	void CheckReached(float deltaTime);

	// 차가 지금 몇 위인지를 알려주는 함수
	void CalculateLapTime();
	void CalculateRank();

	void DrawCheckPoint(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);
	void ShowScoreInfo(D2DEngine* pEngine);
	void ShowRank(D2DEngine* pEngine);

	void ResetCheckPoint(int carIndex);
	void ResetRaceManager();
	void IsFinish(int carIndex);

	int GoToResult() { return m_Winner; }

	void GetReverseDriver(int carIndex, int checkPointIndext);
};