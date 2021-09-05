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
	// 이동을 고려한 원본 값 (위치값은 center)
	float m_Width;
	float m_Height;
	float m_Angle;

	// OBB를 이루는 4개의 점. 위치값이기도 하고, 원점을 기준으로 한 벡터이기도 하다.
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

	// 직선과 점과의 거리를 구함 (외적이용)
public:
	float GetDistanceFromPoint(CVECTOR2 lineVec, CVECTOR2 pointOnLine, CVECTOR2 targetPoint);

	// 2-3 엣지의 오른쪽(시계), 4-1엣지의 오른쪽이면 1-2선분 안에 있는 것이다.
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

	// 포지션은 중심. 포지션을 기반으로 OBB를 재구성한다.
	virtual void SetPosition(D2D1_VECTOR_2F pos) override;
	virtual D2D1_VECTOR_2F GetPosition() override;

	virtual void SetIsCollided(bool val) override;
	virtual bool GetIsCollided() override;

	float DegreeToRad(float degree);

public:
	// 포함될 객체에서 위치 정보를 가져와 여기에 넣어준다.
// 	D2D1_SIZE_F GetSize() const { return m_Size; }
// 
// 	D2D1_RECT_F GetRectPosition() const { return m_Rect; }
// 	void SetCollide(bool val) { m_IsCollide = val; }
// 	float GetAngle() const { return m_Angle; }
// 	bool IsCollide() const { return m_IsCollide; }

};