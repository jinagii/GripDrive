#pragma once

#include "GlobalDefine.h"
#include "ICollisionObject.h"

class D2DEngine;

class CVECTOR2
{
public:
	CVECTOR2()
	{
		x = 0;
		y = 0;
	}

	CVECTOR2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float x;
	float y;

	CVECTOR2 operator+(const CVECTOR2& v);
	CVECTOR2 operator-(const CVECTOR2& v);

	CVECTOR2 operator*(const float& f);
	CVECTOR2 operator/(const float& f);

	float GetLength();

};

class COBB : public ICollisionObject
{
public:
	COBB(float centerX, float centerY, float width, float height, float angle);
	~COBB();


private:
	bool m_IsCollide;

public:
	// �̵��� ����� ���� �� (��ġ���� center)
	float m_Width;
	float m_Height;
	float m_Angle;

	// OBB�� �̷�� 4���� ��. ��ġ���̱⵵ �ϰ�, ������ �������� �� �����̱⵵ �ϴ�.
	CVECTOR2 p1;
	CVECTOR2 p2;
	CVECTOR2 p3;
	CVECTOR2 p4;

	CVECTOR2 edge1;
	CVECTOR2 edge2;
	CVECTOR2 edge3;
	CVECTOR2 edge4;

	const float PI = 3.1415f;

public:
	std::vector<CVECTOR2> Vertices;
	std::vector<CVECTOR2> Edges;

	void MakeEdges();
	void RecalcOBB(CVECTOR2& pos);

	// ������ ������ �Ÿ��� ���� (�����̿�)
public:
	float GetDistanceFromPoint(CVECTOR2 lineVec, CVECTOR2 pointOnLine, CVECTOR2 targetPoint);

	// 2-3 ������ ������(�ð�), 4-1������ �������̸� 1-2���� �ȿ� �ִ� ���̴�.
	bool IsPointInOBB1_2Edge(CVECTOR2 pos);
	bool IsRightSide(CVECTOR2 p1, CVECTOR2 p2, CVECTOR2 pos);
private:
	float CrossProduct(CVECTOR2 a, CVECTOR2 b);

public:
	virtual void Draw(D2DEngine* pEngine) override;

	virtual void MoveHorizontal(float val) override;
	virtual void MoveVertical(float val) override;
	virtual void RotateAABB(float angle) override;

	virtual bool CollisionDetection(ICollisionObject* pTarget) override;
	virtual bool CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec) override;

	// �������� �߽�. �������� ������� OBB�� �籸���Ѵ�.
	virtual void SetPosition(D2D1_VECTOR_2F pos) override;
	virtual D2D1_VECTOR_2F GetPosition() override;

	virtual void SetIsCollided(bool val) override;
	virtual bool GetIsCollided() override;

	float DegreeToRad(float degree);

public:
	// ���Ե� ��ü���� ��ġ ������ ������ ���⿡ �־��ش�.
// 	D2D1_SIZE_F GetSize() const { return m_Size; }
// 
// 	D2D1_RECT_F GetRectPosition() const { return m_Rect; }
// 	void SetCollide(bool val) { m_IsCollide = val; }
// 	float GetAngle() const { return m_Angle; }
// 	bool IsCollide() const { return m_IsCollide; }

};