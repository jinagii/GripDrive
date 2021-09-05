#pragma once

#include "GlobalDefine.h"

class D2DEngine;
class CAMotion;
class CASprite;
class CTransform;
class ICollisionObject;

enum class eCollisionType
{
	CC,
	AABB,
	OBB

};

/// <summary>
/// ���� ���� �����ϴ� ������Ʈ
/// 
/// ����, �巳��, ��, ���, ����Ʈ, ����, Ʈ����
/// </summary>
class CGameObject
{
public:
	CGameObject();
	CGameObject(D2D1_RECT_F _rect);
	~CGameObject();
	

	
protected:
	// ���� ������Ʈ�� �ڱ� �ڽ��� �׷��� ���𰡸� ������ ���� �� �ִ�.
	ID2D1Bitmap* m_bitmap;
	CTransform* m_Transform;
	ICollisionObject* m_Collider;
	D2D1_VECTOR_2F m_bitmapCenter;		// ���� ��ġ�� ������� ��Ʈ���� �׸� ���� ������
										// �̸��׸� ��Ʈ���� ���� LocalPosition�� �ȴ�.


public:
	// �ʱ�ȭ
	virtual void Initialize(D2DEngine* pEngine);

	// ������Ʈ ����
	virtual void Update(float dTime);

	// �浹���� ����
	virtual void CollisionCheck(CGameObject* target);
	virtual void CollisionCheck2(CGameObject* target);

	virtual D2D1_VECTOR_2F GetVelocity();
	virtual void SetVelocity(D2D1_VECTOR_2F velocity);


	// �⺻������ ��ġ���� ������ �� ���� ���� ���� �Լ��� �����д�.
	virtual void MoveHorizontal(float val);
	virtual void MoveVertical(float val);
	virtual void RotateObject(float val);
	virtual void ScaleObject(float val);

	virtual ICollisionObject* GetCollider() { return m_Collider; }

	// �׸��� ����
	// �ִϸ��̼� �ܿ� �ڱ� �ڽ��� �׷��� �Լ��� ������ �ִ´�.
	virtual void TestDraw(D2DEngine* pEngine);
	virtual void Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric);
	virtual void DrawDebugData(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM);

	virtual ID2D1Bitmap* GetBitmap() { return m_bitmap; }

	bool m_IsDrew;

public:
	// Getter/Setter
	D2D1_VECTOR_2F GetNowCenter() const { return m_bitmapCenter; }
	void SetNowCenter(D2D1_VECTOR_2F val) { m_bitmapCenter = val; }

	CTransform* GetTransform() const { return m_Transform; }
	void SetTransform(CTransform* val) { m_Transform = val; }

};

