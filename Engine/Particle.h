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
	ID2D1Bitmap* m_SpriteSheet;			// ����Ʈ�� �� �ִ� ��������Ʈ ��Ʈ
	int m_spriteIndex;						// ��Ʈ�� ��������Ʈ ����
	vector<ParticleInfo*> v_Particles;

public:

	// ��ƼŬ�� ���� �� count ��ŭ�� ������Ʈ Ǯ�� �����.
	void IntializeParticle(int count);
	void PlayParticle(D2D1_VECTOR_2F nowPos, float angle);
	void PlayParticle(D2D1_VECTOR_2F nowPos, D2D1_POINT_2F offset, float angle);

	void ResetParticle();

	// ��������Ʈ ��Ʈ�� ���� �׷��ش�.
	void DrawParticle();
};

