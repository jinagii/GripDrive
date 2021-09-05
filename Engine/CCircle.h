#pragma once

#include "ICollisionObject.h"
#include "GlobalDefine.h"

class CCircle : public ICollisionObject
{
public:
	CCircle();
	CCircle(float posX, float posY, int radius);
	CCircle(D2D1_VECTOR_2F point, int radius);
	~CCircle();

private:
	D2D1_VECTOR_2F m_Position;
	int m_Radius;

	bool m_IsCollide;

public:
	virtual void Draw(D2DEngine* pEngine);

	virtual void MoveHorizontal(float val);
	virtual void MoveVertical(float val);

	virtual bool CollisionDetection(ICollisionObject* pTarget);
	 bool CollisionDetection(CCircle* pTarget);
	
	 virtual bool CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec);	// 좀 더 제대로 된 반사 벡터를 이용한 MK2. 충격량의 손실과 나눔, BTP이슈도 막았다.


	virtual void RotateAABB(float angle);

	// 오브젝트 업데이트 시 현 위치를 갱신해준다.
	virtual void SetPosition(D2D1_VECTOR_2F pos) override;
	virtual D2D1_VECTOR_2F GetPosition() override;

	virtual void SetIsCollided(bool val) override;
	virtual bool GetIsCollided() override;

public:
	int GetRadius() const { return m_Radius; }
	bool GetIsCollide() const { return m_IsCollide; }
	void SetCollide(bool val) { m_IsCollide = val; }



};

