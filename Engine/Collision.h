#pragma once

#include "GlobalDefine.h"

class Collision
{
public:
	static bool CircleCollision(D2D1_POINT_2F point, int radius1, D2D1_POINT_2F point2, int radius2);
	static bool AABBCollision(RECT rc1, RECT rc2);
};

