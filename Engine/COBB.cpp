#include "COBB.h"


CVECTOR2 CVECTOR2::operator+(const CVECTOR2& v)
{
	CVECTOR2 r;
	r.x = this->x + v.x;
	r.y = this->y + v.y;
	return r;
}

CVECTOR2 CVECTOR2::operator-(const CVECTOR2& v)
{
	CVECTOR2 r;
	r.x = this->x - v.x;
	r.y = this->y - v.y;
	return r;
}

CVECTOR2 CVECTOR2::operator*(const float& f)
{
	CVECTOR2 r;
	r.x = this->x * f;
	r.y = this->y * f;
	return r;
}

CVECTOR2 CVECTOR2::operator/(const float& f)
{
	CVECTOR2 r;
	r.x = this->x / f;
	r.y = this->y / f;
	return r;
}

float CVECTOR2::GetLength()
{
	return sqrtf(powf(x, 2.0f) + powf(y, 2.0f));
}

COBB::COBB(float centerX, float centerY, float width, float height, float angle)
	: m_IsCollide(false)
{
	m_Width = width;
	m_Height = height;
	m_Angle = DegreeToRad(angle);

	CVECTOR2 _center(centerX, centerY);

	RecalcOBB(_center);
}

COBB::~COBB()
{

}

void COBB::MakeEdges()
{
	Vertices.clear();
	Edges.clear();

	Vertices.push_back(p1);
	Vertices.push_back(p2);
	Vertices.push_back(p3);
	Vertices.push_back(p4);

	edge1.x = p2.x - p1.x;
	edge1.y = p2.y - p1.y;

	edge2.x = p3.x - p2.x;
	edge2.y = p3.y - p2.y;

	edge3.x = p4.x - p3.x;
	edge3.y = p4.y - p3.y;

	edge4.x = p1.x - p4.x;
	edge4.y = p1.y - p4.y;	/// 엣지 계산은 다 한다. 지금은 OBB충돌감지가 아니므로..

	Edges.push_back(edge1);
	Edges.push_back(edge2);
	//Edges.push_back(edge3);
	//Edges.push_back(edge4);
}

void COBB::RecalcOBB(CVECTOR2& pos)
{
	CVECTOR2 X(cos(m_Angle), sin(m_Angle));
	CVECTOR2 Y(-sin(m_Angle), cos(m_Angle));

	X = X * (m_Width / 2);
	Y = Y * (m_Height / 2);

	p1 = pos - X - Y;
	p2 = pos + X - Y;
	p3 = pos + X + Y;
	p4 = pos - X + Y;

	MakeEdges();
}

// 외적을 이용해서, 직선과 점의 거리를 구하는 함수
float COBB::GetDistanceFromPoint(CVECTOR2 lineVec, CVECTOR2 pointOnLine, CVECTOR2 targetPoint)
{
	float _absU = abs(lineVec.GetLength());

	if (_absU != 0)
	{
		CVECTOR2 _paVec = pointOnLine - targetPoint;

		float _d = abs(this->CrossProduct(_paVec, lineVec)) / _absU;

		return _d;
	}

	return 0;
}

// 2-3 엣지의 오른쪽(시계), 4-1엣지의 오른쪽이면 1-2선분 안에 있는 것이다.
bool COBB::IsPointInOBB1_2Edge(CVECTOR2 pos)
{
	// 2-3엣지의 직선의 방정식을 통해서 사이드를 판별한다.
	if (IsRightSide(p2, p3, pos) == true
		&& IsRightSide(p4, p1, pos) == true)
	{
		return true;
	}

	return false;
}

// 점2개로 이루어지는 방향을 가진 직선 (벡터이지만 위치도 중요하다)의 오른쪽에 있는가?
// 간단히 외적만으로는 안되더라..
bool COBB::IsRightSide(CVECTOR2 p1, CVECTOR2 p2, CVECTOR2 pos)
{
	// 1. x축에 평행한 직선의 경우
	if (abs(p1.y - p2.y) < 0.1f)
	{
		// 왼쪽으로 가는 직선
		if (p2.x < p1.x)
		{
			// 점이 직선 아래에 있으면 오른쪽
			return (pos.y < p1.y);
		}
		// 오른쪽으로 가는 직선
		else
		{
			return (p1.y < pos.y);
		}
	}
	// 2. y축에 평행한 직선
	else if (abs(p1.x - p2.x) < 0.1f)
	{
		// 위로 가는 직선(스크린좌표계)
		if (p2.y < p1.y)
		{
			return (p1.x < pos.x);
		}
		// 아래로 가는 직선
		else
		{
			return (pos.x < p1.x);
		}
	}
	else
	{
		// 직선의 방정식으로 정리한 것
		float _y = ((p2.y - p1.y) / (p2.x - p1.x)) * (pos.x - p1.x) + p1.y;

		// 3. 기울기가 있고, 오른쪽 방향일 경우, 그 직선의 방정식에 점을 대입한 y값이 더 크면 오른쪽
		if (0 < p2.x - p1.x)
		{
			return (_y < pos.y);
		}
		else
		{
			return (pos.y < _y);
		}
	}

	// 모두에 걸리지 않으면...
	return false;
}

float COBB::CrossProduct(CVECTOR2 v1, CVECTOR2 v2)
{
	return v1.x * v2.y - v2.x * v1.y;
}

void COBB::Draw(D2DEngine* pEngine)
{

}

void COBB::MoveHorizontal(float val)
{

}

void COBB::MoveVertical(float val)
{

}

void COBB::RotateAABB(float angle)
{

}

bool COBB::CollisionDetection(ICollisionObject* pTarget)
{

	return true;
}

bool COBB::CollisionDetection2(ICollisionObject* pTarget, D2D1_VECTOR_2F* reflVec)
{
	return true;
}

// 위치를 기반으로 다시 계산해준다.
void COBB::SetPosition(D2D1_VECTOR_2F pos)
{
	CVECTOR2 center(pos.x, pos.y);

	this->RecalcOBB(center);
}

D2D1_VECTOR_2F COBB::GetPosition()
{
	return D2D1::Vector2F(0, 0);
}

void COBB::SetIsCollided(bool val)
{
	m_IsCollide = val;
}

bool COBB::GetIsCollided()
{
	return m_IsCollide;
}

float COBB::DegreeToRad(float degree)
{
	return degree * PI / 180.f;
}
