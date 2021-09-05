#pragma once

#include "GlobalDefine.h"
#include "ModelObject.h"
#include "GameManager.h"

class CGameObject;
class D2DEngine;
class CarMK3;
class ObjectManager;
class GameManager;
class RaceManager;
class CameraManager;
class CTransform;

/// 오브젝트 매니저
///
/// 2021.02.22 LeHideAcademy
class ObjectManager
{
public:
	ObjectManager();
	ObjectManager(GameManager* gameManager);
	~ObjectManager();

	// 로딩 시 꾸준히 엔진을 사용할 것 같아 넣어봤습니다.
	D2DEngine* m_pEngine;
	GameManager* m_GM;

	// json으로부터 데이터를 읽어서 오브젝트들을 new


public:
	void Initialize(eGameTrack track);
	CGameObject* CreateCar(D2DEngine* pEngine, int carIndex, D2D1_VECTOR_2F pos);
	void CreateModelObjectByName(D2D1_VECTOR_2F pos, eObject1Name name);
	void CreateModelObjectByName(D2D1_VECTOR_2F pos, eObject2Name name);
	void CreateDummyObject(D2D1_VECTOR_2F centerPos, float width, float height, float angle);


	/// 게임 오브젝트들. 다형성을 통해서 일괄 처리 한다.
	// 다형적 동작이 되지 않는 경우 (주인공 처리, 예외 처리)는 따로 받아두거나 런타임 시 다이내믹 캐스팅을 한다.
private:
	vector<CGameObject*> m_vec_AllObjects;	// 이제 모든 오브젝트를 한방에 처리한다.
	vector<CarMK3*> v_Cars;					// 이것은 중복으로 가지고 있는 숏컷. 1~4 플레이어를 따로 조작하기 위한 것이다.
	

	vector<ID2D1Bitmap*> v_SpriteType;

	ID2D1Bitmap* m_TotalProb;


	bool m_IsPlay;

	vector<CarMK3*> v_MapCar;

	CarMK3* m_Player1;
	CarMK3* m_Player2;
	CarMK3* m_Player3;
	CarMK3* m_Player4;

	


public:
	/// 초기화 관련
	// 레이스 매니저는 차량의 개수를 알고 있어야하므로 오브젝트 매니저가 대신 생성해준다.
	RaceManager* RaceManagerInit(int checkpointCount);
	

	/// 업데이트 관련
	void UpdateAll(float dTime);

	/// 충돌감지 관련
	void CollisionCheckAll();

	/// car 게터
	CarMK3* GetCars(int index);
	



public:
	/// 드로우 관련
	void RenderAll(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);
	void RenderDebug(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);

	/// Y축 소팅
	void SortingByAxisY();

	/// 디버그 관련
	void DrawTransformInfo();
	void DrawVectorInfo(D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);
	void DrawCarsVariable();

	/// 키 업데이트 관련
	void InputUpdate();
	void KeyMapping();

	/// 레이스 준비를 위한 세팅
	void MakeCarReady(eGameTrack track);

	/// 리셋
	void ResetObject();
};

