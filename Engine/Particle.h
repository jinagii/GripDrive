#pragma once

#include "GlobalDefine.h"
#include <vector>

using namespace std;

class D2DEngine;

class Particle
{
public:
	Particle();
	Particle(PCWSTR path, int count);
	~Particle();

private:
	D2DEngine* m_pEngine;
	ID2D1Bitmap* m_SpriteSheet;			// 이펙트가 들어가 있는 스프라이트 시트
	int m_spriteIndex;						// 시트의 스프라이트 개수
	vector<ParticleInfo*> v_Particles;

public:

	// 파티클을 만들 때 count 만큼의 오브젝트 풀을 만든다.
	void IntializeParticle(int count);
	void PlayParticle(D2D1_VECTOR_2F nowPos, float angle);
	void PlayParticle(D2D1_VECTOR_2F nowPos, D2D1_POINT_2F offset, float angle);

	void ResetParticle();

	// 스프라이트 시트를 통해 그려준다.
	void DrawParticle();
};

