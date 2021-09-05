
#include "CGameObject.h"

#include "CTransform.h"
#include "D2DEngine.h"
#include "CASprite.h"
#include "CAABB.h"
#include "CCircle.h"
#include "CVector.h"


CGameObject::CGameObject()
	: m_bitmap(nullptr), m_Collider(nullptr), m_bitmapCenter(), m_Transform()
{

}

CGameObject::CGameObject(D2D1_RECT_F _rect)
	: m_bitmap(nullptr)

{
	m_Transform = new CTransform();
	m_Collider = new CAABB(_rect);		
	// 게임 오브젝트 생성 시 자신의 콜라이더를 알려준다.
}

CGameObject::~CGameObject()
{

}

void CGameObject::Initialize(D2DEngine* pEngine)
{

}

void CGameObject::Draw(D2DEngine* pEngine, float dTime, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM, bool isIsometric)
{
	// 현재 비트맵이 있는지 먼저 검사한다.
	if (m_bitmap != nullptr)
	{
		// 현재 오브젝트의 최종 TransformMatrix를 가져온다
		pEngine->SetTransform(m_Transform->MatrixTranslation(m_Transform->GetLocalPosition()) * projTM * viewTM);

		//pEngine->ImageDrawByPoint(m_bitmap, m_nowCenter);
		pEngine->DrawGOImage(m_bitmap);
	}
	//DrawDebugData(pEngine);

}

void CGameObject::DrawDebugData(D2DEngine* pEngine, D2D1_MATRIX_3X2_F viewTM, D2D1_MATRIX_3X2_F projTM)
{
	CAABB* _aabb = dynamic_cast<CAABB*>(m_Collider);
	
	if (_aabb != nullptr)
	{
		D2D1_RECT_F _rect = _aabb->GetRectPosition();
		D2D1_SIZE_F _size = _aabb->GetSize();
		bool _isDetection = _aabb->IsCollide();

		pEngine->SetTransform(m_Transform->MatrixTranslation(m_Collider->GetPosition()) * projTM * viewTM);

		pEngine->DrawRectangle_Debug(
			0,
			0,
			_size.width,
			_size.height,
			_isDetection);
	}

	CCircle* _circle = dynamic_cast<CCircle*>(m_Collider);
	
	if (_circle != nullptr)
	{
		bool _isDetection = _circle->GetIsCollided();

		// 현재 오브젝트의 위치를 알려준다 
		pEngine->SetTransform(m_Transform->MatrixTranslation(m_Collider->GetPosition()) * projTM * viewTM);
		
		// 여기서 자신의 위치값 만큼 더해줘야 원하는 곳에 찍힌다
		pEngine->DrawEllipse_Debug(
			0,
			0,
			_circle->GetRadius(),
			_isDetection);
	}
}

void CGameObject::Update(float dTime)
{
	
}

void CGameObject::CollisionCheck(CGameObject* target)
{
	// 딱히 처리가 없다. 사실 이 클래스는 추상 클래스여야 한다.
	// 그 아래 하나의 레이어가 더 있어도 되고..
}

void CGameObject::CollisionCheck2(CGameObject* target)
{
	// 두 번째 버전
}

D2D1_VECTOR_2F CGameObject::GetVelocity()
{

	return D2D1::Vector2F(0,0);
}

void CGameObject::SetVelocity(D2D1_VECTOR_2F)
{

}

void CGameObject::TestDraw(D2DEngine* pEngine)
{

}

void CGameObject::MoveHorizontal(float val)
{
	D2D_VECTOR_2F _vec;

	_vec.x = val;
	_vec.y = 0;

	m_Transform->MoveByTranslate(_vec);
}

void CGameObject::MoveVertical(float val)
{
	D2D_VECTOR_2F _vec;

	_vec.x = 0;
	_vec.y = val;

	m_Transform->MoveByTranslate(_vec);
}

void CGameObject::RotateObject(float val)
{
	m_Transform->TurnByRotation(val);
}

void CGameObject::ScaleObject(float val)
{
	D2D_VECTOR_2F _vec = m_Transform->GetLocalScale();

	_vec.x += val;
	_vec.y += val;

	m_Transform->SetLocalScale(_vec);
}

