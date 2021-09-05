#pragma once

#include "GlobalDefine.h"
#include <vector>

using namespace std;

class D2DEngine;

class Smoke
{
public:
	Smoke();
	~Smoke();

private:
	D2DEngine* m_pEngine;
	ID2D1Bitmap* m_SpriteSheet;
	vector<ParticleInfo*> v_Particles;

public:
	
	// 파티클을 만들 때 count 만큼의 오브젝트 풀을 만든다.
	void IntializeParticle(int count);
	void PlayParticle(D2D1_VECTOR_2F nowPos, float angle);
	void PlayParticle(D2D1_VECTOR_2F nowPos, D2D1_POINT_2F offset, float angle);

	void DrawParticle();
};

