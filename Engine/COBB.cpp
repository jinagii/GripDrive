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
	edge4.y = p1.y - p4.y;	/// ���� ����� �� �Ѵ�. ������ OBB�浹������ �ƴϹǷ�..

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

// ������ �̿��ؼ�, ������ ���� �Ÿ��� ���ϴ� �Լ�
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

// 2-3 ������ ������(�ð�), 4-1������ �������̸� 1-2���� �ȿ� �ִ� ���̴�.
bool COBB::IsPointInOBB1_2Edge(CVECTOR2 pos)
{
	// 2-3������ ������ �������� ���ؼ� ���̵带 �Ǻ��Ѵ�.
	if (IsRightSide(p2, p3, pos) == true
		&& IsRightSide(p4, p1, pos) == true)
	{
		return true;
	}

	return false;
}

// ��2���� �̷������ ������ ���� ���� (���������� ��ġ�� �߿��ϴ�)�� �����ʿ� �ִ°�?
// ������ ���������δ� �ȵǴ���..
bool COBB::IsRightSide(CVECTOR2 p1, CVECTOR2 p2, CVECTOR2 pos)
{
	// 1. x�࿡ ������ ������ ���
	if (abs(p1.y - p2.y) < 0.1f)
	{
		// �������� ���� ����
		if (p2.x < p1.x)
		{
			// ���� ���� �Ʒ��� ������ ������
			return (pos.y < p1.y);
		}
		// ���������� ���� ����
		else
		{
			return (p1.y < pos.y);
		}
	}
	// 2. y�࿡ ������ ����
	else if (abs(p1.x - p2.x) < 0.1f)
	{
		// ���� ���� ����(��ũ����ǥ��)
		if (p2.y < p1.y)
		{
			return (p1.x < pos.x);
		}
		// �Ʒ��� ���� ����
		else
		{
			return (pos.x < p1.x);
		}
	}
	else
	{
		// ������ ���������� ������ ��
		float _y = ((p2.y - p1.y) / (p2.x - p1.x)) * (pos.x - p1.x) + p1.y;

		// 3. ���Ⱑ �ְ�, ������ ������ ���, �� ������ �����Ŀ� ���� ������ y���� �� ũ�� ������
		if (0 < p2.x - p1.x)
		{
			return (_y < pos.y);
		}
		else
		{
			return (pos.y < _y);
		}
	}

	// ��ο� �ɸ��� ������...
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

// ��ġ�� ������� �ٽ� ������ش�.
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
