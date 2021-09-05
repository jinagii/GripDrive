#include "Collision.h"

bool Collision::CircleCollision(D2D1_POINT_2F point, int radius1, D2D1_POINT_2F point2, int radius2)
{
	int distX = point.x - point2.x;                 // �غ�
	int distY = point.y - point2.y;                 // ����
	int distT = sqrt((distX * distX) + (distY * distY));  // ����

	// ��1�� ��2�� �������� �� ������ϴµ�, �� ���� ���� ũ�Ⱑ �����Ƿ� ���ϱ� 2�� ���ش�.
	if (distT < radius1 + radius2)
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Collision::AABBCollision(RECT rc1, RECT rc2)
{
	if (rc1.left < rc2.right &&
		rc2.left < rc1.right &&
		rc1.top < rc2.bottom &&
		rc2.top < rc1.bottom)
	{
		return true;
	}
	else
	{
		return false;
	}

}

