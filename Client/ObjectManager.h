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

/// ������Ʈ �Ŵ���
///
/// 2021.02.22 LeHideAcademy
class ObjectManager
{
public:
	ObjectManager();
	ObjectManager(GameManager* gameManager);
	~ObjectManager();

	// �ε� �� ������ ������ ����� �� ���� �־�ý��ϴ�.
	D2DEngine* m_pEngine;
	GameManager* m_GM;

	// json���κ��� �����͸� �о ������Ʈ���� new


public:
	void Initialize(eGameTrack track);
	CGameObject* CreateCar(D2DEngine* pEngine, int carIndex, D2D1_VECTOR_2F pos);
	void CreateModelObjectByName(D2D1_VECTOR_2F pos, eObject1Name name);
	void CreateModelObjectByName(D2D1_VECTOR_2F pos, eObject2Name name);
	void CreateDummyObject(D2D1_VECTOR_2F centerPos, float width, float height, float angle);


	/// ���� ������Ʈ��. �������� ���ؼ� �ϰ� ó�� �Ѵ�.
	// ������ ������ ���� �ʴ� ��� (���ΰ� ó��, ���� ó��)�� ���� �޾Ƶΰų� ��Ÿ�� �� ���̳��� ĳ������ �Ѵ�.
private:
	vector<CGameObject*> m_vec_AllObjects;	// ���� ��� ������Ʈ�� �ѹ濡 ó���Ѵ�.
	vector<CarMK3*> v_Cars;					// �̰��� �ߺ����� ������ �ִ� ����. 1~4 �÷��̾ ���� �����ϱ� ���� ���̴�.
	

	vector<ID2D1Bitmap*> v_SpriteType;

	ID2D1Bitmap* m_TotalProb;


	bool m_IsPlay;

	vector<CarMK3*> v_MapCar;

	CarMK3* m_Player1;
	CarMK3* m_Player2;
	CarMK3* m_Player3;
	CarMK3* m_Player4;

	


public:
	/// �ʱ�ȭ ����
	// ���̽� �Ŵ����� ������ ������ �˰� �־���ϹǷ� ������Ʈ �Ŵ����� ��� �������ش�.
	RaceManager* RaceManagerInit(int checkpointCount);
	

	/// ������Ʈ ����
	void UpdateAll(float dTime);

	/// �浹���� ����
	void CollisionCheckAll();

	/// car ����
	CarMK3* GetCars(int index);
	



public:
	/// ��ο� ����
	void RenderAll(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);
	void RenderDebug(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);

	/// Y�� ����
	void SortingByAxisY();

	/// ����� ����
	void DrawTransformInfo();
	void DrawVectorInfo(D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);
	void DrawCarsVariable();

	/// Ű ������Ʈ ����
	void InputUpdate();
	void KeyMapping();

	/// ���̽� �غ� ���� ����
	void MakeCarReady(eGameTrack track);

	/// ����
	void ResetObject();
};

