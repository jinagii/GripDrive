#pragma once

#include "GlobalDefine.h"

class D2DEngine;

class ICollisionObject 
{
public:

	virtual void Draw(D2DEngine* pEngine) abstract;

	virtual void MoveHorizontal(float val) abstract;
	virtual void MoveVertical(float val) abstract;
	virtual void RotateAABB(float angle) abstract;

	virtual bool CollisionDetection(ICollisionObject* pTarget) abstract;

	virtual bool CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec) abstract;

	// ������Ʈ�� ������Ʈ ���� �� �������� ������ �ִ� �ݶ��̴��� ��ġ���� �������ش�.
	virtual void SetPosition(D2D1_VECTOR_2F pos) abstract;
	virtual D2D1_VECTOR_2F GetPosition() abstract;

	virtual void SetIsCollided(bool val) abstract;
	virtual bool GetIsCollided() abstract;
};

