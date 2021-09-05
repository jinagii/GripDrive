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
	// 비트맵을 불러와서 포인터로 저장한뒤, for문 내에서 모두 같은 포인터를 전달해준다.
	// 스프라이트로 할 거라 아무거나 넣어라
	ID2D1Bitmap* _pBitmap = m_pEngine->LoadBitmapByPath(L"./data/dustMini.png");

	m_SpriteSheet = m_pEngine->LoadBitmapByPath(L"./data/Smoke_900x300.png");

	// 오브젝트 풀링으로 파티클을 여러개 만들어둔다.
	for (int i = 0; i < count; i++)
	{
		ParticleInfo* _particleInfo = new ParticleInfo();


		// LifeTime은 파티클을 실행할 때 정해준다.
		// IsPlay는 생성 시에는 false로, 실행 시에는 true로해서 그려준다.
		// RocalRotation은 자기 자신이 빙빙 돌아가는 것을 표현한다.
		// Opacity는 처음에는 1로 해주고, 필요 시 나중에 바꿔준다.

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

	// 소스 이미지를 불러올 곳을 정한다.
	D2D1_RECT_F _rect;
	_rect.left = _seedIndex * 300;
	_rect.top = 0;
	_rect.right = _rect.left + 300;
	_rect.bottom = 300;

	// 여기서 Offset은 자동차의 뒷부분, 즉 연기가 발생할 곳을 의미한다.
	D2D1_POINT_2F _offset;
	_offset.x = 0.f;
	_offset.y = 30.f;
	D2D1_POINT_2F _newPos = CTransform::VectorRotation(_nowPos, _offset, angle);

	/// 맵의 속성에 따라 제각기 다른 재생 빈도를 만들어주자
	/// 아스팔트와 흙 위에서 그려주는 이펙트가 달라도 좋을 것 같다.

	for (unsigned int i = 0; i < v_Particles.size(); i++)
	{
		if (v_Particles[i] == nullptr) continue;
		if (v_Particles[i]->m_IsPlay) continue;

		ParticleInfo* _nowPlaying = v_Particles[i];

		_nowPlaying->m_IsPlay = true;

		// 어떤 부분을 그려줄지 결정하는 부분
		_nowPlaying->m_SourceRect = _rect;

		_nowPlaying->m_LifeTime = _seedLifeTime;
		_nowPlaying->m_Pos = _newPos;
		//_nowPlaying->m_Pos = D2D1::Point2F(nowPos.x, nowPos.y);
		_nowPlaying->m_RocalRotation = _seedRotation;
		_nowPlaying->m_Scale = 0;
		_nowPlaying->m_Opacity = _seedOpacity;

		// 하나를 만들어줬으면 break로 빠져나온다.
		// 안빠져나오면 한 군데에 여러개를 그린다.
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

	// 소스 이미지를 불러올 곳을 정한다.
	D2D1_RECT_F _rect;
	_rect.left = _seedIndex * 300;
	_rect.top = 0;
	_rect.right = _rect.left + 300;
	_rect.bottom = 300;

	D2D1_POINT_2F _newPos = CTransform::VectorRotation(_nowPos, _offset, angle);

	/// 맵의 속성에 따라 제각기 다른 재생 빈도를 만들어주자
	/// 아스팔트와 흙 위에서 그려주는 이펙트가 달라도 좋을 것 같다.

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

		// 하나를 만들어줬으면 break로 빠져나온다.
		// 안빠져나오면 한 군데에 여러개를 그린다.
		break;
	}
}

void Smoke::DrawParticle()
{
	for (unsigned int i = 0; i < v_Particles.size(); i++)
	{
		if (v_Particles[i] == nullptr) continue;

		// True되어있는 파티클에게만 접근하겠다.
		if (!v_Particles[i]->m_IsPlay) continue;

		ParticleInfo* _particle = v_Particles[i];
		

		/// 기존에 파티클을 그려주던 함수
		//m_pEngine->ImageDrawSmoke(_particle->m_Bitmap, _particle->m_Pos, _particle->m_Scale, _particle->m_Opacity);

		m_pEngine->ImageDrawSmokeFromSheet(
			m_SpriteSheet, 
			_particle->m_SourceRect,
			_particle->m_Pos, 
			_particle->m_Scale,
			_particle->m_Opacity);

		// 파티클 자신이 회전하려면 회전 행렬로 계속 곱해줘야한다.
		/*
		D2D1::Matrix3x2F _matrix = D2D1::Matrix3x2F::Rotation(_particle->m_RocalRotation, _particle->m_Pos);

		ID2D1Effect* _effect = m_pEngine->AddAffinTransform(_particle->m_Bitmap, _matrix);

		_effect = m_pEngine->AddOpacity(_effect, _particle->m_Opacity);

		m_pEngine->DrawEffectBitmap(_effect);
		*/

		

		_particle->m_Scale += 0.1f;

		_particle->m_Opacity -= 0.01f;
		_particle->m_LifeTime -= 0.005f;

		// 라이프 타임이 지나면 실행을 종료시킨다.
		if (_particle->m_LifeTime < 0)
		{
			_particle->m_LifeTime = 0;
			_particle->m_IsPlay = false;

			_particle->m_Opacity = 1.f;
		}
	}
}

