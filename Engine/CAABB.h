#pragma once

#include "GlobalDefine.h"
#include "ICollisionObject.h"

class D2DEngine;

class CAABB : public ICollisionObject
{
public:
	CAABB();
	CAABB(float posX, float posY, int width, int height);
	CAABB(D2D1_RECT_F _rect);
	~CAABB();


private:
	D2D1_RECT_F m_Rect;
	D2D1_SIZE_F m_Size;

	float m_Angle;
	bool m_IsCollide;

public:
	virtual void Draw(D2DEngine* pEngine) override;

	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;
	virtual void RotateAABB(float angle) override;

	virtual bool CollisionDetection(ICollisionObject* pTarget) override;
	virtual bool CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec) override;

	virtual void SetPosition(D2D1_VECTOR_2F pos) override;
	// 왼쪽 상단을 중점으로 좌표를 반환한다.
	virtual D2D1_VECTOR_2F GetPosition() override;
	
	virtual void SetIsCollided(bool val) override;
	virtual bool GetIsCollided() override;

public:
	// 포함될 객체에서 위치 정보를 가져와 여기에 넣어준다.
	D2D1_SIZE_F GetSize() const { return m_Size; }

	D2D1_RECT_F GetRectPosition() const { return m_Rect; }
	void SetCollide(bool val) { m_IsCollide = val; }
	float GetAngle() const { return m_Angle; }
	bool IsCollide() const { return m_IsCollide; }
};

