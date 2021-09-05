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
/// 게임 내에 등장하는 오브젝트
/// 
/// 차량, 드럼통, 벽, 사람, 이펙트, 먼지, 트리거
/// </summary>
class CGameObject
{
public:
	CGameObject();
	CGameObject(D2D1_RECT_F _rect);
	~CGameObject();
	

	
protected:
	// 게임 오브젝트는 자기 자신을 그려줄 무언가를 가지고 있을 수 있다.
	ID2D1Bitmap* m_bitmap;
	CTransform* m_Transform;
	ICollisionObject* m_Collider;
	D2D1_VECTOR_2F m_bitmapCenter;		// 현재 위치를 기반으로 비트맵을 그릴 곳을 재조정
										// 이를테면 비트맵을 위한 LocalPosition이 된다.


public:
	// 초기화
	virtual void Initialize(D2DEngine* pEngine);

	// 업데이트 관련
	virtual void Update(float dTime);

	// 충돌감지 관련
	virtual void CollisionCheck(CGameObject* target);
	virtual void CollisionCheck2(CGameObject* target);

	virtual D2D1_VECTOR_2F GetVelocity();
	virtual void SetVelocity(D2D1_VECTOR_2F velocity);


	// 기본적으로 위치값을 조정할 수 있을 만한 가상 함수도 만들어둔다.
	virtual void MoveHorizontal(float val);
	virtual void MoveVertical(float val);
	virtual void RotateObject(float val);
	virtual void ScaleObject(float val);

	virtual ICollisionObject* GetCollider() { return m_Collider; }

	// 그리기 관련
	// 애니메이션 외에 자기 자신을 그려줄 함수는 가지고 있는다.
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

