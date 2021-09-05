#include "Smoke.h"
#include "D2DEngine.h"
#include "CTransform.h"

Smoke::Smoke()
{
	m_pEngine = D2DEngine::Instance();
}

Smoke::~Smoke()
{

	for (unsigned int i = 0; i < v_Particles.size(); i++)
	{
		delete v_Particles[i];
	}

	v_Particles.clear();

}

void Smoke::IntializeParticle(int count)
{
	// ��Ʈ���� �ҷ��ͼ� �����ͷ� �����ѵ�, for�� ������ ��� ���� �����͸� �������ش�.
	// ��������Ʈ�� �� �Ŷ� �ƹ��ų� �־��
	ID2D1Bitmap* _pBitmap = m_pEngine->LoadBitmapByPath(L"./data/dustMini.png");

	m_SpriteSheet = m_pEngine->LoadBitmapByPath(L"./data/Smoke_900x300.png");

	// ������Ʈ Ǯ������ ��ƼŬ�� ������ �����д�.
	for (int i = 0; i < count; i++)
	{
		ParticleInfo* _particleInfo = new ParticleInfo();


		// LifeTime�� ��ƼŬ�� ������ �� �����ش�.
		// IsPlay�� ���� �ÿ��� false��, ���� �ÿ��� true���ؼ� �׷��ش�.
		// RocalRotation�� �ڱ� �ڽ��� ���� ���ư��� ���� ǥ���Ѵ�.
		// Opacity�� ó������ 1�� ���ְ�, �ʿ� �� ���߿� �ٲ��ش�.

		_particleInfo->m_Bitmap = _pBitmap;
		_particleInfo->m_SourceRect = D2D1::RectF();
		_particleInfo->m_LifeTime = 0;
		_particleInfo->m_IsPlay = false;
		_particleInfo->m_Pos = D2D1::Point2F();
		_particleInfo->m_Scale = 0;
		_particleInfo->m_RocalRotation = 0;
		_particleInfo->m_Opacity = 1.0f;

		v_Particles.push_back(_particleInfo);
	}

}

void Smoke::PlayParticle(D2D1_VECTOR_2F _nowPos,  float angle)
{
	float _seedLifeTime = (rand() % 10) * 0.1f;
	float _seedScale = (rand() % 10) * 0.1f;
	float _seedOpacity = (rand() % 10) * 0.1f;
	float _seedRotation = (rand() % 360);
	int _seedIndex = (rand() % 3);

	// �ҽ� �̹����� �ҷ��� ���� ���Ѵ�.
	D2D1_RECT_F _rect;
	_rect.left = _seedIndex * 300;
	_rect.top = 0;
	_rect.right = _rect.left + 300;
	_rect.bottom = 300;

	// ���⼭ Offset�� �ڵ����� �޺κ�, �� ���Ⱑ �߻��� ���� �ǹ��Ѵ�.
	D2D1_POINT_2F _offset;
	_offset.x = 0.f;
	_offset.y = 30.f;
	D2D1_POINT_2F _newPos = CTransform::VectorRotation(_nowPos, _offset, angle);

	/// ���� �Ӽ��� ���� ������ �ٸ� ��� �󵵸� ���������
	/// �ƽ���Ʈ�� �� ������ �׷��ִ� ����Ʈ�� �޶� ���� �� ����.

	for (unsigned int i = 0; i < v_Particles.size(); i++)
	{
		if (v_Particles[i] == nullptr) continue;
		if (v_Particles[i]->m_IsPlay) continue;

		ParticleInfo* _nowPlaying = v_Particles[i];

		_nowPlaying->m_IsPlay = true;

		// � �κ��� �׷����� �����ϴ� �κ�
		_nowPlaying->m_SourceRect = _rect;

		_nowPlaying->m_LifeTime = _seedLifeTime;
		_nowPlaying->m_Pos = _newPos;
		//_nowPlaying->m_Pos = D2D1::Point2F(nowPos.x, nowPos.y);
		_nowPlaying->m_RocalRotation = _seedRotation;
		_nowPlaying->m_Scale = 0;
		_nowPlaying->m_Opacity = _seedOpacity;

		// �ϳ��� ����������� break�� �������´�.
		// �Ⱥ��������� �� ������ �������� �׸���.
		break;
	}	
}

void Smoke::PlayParticle(D2D1_VECTOR_2F _nowPos, D2D1_POINT_2F _offset, float angle)
{
	float _seedLifeTime = (rand() % 10) * 0.1f;
	float _seedScale = (rand() % 5) * 0.1f;
	float _seedOpacity = (rand() % 10) * 0.1f;
	float _seedRotation = (rand() % 360);
	int _seedIndex = (rand() % 3);

	// �ҽ� �̹����� �ҷ��� ���� ���Ѵ�.
	D2D1_RECT_F _rect;
	_rect.left = _seedIndex * 300;
	_rect.top = 0;
	_rect.right = _rect.left + 300;
	_rect.bottom = 300;

	D2D1_POINT_2F _newPos = CTransform::VectorRotation(_nowPos, _offset, angle);

	/// ���� �Ӽ��� ���� ������ �ٸ� ��� �󵵸� ���������
	/// �ƽ���Ʈ�� �� ������ �׷��ִ� ����Ʈ�� �޶� ���� �� ����.

	for (unsigned int i = 0; i < v_Particles.size(); i++)
	{
		if (v_Particles[i] == nullptr) continue;
		if (v_Particles[i]->m_IsPlay) continue;

		ParticleInfo* _nowPlaying = v_Particles[i];

		_nowPlaying->m_IsPlay = true;

		_nowPlaying->m_SourceRect = _rect;

		_nowPlaying->m_LifeTime = _seedLifeTime;
		_nowPlaying->m_Pos = _newPos;
		//_nowPlaying->m_Pos = D2D1::Point2F(nowPos.x, nowPos.y);
		_nowPlaying->m_RocalRotation = _seedRotation;
		_nowPlaying->m_Scale = _seedScale;
		_nowPlaying->m_Opacity = _seedOpacity;

		// �ϳ��� ����������� break�� �������´�.
		// �Ⱥ��������� �� ������ �������� �׸���.
		break;
	}
}

void Smoke::DrawParticle()
{
	for (unsigned int i = 0; i < v_Particles.size(); i++)
	{
		if (v_Particles[i] == nullptr) continue;

		// True�Ǿ��ִ� ��ƼŬ���Ը� �����ϰڴ�.
		if (!v_Particles[i]->m_IsPlay) continue;

		ParticleInfo* _particle = v_Particles[i];
		

		/// ������ ��ƼŬ�� �׷��ִ� �Լ�
		//m_pEngine->ImageDrawSmoke(_particle->m_Bitmap, _particle->m_Pos, _particle->m_Scale, _particle->m_Opacity);

		m_pEngine->ImageDrawSmokeFromSheet(
			m_SpriteSheet, 
			_particle->m_SourceRect,
			_particle->m_Pos, 
			_particle->m_Scale,
			_particle->m_Opacity);

		// ��ƼŬ �ڽ��� ȸ���Ϸ��� ȸ�� ��ķ� ��� ��������Ѵ�.
		/*
		D2D1::Matrix3x2F _matrix = D2D1::Matrix3x2F::Rotation(_particle->m_RocalRotation, _particle->m_Pos);

		ID2D1Effect* _effect = m_pEngine->AddAffinTransform(_particle->m_Bitmap, _matrix);

		_effect = m_pEngine->AddOpacity(_effect, _particle->m_Opacity);

		m_pEngine->DrawEffectBitmap(_effect);
		*/

		

		_particle->m_Scale += 0.1f;

		_particle->m_Opacity -= 0.01f;
		_particle->m_LifeTime -= 0.005f;

		// ������ Ÿ���� ������ ������ �����Ų��.
		if (_particle->m_LifeTime < 0)
		{
			_particle->m_LifeTime = 0;
			_particle->m_IsPlay = false;

			_particle->m_Opacity = 1.f;
		}
	}
}

