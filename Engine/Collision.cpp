#include "Collision.h"

bool Collision::CircleCollision(D2D1_POINT_2F point, int radius1, D2D1_POINT_2F point2, int radius2)
{
	int distX = point.x - point2.x;                 // 밑변
	int distY = point.y - point2.y;                 // 높이
	int distT = sqrt((distX * distX) + (distY * distY));  // 빗변

	// 원1과 원2의 반지름을 더 해줘야하는데, 두 개의 원의 크기가 같으므로 곱하기 2를 해준다.
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

