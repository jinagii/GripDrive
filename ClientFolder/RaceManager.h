#pragma once

#include "GlobalDefine.h"

#include "GameManager.h"

class D2DEngine;
class CAABB;
class CGameObject;
class CarMK3;

struct CheckInfo
{
	// map���� �ε��̵� �����̹Ƿ� int������ ���� ���������� �� �� �ִ�.
	int m_ReachedPlayer;
	BOOL m_IsPassed;

	float m_PassedTime;
	BOOL m_IsCalculated;	// �̹� �ջ��� �����ٸ� �� ��������

	BOOL m_PrevState;
	BOOL m_NowState;
};

struct ScoreInfo
{
	int m_CarIndex;			// �� ���ھ �����Ǿ� �ִ� ��
	int m_CheckCount;		// �� ���� üũ ����Ʈ�� �����ƴ���
	float m_TotalLapTime;	// �� Ÿ���� �� �ջ��� �� ����?
	float m_ScoreByTime;	// �ɸ� �ð����� ���� ���� �������� ����
	int m_NowLap;			// ���� �÷��̾ ������ ����
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

	// ���� �����ߴ����� �� üũ ����Ʈ���� �˰� �־�� �Ѵ�.
	// �� üũ ����Ʈ�� �ڵ����� ������ŭ�� boolean�� ������ �־�� �Ѵ�.
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
	// �÷��̾���� �ε����� mapping ���ش�.
	map<int, CarMK3*> m_PlayerIndex;	
	
	// �ε��̵� ������ �������� mapping ���ش�.
	map<int, ScoreInfo*> m_ScoreInfo;
	
	// üũ ����Ʈ�� ���� �������� ������ �ִ´�.
	// ����ؼ� ���� üũ�� ����� �ϹǷ�
	vector<CheckPoint*> v_CheckPoint;
	
	// ���Ϳ� ����ִ� ������ �ٷ� ������.

	int m_MaxPlayer;
	float m_RunningTime;

	int m_TotalLap;

	// ������ �����ϰ� �Ǵ� ������
	int m_Winner;
	
public:
	vector<int> v_Rank;
	// ���� ����� �� ��Ȱ���� ���� Initializer�� �����
	void Initialize(vector<CarMK3*> objs, int totalLap, eGameTrack track);

	// ���� üũ ����Ʈ�鿡 �����ߴ����� ��� �˻����ִ� �Լ�
	void CheckReached(float deltaTime);

	// ���� ���� �� �������� �˷��ִ� �Լ�
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